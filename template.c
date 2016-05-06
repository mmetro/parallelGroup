

/***************************************************************************/
/* Includes ****************************************************************/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include "clcg4.h"
#include <mpi.h>


/***************************************************************************/
/* Extra Utility Macros and Defines ****************************************/
/***************************************************************************/

// Define if running not on the BGQ
// #define __LOCAL__


// This gets clock cycle count locally
#ifdef __LOCAL__
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#else
  #include <hwi/include/bqc/A2_inlines.h>
#endif


/***************************************************************************/
/* Defines *****************************************************************/
/***************************************************************************/

#define __MATRIX_SIZE__ 10


#define TRUE 1
#define FALSE  0

typedef struct Ant {
  double foodEaten;
  unsigned int x, y;
  int state;
} Ant;

typedef struct param_t{
	int mystart;
	int myend;
	pthread_mutex_t * foodlock;
	pthread_mutex_t * ticklock;
	int threadnum;
} param_t;

// MOVE_TO: increments a cell's occupancy
// MOVE_FROM: decrements a cell's occupancy
// SPRAY_PHEREMONE: increment pheremone levels
// no eat action, because it's assumed ants will always eat
// TODO: maybe make an EAT action
typedef enum e_ActionType
{
    MOVE_TO, MOVE_FROM, SPRAY_PHEREMONE, SPRAY_FOUND, SPRAY_NEG, EAT;

} ActionType;


/***************************************************************************/
/* Global Vars *************************************************************/
/***************************************************************************/

double ** g_localFoodGrid;
double *** g_remoteFoodGrids;
double ** g_localPheremoneGrid;
double *** g_remotePheremoneGrids;
int ** g_localOccupancyGrid;
int *** g_remoteOccupancyGrids;
MPI_Win winfood, winph, winoc;
unsigned int * tickcounts;

unsigned int g_array_size = 0;
unsigned int my_array_size = 0;
unsigned int standard_array_size = 0;
int g_num_threads = 0;
int g_num_ants = 0;
int g_total_food = 0;

double g_food_thresh_hold = 0.0;

int mpi_myrank = -1;
int mpi_commsize = -1;

// each rank owns some ants
unsigned int myNumAnts;
Ant * myAnts;

#ifdef __LOCAL__
  double clockrate = 2.523e9;
#else
  double clockrate = 1.6e9;
#endif


/***************************************************************************/
/* Function Decs ***********************************************************/
/***************************************************************************/

// Function to process arguments outside of main to keep it readable
void process_arguments(int argc, char* argv[]);

//init
void allocate_and_init_array();

//simulation
void *run_farm( void * pt);

//run one iteration
void run_tick(param_t * p);
void update_total_food();


// helper functions
void spray(int x, int y, int occupancy);
void undo_spray(int x, int y, int occupancy);

// Timing
unsigned long long get_Time();



/***************************************************************************/
/* Function: Main **********************************************************/
/***************************************************************************/

int main(int argc, char *argv[])
{
  // Keep track of time
  unsigned long long start_time=0;
  unsigned long long end_time=0;
  double compute_time = 0;
  int provided;

// Example MPI startup and using CLCG4 RNG
  MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  MPI_Comm_size( MPI_COMM_WORLD, &mpi_commsize);
  MPI_Comm_rank( MPI_COMM_WORLD, &mpi_myrank);

  if (mpi_myrank == 0) {
    start_time = get_Time();
  }

// Init 16,384 RNG streams - each row and each ant has an independent stream
  InitDefault();

  mpi_log_debug("Rank %d of %d started.\n", mpi_myrank, mpi_commsize);

  // Read in and process the remaining arguments.
  process_arguments(argc, argv);

  // Allocate the chunk of the universe that we need for this rank, and
  // randomly initialize the values in that chunk using the proper RNG stream
  // for that chunk.
  allocate_and_init_array();

  MPI_Barrier( MPI_COMM_WORLD );

  mpi_log_debug("Rank %d: Initialization complete. Ant farm started...\n", mpi_myrank);

  //Run simulation 
  int i;
  pthread_mutex_t foodlock;//lock for editing/checking recprogress
  pthread_mutex_t ticklock;//lock for editing/checking progress
  pthread_mutex_init(&foodlock, NULL);
  pthread_mutex_init(&ticklock, NULL);
  int ants_per_thread = myNumAnts/g_num_threads;
  pthread_t p[g_num_threads];//keep track of each thread
  for (i = 0; i < g_num_threads;i++){
    param_t * p = (param_t *)malloc(1*sizeof(param_t));
    p->mystart=ants_per_thread*i;
    p->myend = ants_per_thread*(i+1);
    p->foodlock = &foodlock;
    p->ticklock = &ticklock;
    p->threadnum = i;
    pthread_create(&(p[i-1]), NULL,run_farm,(void *)p); 
  }
  param_t * p = (param_t *)malloc(1*sizeof(param_t));
  p->mystart=ants_per_thread*g_num_threads;
  p->myend = myNumAnts;
  p->foodlock = &foodlock;
  p->ticklock = &ticklock;
  p->threadnum = g_num_threads;
  run_farm((void *)p);  
  
  // // Barrier after completion
  MPI_Barrier( MPI_COMM_WORLD );
  pthread_mutex_destroy(&foodlock);
  pthread_mutex_destroy(&ticklock);

  if (mpi_myrank == 0) {
    mid_cycle_time = get_cycles();

    compute_time = (mid_cycle_time - start_cycle_time) / clockrate;
    printf("Simulation duration:\t%f seconds\n", compute_time);
    mpi_log_debug(" Simulation Complete!");
  }


  MPI_Finalize();
  return 0;

}

/***************************************************************************/
/* Function: process_arguments *********************************************/
/***************************************************************************/
/* This function takes in the argc and argv values and processes them,     */
/*  saving their values in the proper global variables. It will terminate  */
/*  the program upon failure.                                              */
/***************************************************************************/
void process_arguments(int argc, char* argv[]) {
  int threads = -1;
  int size = -1;
  int ants = -1;
  int food = -1;
  double food_respawn = 0;
  // Argument 1 is the number of threads to use per rank
  // Argument 2 is the size of matrix
  // Argument 3 is the number of ants
  // Argument 4 is the amount of food in the system 
  // Argument 5 is the food respawn chance.  It can be used to make the simulation endless, but should be left at 0 for now 

  // Check for 5 arguments
  if (argc != 6) {
    mpi_log_error("Usage: %s <num of ants> <matrix size> <num of ants> <num of food>\n", argv[0]);
    exit(1);
  }
  else
  {
    // Next, read in the number of threads
    mpi_log_debug("Read in number of threads: %s\n", argv[1]);
    threads = atoi(argv[1]);
    
    // Read in the matrix size
    mpi_log_debug("Read in matrix size: %s\n", argv[2]);
    size = atoi(argv[2]);

    // Read in the number of ants
    mpi_log_debug("Read in number of ants: %s\n", argv[3]);
    ants = atoi(argv[3]);

    // Read in the total amount of food
    mpi_log_debug("Read in amount of food: %s\n", argv[4]);
    food = atoi(argv[4]);

    // Read in the food respawn chance
    mpi_log_debug("Read in food respawn chance %s\n", argv[5]);
    food_respawn = ((double)atoi(argv[5])) / 100.0;

    if (size <= 0)
    {
      mpi_log_debug("Matrix size not defined, setting to defined value: %u.\n", __MATRIX_SIZE__);
      size = __MATRIX_SIZE__;
    }

    // Validate this input, and then save it in the global variables
    if (threads < 0)
    {
      mpi_log_error("ERROR: Number of threads less than 0. Please try again.\n");
      exit(1);
    }

    if (ants <= 0)
    {
      mpi_log_error("ERROR: Cannot have less than 1 ant.\n");
      exit(1);
    }

    if (food <= 0)
    {
      mpi_log_error("ERROR: Cannot have less than 1 food.\n");
      exit(1);
    }

    if (food_respawn < 0.0 || food_respawn > 1.0)
    {
      mpi_log_error("ERROR: Food respawn rate must be between 0 and 100 percent.\n");
      exit(1);
    }

    // Time to set the globals.
    g_array_size = size;
    g_num_ants = ants;
    g_total_food = food;
    g_food_thresh_hold = food_respawn;
    g_num_threads = threads;

    // each rank owns some ants
    myNumAnts = g_num_ants / mpi_commsize;
    int modrem = g_num_ants % mpi_commsize;
    if (mpi_myrank <modrem)
      myNumAnts++;
    my_array_size = g_array_size/mpi_commsize;
    standard_array_size = my_array_size;
    if (mpi_mpirank == mpi_commsize-1)
        my_array_size+=g_array_size%mpi_commsize;
  }
}


/***************************************************************************/
/* Function: allocate_and_init_array ***************************************/
/***************************************************************************/

void allocate_and_init_array()
{
  unsigned int numrows = g_array_size/mpi_commsize;
  unsigned int rowstart = mpi_myrank*numrows;
  unsigned int row = 0;
  unsigned int rowend = rowstart+my_array_size;
  unsigned int col = 0;
  unsigned int i;
  unsigned int foodheap = g_total_food/mpi_commsize;

  // each rank has a local copy of the world
  // ranks update their local copies, gasking only for the pieces of the world near their ants

  MPI_Alloc_mem(sizeof(double*)*my_array_size,MPI_INFO_NULL, &g_localFoodGrid);
  MPI_Alloc_mem(sizeof(double*)*my_array_size,MPI_INFO_NULL, &g_localPheremoneGrid);
  MPI_Alloc_mem(sizeof(int*)*my_array_size,MPI_INFO_NULL, &g_localOccupancyGrid);
  for (row = 0; row < my_array_size; row++)//rowstart; row < rowend; row++)
  {
  MPI_Alloc_mem(sizeof(double)*g_array_size,MPI_INFO_NULL, &(g_localFoodGrid[row]));
  MPI_Alloc_mem(sizeof(double)*g_array_size,MPI_INFO_NULL, &(g_localPheremoneGrid[row]));
  MPI_Alloc_mem(sizeof(int)*g_array_size,MPI_INFO_NULL, &(g_localOccupancyGrid[row]));
    for (col = 0; col < g_array_size; col++)
    {
      g_localFoodGrid[row][col] = 0;
      g_localPheremoneGrid[row][col] = 0;
      g_localOccupancyGrid[row][col] = 0;
    }
  }
  MPI_Barrier( MPI_COMM_WORLD );
  MPI_Win_allocate_shared(my_array_size*g_array_size*sizeof(double),sizeof(double),MPI_INFO_NULL,MPI_COMM_WORLD, &g_localFoodGrid,&winfood);
  MPI_Win_allocate_shared(my_array_size*g_array_size*sizeof(double),sizeof(int),MPI_INFO_NULL,MPI_COMM_WORLD, &g_localPheremoneGrid,&winfood);
  MPI_Win_allocate_shared(my_array_size*g_array_size*sizeof(int),sizeof(int),MPI_INFO_NULL,MPI_COMM_WORLD, &g_localOccupancyGrid,&winfood);
  MPI_Win_fence(0,winfood);
  MPI_Win_fence(0,winph);
  MPI_Win_fence(0,winoc);
  g_remoteFoodGrids=(double ***)malloc(mpi_commsize*sizeof(double **));
  g_remotePheremoneGrids=(int ***)malloc(mpi_commsize*sizeof(double **));
  g_remoteOccupancyGrids=(int ***)malloc(mpi_commsize*sizeof(int **));
  for (i=0; i < mpi_commsize;i++)
  {
    if (i != mpi_myrank)
    {
      int sz, disp;
      MPI_Win_shared_query(winfood,i,&sz, &disp, &(g_remoteFoodGrids[i]));
      MPI_Win_shared_query(winph,i,&sz, &disp, &(g_remotePheremoneGrids[i]));
      MPI_Win_shared_query(winoc,i,&sz, &disp, &(g_remoteOccupancyGrids[i]));
    }
    else
    {
      g_remoteFoodGrids[i]=g_localFoodGrid;
      g_remotePheremoneGrids[i]=g_localPheremoneGrid;
      g_remoteOccupancyGrids[i]=g_localOccupancyGrid;
    }
  }
  MPI_Win_fence(0,winfood);
  MPI_Win_fence(0,winph);
  MPI_Win_fence(0,winoc);
  
  //place food pices at random
  while (foodheap > 0){
    //row = rowstart+(50*g_array_size*GenVal((mpi_myrank)%Maxgen))%(rowend-rowstart);
    row = (50*g_array_size*GenVal((mpi_myrank)%Maxgen))%(my_array_size);
    col = (50*g_array_size*GenVal((mpi_myrank)%Maxgen))%g_array_size;
    g_localFoodGrid[row][col]+= 1;
    foodheap--;
  }

  MPI_Barrier( MPI_COMM_WORLD );
  // initialize the ants
  myAnts = malloc(myNumAnts*sizeof(Ant));
  for(i = 0; i < myNumAnts; i++)
  {
    myAnts[i].foodEaten = 0;
    myAnts[i].state = 0;
    myAnts[i].x = (unsigned int) (GenVal(i%Maxgen) * g_array_size) % g_array_size;
    myAnts[i].y = (unsigned int) (GenVal(i%Maxgen) * g_array_size) % g_array_size;
    int rank = y/standard_array_size;
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winoc);
    MPI_Win_fence(0,winoc);
    int currentoccupancy;
    MPI_Get(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
    currentoccupancy++;
    MPI_Put(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
    MPI_Win_unlock(rank,winoc);
    MPI_Win_fence(0,winoc);
  }
  tickcounts = (unsigned int *)calloc(g_num_threads+1,sizeof(unsigned int));
}


/***************************************************************************/
/* Function: run_farm ******************************************************/
/***************************************************************************/

void *run_farm(void *pt) {
  param_t p = (param_t *)pt;
  int mystart = p->mystart;
  int myend = p->myend;
  pthread_mutex_t * foodlock = p->foodlock;
  pthread_mutex_t * ticklock = p->ticklock;
  int threadnum = p->threadnum;
  while (g_total_food > 0) 
  {
    // run ant decisions
    run_tick(p);
    MPI_Barrier( MPI_COMM_WORLD );

    //every 5 ticks
      // update_total_food()
      // world rank sends g_total_food to ranks 
      
      // world rank decrements any pheremone levels by 1
  }
  free(p);
  return NULL;
}

/***************************************************************************/
/* Function: run_tick ******************************************************/
/***************************************************************************/
// run ant decisions
void run_tick(param_t * p) {
  int mystart = p->mystart;
  int myend = p->myend;
  pthread_mutex_t * foodlock = p->foodlock;
  pthread_mutex_t * ticklock = p->ticklock;
  int threadnum = p->threadnum;
  unsigned int i,j,k;
  unsigned int x,y;
  unsigned int ny, ny;
  j = 0;
  //loop through ants
  for(i = mystart; i < myend; i++)
    {
      x = myAnts[i].x;
      y = myAnts[i].y;
      double foodremaining;
      double pheremoneLevel[3][3];
      int occupancy;
      int ranktop,rankmid,rankbottom;
      ranktop = (y-1)/standard_array_size;
      rankmid = (y)/standard_array_size;
      rankbottom = (y+1)/standard_array_size;
      unsigned int xstart=x;
      if (x>0)
        xstart--;
      unsigned int xend = x;
      if (x<g_array_size-1)
        xend++;
      unsigned int arraylen=xend-xstart+1;
      int arraystart=xstart-x+1;
      
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winoc);
      MPI_Win_fence(0,winoc);
      MPI_Get(&occupancy,1,MPI_INT,rankmid,y*g_array_size+x,1,MPI_INT,winoc);
      MPI_Win_unlock(rankmid,winoc);
      MPI_Win_fence(0,winoc);
      
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winfood);
      MPI_Win_fence(0,winfood);
      MPI_Get(&foodremaining,1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
      MPI_Win_unlock(rankmid,winfood);
      MPI_Win_fence(0,winfood);
      
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winph);
      MPI_Win_fence(0,winph);
      MPI_Get(&(pheremonLevel[1][arraystart]),arraylen,MPI_DOUBLE,rankmid,y*g_array_size+xstart,arraylen,MPI_DOUBLE,winph);
      MPI_Win_unlock(rankmid,winph);
      MPI_Win_fence(0,winph);

      if (y>0)
      {      
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,ranktop,0,winph);
        MPI_Win_fence(0,winph);
        MPI_Get(&(pheremonLevel[0][arraystart]),arraylen,MPI_DOUBLE,ranktop,(y-1)*g_array_size+xstart,arraylen,MPI_DOUBLE,winph);
        MPI_Win_unlock(ranktop,winph);
        MPI_Win_fence(0,winph);
      }
      
      if (y<g_array_size-1)
      { 
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankbottom,0,winph);
        MPI_Win_fence(0,winph);
        MPI_Get(&(pheremonLevel[2][arraystart]),arraylen,MPI_DOUBLE,rankbottom,(y+1)*g_array_size+xstart,arraylen,MPI_DOUBLE,winph);
        MPI_Win_unlock(rankbottom,winph);
        MPI_Win_fence(0,winph);
      }
      
      MPI_Barrier( MPI_COMM_WORLD );
      if (foodremaining > 0 && foodremaining < occupancy)
      {
        double foodtoeat=foodremaining/occupancy;
        pthread_mutex_lock(foodlock);
        g_total_food-=foodremaining;
        pthread_mutex_unlock(foodlock);
        foodremaining=0;
        myAnts[i].foodEaten+=foodtoeat;
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winfood);
        MPI_Win_fence(0,winfood);
        MPI_Put(&foodremaining,1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
        MPI_Win_unlock(rankmid,winfood);
        MPI_Win_fence(0,winfood);
        if (pheremoneLevel[1][1]!= 100)
          spray(x,y,occupancy);
      }  
      else if (foodremaining > 0)
      {
        myAnts[i].foodEaten++;
        pthread_mutex_lock(foodlock);
        g_total_food--;
        pthread_mutex_unlock(foodlock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winfood);
        MPI_Win_fence(0,winfood);
        MPI_Get(&foodremaining,1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
        foodremaining--;
        MPI_Put(&foodremaining,1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
        MPI_Win_unlock(rankmid,winfood);
        MPI_Win_fence(0,winfood);
        undo_spray(x,y,occupancy);
      }
      else
      {
        nx=x;
        ny=y;
        double maxph=pheremonLevel[1][1];
        for (j=0;j<3;j++)
        {
          if ((j==0 && y > 0) || (j==2 && y < g_array_size-1))
          {
            for (k=arraystart;k<arraystart+arraylen;k++)
            {
              double phlevel = pheremoneLevel[j][k];
              if (phlevel>maxph)
              {
                maxph=phlevel;
                nx=x+k-1;
                ny=y+j-1;
              }  
            }
          }
        }
        while (x==nx && y==ny)
        {
          unsigned int dx = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;
          unsigned int dy = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;
          while((dx==0 && !(x >0)) || (dx==2 && !(x<g_array_size-1)))
            dx = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;
          while((dy==0 && !(y >0)) || (dy==2 && !(y<g_array_size-1)))
            dy = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;          
          nx=x+dx
          ny=y+dy;
        }
        myAnts[i].x=nx;
        myAnts[i].y=ny;
        int rank = y/standard_array_size;
        int currentoccupancy;
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winoc);
        MPI_Win_fence(0,winoc);
        MPI_Get(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
        currentoccupancy--;
        MPI_Put(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
        MPI_Win_unlock(rank,winoc);
        MPI_Win_fence(0,winoc);
        rank=ny/standard_array_size;
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winoc);
        MPI_Win_fence(0,winoc);
        MPI_Get(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
        currentoccupancy++;
        MPI_Put(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
        MPI_Win_unlock(rank,winoc);
        MPI_Win_fence(0,winoc);
      }
    }
}

/***************************************************************************/
/* Function: get_Time ******************************************************/
/***************************************************************************/
unsigned long long get_Time()
{
  #ifdef __LOCAL__
    return rdtsc();
  #else
    return GetTimeBase();
  #endif
}


/***************************************************************************/
/* Function: spray *********************************************************/
/***************************************************************************/

//sprays pheremones on adjacent Cells
void spray(int x, int y, int occupancy)
{
  int arraydimension=19;
  double pheremoneLevel[arraydimension];
  unsigned int xstart=x;
  if (xstart < (arraydimension-1)/2)
    xstart=0;
  else
    xstart-=(arraydimension-1)/2;
  unsigned int xend = x;
  if (xend<g_array_size-(arraydimension-1)/2)
    xend+=(arraydimension-1)/2;
  else
    xend=g_array_size-1;
  unsigned int arraylen=xend-xstart+1;
  int arraystart=xstart-x+1;
  unsigned int ystart=y;
  if (ystart < (arraydimension-1)/2)
    ystart=0;
  else
    ystart-=(arraydimension-1)/2;
  unsigned int yend = y;
  if (yend<g_array_size-(arraydimension-1)/2)
    yend+=(arraydimension-1)/2;
  else
    yend=g_array_size-1;
  int i,j;
  
  for (i=ystart;i<yend;i++)
  {
    int rank = i/standard_array_size;
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winph);
    MPI_Win_fence(0,winph);
    MPI_Get(&(pheremonLevel[arraystart]),arraylen,MPI_DOUBLE,rank,i*g_array_size+xstart,arraylen,MPI_DOUBLE,winph);
    for (j=arraystart; j < arraylen+arraystart;j++)
    {
      double spraylevel= 100.0-10.0/occupancy*(j-(arraydimension-1)/2);
      if (spraylevel < 0)
        spraylevel*=-1;
      pheremoneLevel[j]+=spraylevel;
    }
    MPI_Put(&(pheremonLevel[arraystart]),arraylen,MPI_DOUBLE,rank,i*g_array_size+xstart,arraylen,MPI_DOUBLE,winph);
    MPI_Win_unlock(rank,winph);
    MPI_Win_fence(0,winph);
  }
}


/***************************************************************************/
/* Function: undo_spray ****************************************************/
/***************************************************************************/

//sprays pheremones on adjacent Cells
void undo_spray(int x, int y, int occupancy)
{
  int arraydimension=19;
  double pheremoneLevel[arraydimension];
  unsigned int xstart=x;
  if (xstart < (arraydimension-1)/2)
    xstart=0;
  else
    xstart-=(arraydimension-1)/2;
  unsigned int xend = x;
  if (xend<g_array_size-(arraydimension-1)/2)
    xend+=(arraydimension-1)/2;
  else
    xend=g_array_size-1;
  unsigned int arraylen=xend-xstart+1;
  int arraystart=xstart-x+1;
  unsigned int ystart=y;
  if (ystart < (arraydimension-1)/2)
    ystart=0;
  else
    ystart-=(arraydimension-1)/2;
  unsigned int yend = y;
  if (yend<g_array_size-(arraydimension-1)/2)
    yend+=(arraydimension-1)/2;
  else
    yend=g_array_size-1;
  int i,j;
  
  for (i=ystart;i<yend;i++)
  {
    int rank = i/standard_array_size;
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winph);
    MPI_Win_fence(0,winph);
    MPI_Get(&(pheremonLevel[arraystart]),arraylen,MPI_DOUBLE,rank,i*g_array_size+xstart,arraylen,MPI_DOUBLE,winph);
    for (j=arraystart; j < arraylen+arraystart;j++)
    {
      double spraylevel= 100.0-10.0/occupancy*(j-(arraydimension-1)/2);
      if (spraylevel < 0)
        spraylevel*=-1;
      pheremoneLevel[j]-=spraylevel;
    }
    MPI_Put(&(pheremonLevel[arraystart]),arraylen,MPI_DOUBLE,rank,i*g_array_size+xstart,arraylen,MPI_DOUBLE,winph);
    MPI_Win_unlock(rank,winph);
    MPI_Win_fence(0,winph);
  }
}
