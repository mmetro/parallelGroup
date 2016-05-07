

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
  unsigned int alreadysprayed;
} Ant;

typedef struct param_t{
	int mystart;
	int myend;
	pthread_mutex_t * foodlock;
	pthread_mutex_t * ticklock;
	pthread_mutex_t * winfoodlock;
	pthread_mutex_t * winphlock;
	pthread_mutex_t * winoclock;
	int threadnum;
} param_t;


/***************************************************************************/
/* Global Vars *************************************************************/
/***************************************************************************/

double ** g_localFoodGrid;
int ** g_localPheremoneGrid;
int ** g_localOccupancyGrid;
MPI_Win winfood, winph, winoc,wintotalfood;
unsigned int * tickcounts;

unsigned int g_array_size = 0;
unsigned int my_array_size = 0;
unsigned int standard_array_size = 0;
unsigned int g_num_threads = 0;
unsigned int g_num_ants = 0;
double * g_total_food = NULL;

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


// helper functions
void spray(int x, int y, pthread_mutex_t * winphlock);
void undo_spray(int x, int y, pthread_mutex_t * winphlock);

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

  printf("Rank %d of %d started.\n", mpi_myrank, mpi_commsize);

  // Read in and process the remaining arguments.
  process_arguments(argc, argv);

  // Allocate the chunk of the universe that we need for this rank, and
  // randomly initialize the values in that chunk using the proper RNG stream
  // for that chunk.
  allocate_and_init_array();

  MPI_Barrier( MPI_COMM_WORLD );

  printf("Rank %d: Initialization complete. Ant farm starting...\n", mpi_myrank);

  //Run simulation 
  int i;
  pthread_mutex_t foodlock;//lock for editing/checking g_total_food
  pthread_mutex_t ticklock;//lock for editing/checking progress
  pthread_mutex_t winfoodlock;//lock for editing/checking winfood
  pthread_mutex_t winphlock;//lock for editing/checking winph
  pthread_mutex_t winoclock;//lock for editing/checking winoc
  pthread_mutex_init(&foodlock, NULL);
  pthread_mutex_init(&ticklock, NULL);
  pthread_mutex_init(&winfoodlock, NULL);
  pthread_mutex_init(&winphlock, NULL);
  pthread_mutex_init(&winoclock, NULL);
  int ants_per_thread = myNumAnts/(g_num_threads+1);
  pthread_t pthreads[g_num_threads];//keep track of each thread
  for (i = 0; i < g_num_threads;i++){
    param_t * p = (param_t *)malloc(1*sizeof(param_t));
    p->mystart=ants_per_thread*i;
    p->myend = ants_per_thread*(i+1);
    p->foodlock = &foodlock;
    p->ticklock = &ticklock;
    p->ticklock = &winfoodlock;
    p->ticklock = &winphlock;
    p->ticklock = &winoclock;
    p->threadnum = i;
    pthread_create(&(pthreads[i]), NULL,run_farm,(void *)p); 
    printf("Rank %d: thread %d  of %u will handle ants %d thru %d\n", mpi_myrank,i, g_num_threads,p->mystart,p->myend);
  }
  param_t * p = (param_t *)malloc(1*sizeof(param_t));
  p->mystart=ants_per_thread*g_num_threads;
  p->myend = myNumAnts;
  p->foodlock = &foodlock;
  p->ticklock = &ticklock;
  p->ticklock = &winfoodlock;
  p->ticklock = &winphlock;
  p->ticklock = &winoclock;
  p->threadnum = g_num_threads;
  printf("Rank %d: main thread will handle ants %d thru %d\n", mpi_myrank,p->mystart,p->myend);
  run_farm((void *)p);  
  
  // // Barrier after completion
  MPI_Barrier( MPI_COMM_WORLD );
  pthread_mutex_destroy(&foodlock);
  pthread_mutex_destroy(&ticklock);
  pthread_mutex_destroy(&winfoodlock);
  pthread_mutex_destroy(&winphlock);
  pthread_mutex_destroy(&winoclock);

  if (mpi_myrank == 0) {
    end_time = time(NULL);//get_cycles();

    compute_time = (end_time - start_time) / clockrate;
    printf("Simulation duration:\t%f seconds\n", compute_time);
    printf(" Simulation Complete!\n");
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
  double food = -1;
  double food_respawn = 0;
  // Argument 1 is the number of threads to use per rank
  // Argument 2 is the size of matrix
  // Argument 3 is the number of ants
  // Argument 4 is the amount of food in the system 
  // Argument 5 is the food respawn chance.  It can be used to make the simulation endless, but should be left at 0 for now 

  // Check for 5 arguments
  if (argc != 5) {
    printf("Usage: %s <num of threads> <matrix size> <num of ants> <num of food>\n", argv[0]);
    exit(1);
  }
  else
  {
    // Next, read in the number of threads
    threads = atoi(argv[1]);
    printf("Read in number of threads: %d\n", threads);
    
    // Read in the matrix size
    size = atoi(argv[2]);
    printf("Read in matrix size: %d\n", size);

    // Read in the number of ants
    ants = atoi(argv[3]);
    printf("Read in number of ants: %d\n", ants);

    // Read in the total amount of food
    food = (double)atoi(argv[4]);
    printf("Read in amount of food: %lf\n", food);

    // Read in the food respawn chance
    //food_respawn = ((double)atoi(argv[5])) / 100.0;
    printf("Read in food respawn chance %lf\n", food_respawn);
    
    if (size <= 0)
    {
      printf("Matrix size not defined, setting to defined value: %u.\n", __MATRIX_SIZE__);
      size = __MATRIX_SIZE__;
    }

    // Validate this input, and then save it in the global variables
    if (threads < 0)
    {
      printf("ERROR: Number of threads less than 0. Please try again.\n");
      exit(1);
    }

    if (ants <= 0)
    {
      printf("ERROR: Cannot have less than 1 ant.\n");
      exit(1);
    }

    if (food <= 0)
    {
      printf("ERROR: Cannot have less than 1 food.\n");
      exit(1);
    }

    if (food_respawn < 0.0 || food_respawn > 1.0)
    {
      printf("ERROR: Food respawn rate must be between 0 and 100 percent.\n");
      exit(1);
    }

    // Time to set the globals.
    g_array_size = (unsigned int)size;
    g_num_ants = (unsigned int)ants+0;
    printf("Rank %d: gnumants=%u(=ants=%d)\n",mpi_myrank,g_num_ants,ants);
    g_food_thresh_hold = food_respawn;
    g_num_threads = (unsigned int)threads;
    MPI_Alloc_mem(sizeof(double)*1,MPI_INFO_NULL, &g_total_food);
    MPI_Win_allocate_shared(1*sizeof(double),sizeof(double),MPI_INFO_NULL,MPI_COMM_WORLD, &g_total_food,&wintotalfood);
    MPI_Win_fence(0,wintotalfood);
    
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,mpi_myrank,0,wintotalfood);
    MPI_Win_fence(0,wintotalfood);
    MPI_Put(&food,1,MPI_DOUBLE,0,mpi_myrank,1,MPI_DOUBLE,wintotalfood);
    MPI_Win_unlock(mpi_myrank,wintotalfood);
    MPI_Win_fence(0,wintotalfood);

    // each rank owns some ants
    myNumAnts = g_num_ants / mpi_commsize;
    unsigned int modrem = g_num_ants % mpi_commsize;
    if (mpi_myrank <modrem)
      myNumAnts++;
    my_array_size = g_array_size/mpi_commsize;
    standard_array_size = my_array_size;
    if (mpi_myrank == mpi_commsize-1)
        my_array_size+=g_array_size%mpi_commsize;
    printf("Rank %d: standardarrsize=%u,myarrsize=%u,mynumants=%u,modrem=%u,gnumants=%u(=ants=%d),commsize=%d\n",mpi_myrank,standard_array_size,my_array_size,myNumAnts,modrem,g_num_ants,ants,mpi_commsize);
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
  double total_food;
  MPI_Win_lock(MPI_LOCK_EXCLUSIVE,mpi_myrank,0,wintotalfood);
  MPI_Win_fence(0,wintotalfood);
  MPI_Get(&total_food,1,MPI_DOUBLE,0,mpi_myrank,1,MPI_DOUBLE,wintotalfood);
  MPI_Win_unlock(mpi_myrank,wintotalfood);
  MPI_Win_fence(0,wintotalfood);
  double foodheap = total_food/mpi_commsize;

  // each rank has a local copy of the world
  // ranks update their local copies, gasking only for the pieces of the world near their ants

  MPI_Alloc_mem(sizeof(double*)*my_array_size,MPI_INFO_NULL, &g_localFoodGrid);
  MPI_Alloc_mem(sizeof(int*)*my_array_size,MPI_INFO_NULL, &g_localPheremoneGrid);
  MPI_Alloc_mem(sizeof(int*)*my_array_size,MPI_INFO_NULL, &g_localOccupancyGrid);
  for (row = 0; row < my_array_size; row++)//rowstart; row < rowend; row++)
  {
  MPI_Alloc_mem(sizeof(double)*g_array_size,MPI_INFO_NULL, &(g_localFoodGrid[row]));
  MPI_Alloc_mem(sizeof(int)*g_array_size,MPI_INFO_NULL, &(g_localPheremoneGrid[row]));
  MPI_Alloc_mem(sizeof(int)*g_array_size,MPI_INFO_NULL, &(g_localOccupancyGrid[row]));
    for (col = 0; col < g_array_size; col++)
    {
      printf("Rank %d: initializing (%u,%u) out of (%u,%u)...\n",mpi_myrank,row,col, my_array_size,g_array_size);
      g_localFoodGrid[row][col] = 0;
      g_localPheremoneGrid[row][col] = 0;
      g_localOccupancyGrid[row][col] = 0;
      printf("Rank %d: initialized (%u,%u) out of (%u,%u) to f=%lf,ph=%d,o=%d...\n",mpi_myrank,row,col, my_array_size,g_array_size, g_localFoodGrid[row][col], g_localPheremoneGrid[row][col], g_localOccupancyGrid[row][col]);
    }
  }
  MPI_Barrier( MPI_COMM_WORLD );
  MPI_Win_allocate_shared(my_array_size*g_array_size*sizeof(double),sizeof(double),MPI_INFO_NULL,MPI_COMM_WORLD, &g_localFoodGrid,&winfood);
  MPI_Win_allocate_shared(my_array_size*g_array_size*sizeof(int),sizeof(int),MPI_INFO_NULL,MPI_COMM_WORLD, &g_localPheremoneGrid,&winph);
  MPI_Win_allocate_shared(my_array_size*g_array_size*sizeof(int),sizeof(int),MPI_INFO_NULL,MPI_COMM_WORLD, &g_localOccupancyGrid,&winoc);
  MPI_Win_fence(0,winfood);
  MPI_Win_fence(0,winph);
  MPI_Win_fence(0,winoc);
  printf("Rank %d: allocated and shared memory\n",mpi_myrank);
  
  //place food pices at random
  while (foodheap > 0){
    //row = rowstart+(50*g_array_size*GenVal((mpi_myrank)%Maxgen))%(rowend-rowstart);
    row = ((unsigned int)(50*g_array_size*GenVal((mpi_myrank)%Maxgen)))%(my_array_size);
    col = ((unsigned int)(50*g_array_size*GenVal((mpi_myrank)%Maxgen)))%g_array_size;
    printf("Rank %d: placing food at (%u,%u) out of (%u,%u)...\n",mpi_myrank,row,col, my_array_size,g_array_size);
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,mpi_myrank,0,winfood);
    MPI_Win_fence(0,winfood);
    double currentfood;
    MPI_Get(&currentfood,1,MPI_DOUBLE,mpi_myrank,row*g_array_size+col,1,MPI_DOUBLE,winfood);
    currentfood++;
    MPI_Put(&currentfood,1,MPI_DOUBLE,mpi_myrank,row*g_array_size+col,1,MPI_DOUBLE,winfood);
    MPI_Win_unlock(mpi_myrank,winfood);
    MPI_Win_fence(0,winfood);
    printf("Rank %d: placed food at (%u,%u) out of (%u,%u).\n",mpi_myrank,row,col, my_array_size,g_array_size);
    foodheap--;
  }
  printf("Rank %d: allocated food\n",mpi_myrank);

  MPI_Barrier( MPI_COMM_WORLD );
  // initialize the ants
  myAnts = (Ant *)malloc(myNumAnts*sizeof(Ant));
  printf("Rank %d: initializing %u ants\n",mpi_myrank,myNumAnts);
  for(i = 0; i < myNumAnts; i++)
  {
    myAnts[i].foodEaten = 0;
    myAnts[i].x = (unsigned int) (g_array_size+GenVal(i%Maxgen) * g_array_size) % g_array_size;
    myAnts[i].y = (unsigned int) (g_array_size+GenVal(i%Maxgen) * g_array_size) % g_array_size;
    myAnts[i].alreadysprayed = 0;
    unsigned int x = myAnts[i].x;
    unsigned int y = myAnts[i].y;
    int rank = y/standard_array_size;
    printf("Rank %d: placing ant %d, at (%u,%u) in rank %d\n",mpi_myrank,i,x,y,rank);
    printf("Rank %d: placing ant %d, requesting lock\n",mpi_myrank,i);
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winoc);
    printf("Rank %d: placing ant %d, requested lock\n",mpi_myrank,i);
    MPI_Win_fence(0,winoc);
    int currentoccupancy;
    printf("Rank %d: placing ant %d, lock achieved\n",mpi_myrank,i);
    MPI_Get(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
    currentoccupancy++;
    MPI_Put(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
    MPI_Win_unlock(rank,winoc);
    MPI_Win_fence(0,winoc);
  }
  printf("Rank %d: initialized ants\n",mpi_myrank);
  tickcounts = (unsigned int *)calloc(g_num_threads+1,sizeof(unsigned int));
  printf("Rank %d: initialized tickcounts\n",mpi_myrank);
}


/***************************************************************************/
/* Function: run_farm ******************************************************/
/***************************************************************************/

void *run_farm(void *pt) {
  param_t * p = (param_t *)pt;
  int mystart = p->mystart;
  int myend = p->myend;
  pthread_mutex_t * foodlock = p->foodlock;
  pthread_mutex_t * ticklock = p->ticklock;
  int threadnum = p->threadnum;
  double total_food;
  pthread_mutex_lock(foodlock);
  MPI_Win_lock(MPI_LOCK_EXCLUSIVE,0,0,wintotalfood);
  MPI_Win_fence(0,wintotalfood);
  MPI_Get(&total_food,1,MPI_DOUBLE,0,0,1,MPI_DOUBLE,wintotalfood);
  MPI_Win_unlock(0,wintotalfood);
  MPI_Win_fence(0,wintotalfood);  
  pthread_mutex_unlock(foodlock);
  while (total_food > 0) 
  {
    // run ant decisions
    run_tick(p);
    unsigned int finished;
    if (threadnum!= g_num_threads)
    {
      pthread_mutex_lock(ticklock);
      unsigned int myprogress = ++tickcounts[threadnum];
      unsigned int mainprogress = tickcounts[g_num_threads];
      pthread_mutex_unlock(ticklock); 
      printf("Rank %d: Thread %d waiting on main.\n",mpi_myrank, threadnum);
      while (myprogress!=mainprogress)
      {
        pthread_mutex_lock(ticklock);
        mainprogress = tickcounts[g_num_threads];
        pthread_mutex_unlock(ticklock); 
      }
    }
    else
    {
      int i;
      pthread_mutex_lock(ticklock);
      finished = tickcounts[g_num_threads]+1;
      pthread_mutex_unlock(ticklock); 
      for (i = 0; i < g_num_threads;i++)
      {
        pthread_mutex_lock(ticklock);
        unsigned int threadprogress = tickcounts[i];
        pthread_mutex_unlock(ticklock); 
        printf("Rank %d: main waiting on thread %d.\n",mpi_myrank,i);
        while(threadprogress != finished)
        {
          pthread_mutex_lock(ticklock);
          threadprogress = tickcounts[i];
          pthread_mutex_unlock(ticklock);
        }
      }
      MPI_Barrier( MPI_COMM_WORLD );
      pthread_mutex_lock(ticklock);
      tickcounts[g_num_threads]++;
      pthread_mutex_unlock(ticklock);
    }
    pthread_mutex_lock(foodlock);
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,0,0,wintotalfood);
    MPI_Win_fence(0,wintotalfood);
    MPI_Get(&total_food,1,MPI_DOUBLE,0,0,1,MPI_DOUBLE,wintotalfood);
    MPI_Win_unlock(0,wintotalfood);
    MPI_Win_fence(0,wintotalfood);
    pthread_mutex_unlock(foodlock);
    if (threadnum==g_num_threads)
      printf("Rank %d: tick %u complete. %lf food remaining.\n",mpi_myrank,finished/2,total_food);
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
  pthread_mutex_t * winfoodlock = p->winfoodlock;
  pthread_mutex_t * winphlock = p->winphlock;
  pthread_mutex_t * winoclock = p->winoclock;
  int threadnum = p->threadnum;
  unsigned int i,j,k;
  unsigned int x,y;
  unsigned int nx, ny;
  double total_food;
  j = 0;
  double foodamountsremaining[myend-mystart];
  int pheremoneLevels[myend-mystart][3][3];
  unsigned int occupancies[myend-mystart];
  //loop through ants
  printf("Rank %d: Thread %d starting recs.\n",mpi_myrank,threadnum);
  for(i = mystart; i < myend; i++)
    {
      x = myAnts[i].x;
      y = myAnts[i].y;
      int ranktop,rankmid,rankbottom;
      ranktop = (y-1)/standard_array_size;
      rankmid = (y)/standard_array_size;
      rankbottom = (y+1)/standard_array_size;
      unsigned int xstart=x;
      int arrind = i-mystart;
      if (x>0)
        xstart--;
      unsigned int xend = x;
      if (x<g_array_size-1)
        xend++;
      unsigned int arraylen=xend-xstart+1;
      int arraystart=xstart-x+1;
      //printf("Rank %d: threadnum %d check ranks %d,%d,%d since y = %u and sas=%u\n",mpi_myrank,threadnum,ranktop,rankmid,rankbottom,y,standard_array_size);
      
      pthread_mutex_lock(winoclock);
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winoc);
      MPI_Win_fence(0,winoc);
      MPI_Get(&(occupancies[arrind]),1,MPI_INT,rankmid,y*g_array_size+x,1,MPI_INT,winoc);
      MPI_Win_unlock(rankmid,winoc);
      MPI_Win_fence(0,winoc);
      pthread_mutex_unlock(winoclock);
      
      pthread_mutex_lock(winfoodlock);
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winfood);
      MPI_Win_fence(0,winfood);
      MPI_Get(&(foodamountsremaining[arrind]),1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
      MPI_Win_unlock(rankmid,winfood);
      MPI_Win_fence(0,winfood);
      pthread_mutex_unlock(winfoodlock);
      
      pthread_mutex_lock(winphlock);
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winph);
      MPI_Win_fence(0,winph);
      MPI_Get(&(pheremoneLevels[arrind][1][arraystart]),arraylen,MPI_INT,rankmid,y*g_array_size+xstart,arraylen,MPI_INT,winph);
      MPI_Win_unlock(rankmid,winph);
      MPI_Win_fence(0,winph);
      pthread_mutex_unlock(winphlock);

      if (y>0)
      {      
        pthread_mutex_lock(winphlock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,ranktop,0,winph);
        MPI_Win_fence(0,winph);
        MPI_Get(&(pheremoneLevels[arrind][0][arraystart]),arraylen,MPI_INT,ranktop,(y-1)*g_array_size+xstart,arraylen,MPI_INT,winph);
        MPI_Win_unlock(ranktop,winph);
        MPI_Win_fence(0,winph);
        pthread_mutex_unlock(winphlock);
      }
      
      if (y<g_array_size-1)
      { 
        pthread_mutex_lock(winphlock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankbottom,0,winph);
        MPI_Win_fence(0,winph);
        MPI_Get(&(pheremoneLevels[arrind][2][arraystart]),arraylen,MPI_INT,rankbottom,(y+1)*g_array_size+xstart,arraylen,MPI_INT,winph);
        MPI_Win_unlock(rankbottom,winph);
        MPI_Win_fence(0,winph);
        pthread_mutex_unlock(winphlock);
      }
      //if (foodamountsremaining[arrind] != 0)
      //  printf("Rank %d:Thread %d ant %d on (%u,%u) found %lf food among %d ants\n",mpi_myrank,threadnum, i, x, y, foodamountsremaining[arrind],occupancies[arrind]);
    }
  printf("Rank %d: Thread %d finished recs.\n",mpi_myrank,threadnum);
    
  unsigned int finished;
  if (threadnum!= g_num_threads)
  {
    pthread_mutex_lock(ticklock);
    unsigned int myprogress = ++tickcounts[threadnum];
    unsigned int mainprogress = tickcounts[g_num_threads];
    pthread_mutex_unlock(ticklock); 
    printf("Rank %d: Thread %d waiting on main in tick.\n",mpi_myrank, threadnum);
    while (myprogress!=mainprogress)
    {
      pthread_mutex_lock(ticklock);
      mainprogress = tickcounts[g_num_threads];
      pthread_mutex_unlock(ticklock); 
    }
    printf("Rank %d: Thread %d sees that main is ready in tick.\n",mpi_myrank, threadnum);
  }
  else
  {
    int t;
    pthread_mutex_lock(ticklock);
    finished = tickcounts[g_num_threads]+1;
    pthread_mutex_unlock(ticklock); 
    for (t = 0; t < g_num_threads;t++)
    {
      pthread_mutex_lock(ticklock);
      unsigned int threadprogress = tickcounts[t];
      pthread_mutex_unlock(ticklock); 
      printf("Rank %d: main waiting on thread %d in tick.\n",mpi_myrank,t);
      while(threadprogress != finished)
      {
        pthread_mutex_lock(ticklock);
        threadprogress = tickcounts[t];
        pthread_mutex_unlock(ticklock);
      }
      printf("Rank %d: main sees thread %d ready in tick.\n",mpi_myrank,t);
    }
    MPI_Barrier( MPI_COMM_WORLD );
    pthread_mutex_lock(ticklock);
    tickcounts[g_num_threads]++;
    pthread_mutex_unlock(ticklock);
  }
  for(i = mystart; i < myend; i++)
    {
      x = myAnts[i].x;
      y = myAnts[i].y;
      int rankmid;
      rankmid = (y)/standard_array_size;
      unsigned int xstart=x;
      int arrind = i-mystart;
      if (x>0)
        xstart--;
      unsigned int xend = x;
      if (x<g_array_size-1)
        xend++;
      unsigned int arraylen=xend-xstart+1;
      int arraystart=xstart-x+1;
      
      nx=x;
      ny=y;
      int maxph=pheremoneLevels[arrind][1][1];
      for (j=0;j<3;j++)
      {
        if ((j==0 && y > 0) || (j==2 && y < g_array_size-1))
        {
          for (k=arraystart;k<arraystart+arraylen;k++)
          {
            int phlevel = pheremoneLevels[arrind][j][k];
            if (phlevel>maxph)
            {
              maxph=phlevel;
              nx=x+k-1;
              ny=y+j-1;
            }  
          }
        }
      }
    
      if (foodamountsremaining[arrind] > 0 && foodamountsremaining[arrind] < occupancies[arrind])
      {
        double foodtoeat=foodamountsremaining[arrind]/occupancies[arrind];
        pthread_mutex_lock(foodlock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,0,0,wintotalfood);
        MPI_Win_fence(0,wintotalfood);
        MPI_Get(&total_food,1,MPI_DOUBLE,0,0,1,MPI_DOUBLE,wintotalfood);
        total_food-=foodtoeat;
        MPI_Put(&total_food,1,MPI_DOUBLE,0,0,1,MPI_DOUBLE,wintotalfood);
        MPI_Win_unlock(0,wintotalfood);
        MPI_Win_fence(0,wintotalfood);
        pthread_mutex_unlock(foodlock);
        printf("Rank %d:Thread %d ant %d on (%u,%u) shared %lf food among %d ants (ate %lf)\n",mpi_myrank,threadnum, i, x, y, foodamountsremaining[arrind],occupancies[arrind],foodtoeat);
        foodamountsremaining[arrind]=0;
        myAnts[i].foodEaten+=foodtoeat;
        pthread_mutex_lock(winfoodlock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winfood);
        MPI_Win_fence(0,winfood);
        MPI_Put(&(foodamountsremaining[arrind]),1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
        MPI_Win_unlock(rankmid,winfood);
        MPI_Win_fence(0,winfood);
        pthread_mutex_unlock(winfoodlock);
        if (myAnts[i].alreadysprayed==0)
        {
          spray(x,y,winphlock);
          myAnts[i].alreadysprayed=1;
        }
      }  
      else if (foodamountsremaining[arrind] > 0)
      {
        myAnts[i].foodEaten++;
        printf("Rank %d:Thread %d ant %d on (%u,%u) (out of %d on the space) ate 1 food\n",mpi_myrank,threadnum, i, x, y, occupancies[arrind]);
        pthread_mutex_lock(foodlock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,0,0,wintotalfood);
        MPI_Win_fence(0,wintotalfood);
        MPI_Get(&total_food,1,MPI_DOUBLE,0,0,1,MPI_DOUBLE,wintotalfood);
        total_food-=1;
        MPI_Put(&total_food,1,MPI_DOUBLE,0,0,1,MPI_DOUBLE,wintotalfood);
        MPI_Win_unlock(0,wintotalfood);
        MPI_Win_fence(0,wintotalfood);
        pthread_mutex_unlock(foodlock);
        pthread_mutex_lock(winfoodlock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rankmid,0,winfood);
        MPI_Win_fence(0,winfood);
        MPI_Get(&(foodamountsremaining[arrind]),1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
        foodamountsremaining[arrind]--;
        MPI_Put(&(foodamountsremaining[arrind]),1,MPI_DOUBLE,rankmid,y*g_array_size+x,1,MPI_DOUBLE,winfood);
        MPI_Win_unlock(rankmid,winfood);
        MPI_Win_fence(0,winfood);
        pthread_mutex_unlock(winfoodlock);
        if (myAnts[i].alreadysprayed==0)
        {
          spray(x,y,winphlock);
          myAnts[i].alreadysprayed=1;
        }
      }
      else
      {
        int rand=0;
        while (x==nx && y==ny)
        {
          rand=1;
          unsigned int dx = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;
          unsigned int dy = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;
          while((dx==0 && !(x >0)) || (dx==2 && !(x<g_array_size-1)))
            dx = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;
          while((dy==0 && !(y >0)) || (dy==2 && !(y<g_array_size-1)))
            dy = ((unsigned int)(100*GenVal(mpi_myrank%Maxgen)))%3;          
          nx=x+dx-1;
          ny=y+dy-1;
        }
        if (myAnts[i].alreadysprayed == 1)
        {
          undo_spray(x,y,winphlock);
          myAnts[i].alreadysprayed=0;
        }
        printf("Rank %d:Thread %d moving ant %d from (%u,%u) to (%u,%u),rand=%d,ph=%d\n",mpi_myrank,threadnum, i, x, y, nx, ny,rand,pheremoneLevels[arrind][ny-y+1][nx-x+1]);
        myAnts[i].x=nx;
        myAnts[i].y=ny;
        int rank = y/standard_array_size;
        int currentoccupancy;
        pthread_mutex_lock(winoclock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winoc);
        MPI_Win_fence(0,winoc);
        MPI_Get(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
        currentoccupancy--;
        MPI_Put(&currentoccupancy,1,MPI_INT,rank,y*g_array_size+x,1,MPI_INT,winoc);
        MPI_Win_unlock(rank,winoc);
        MPI_Win_fence(0,winoc);
        pthread_mutex_unlock(winoclock);
        rank=ny/standard_array_size;
        pthread_mutex_lock(winoclock);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winoc);
        MPI_Win_fence(0,winoc);
        MPI_Get(&currentoccupancy,1,MPI_INT,rank,ny*g_array_size+nx,1,MPI_INT,winoc);
        currentoccupancy++;
        MPI_Put(&currentoccupancy,1,MPI_INT,rank,ny*g_array_size+nx,1,MPI_INT,winoc);
        MPI_Win_unlock(rank,winoc);
        MPI_Win_fence(0,winoc);
        pthread_mutex_unlock(winoclock);
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
void spray(int x, int y, pthread_mutex_t * winphlock)
{
  int arraydimension=19;
  int pheremoneLevel[arraydimension];
  unsigned int xstart=x;
  if (xstart < (arraydimension-1)/2)
    xstart=0;
  else
    xstart-=(arraydimension-1)/2;
  unsigned int xend = x;
  if (xend<g_array_size-(arraydimension-1)/2-1)
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
  if (yend<g_array_size-(arraydimension-1)/2-1)
    yend+=(arraydimension-1)/2;
  else
    yend=g_array_size-1;
  int i,j;
  
  for (i=ystart;i<yend;i++)
  {
    int rank = i/standard_array_size;
    //printf("Rank %d: spraying (%u to %u,%u) in rank %d\n",mpi_myrank,xstart,xend,i,rank);
    pthread_mutex_lock(winphlock);
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winph);
    MPI_Win_fence(0,winph);
    MPI_Get(&(pheremoneLevel[arraystart]),arraylen,MPI_INT,rank,i*g_array_size+xstart,arraylen,MPI_INT,winph);
    for (j=arraystart; j < arraylen+arraystart;j++)
    {
      int spraylevel = 10;
      int distfrommid=i-(arraydimension-1)/2;
      if (distfrommid<0)
        distfrommid*=-1;
      spraylevel*=(j-(arraydimension-1)/2);
      if (spraylevel < 0)
        spraylevel*=-1;
      spraylevel=100-spraylevel-10*distfrommid;
      if (spraylevel < 0)
        spraylevel=0;
      pheremoneLevel[j]+=spraylevel;
    }
    MPI_Put(&(pheremoneLevel[arraystart]),arraylen,MPI_INT,rank,i*g_array_size+xstart,arraylen,MPI_INT,winph);
    MPI_Win_unlock(rank,winph);
    MPI_Win_fence(0,winph);
    pthread_mutex_unlock(winphlock);
  }
}


/***************************************************************************/
/* Function: undo_spray ****************************************************/
/***************************************************************************/

//removes pheremones on adjacent Cells
void undo_spray(int x, int y, pthread_mutex_t * winphlock)
{
  int arraydimension=19;
  int pheremoneLevel[arraydimension];
  unsigned int xstart=x;
  if (xstart < (arraydimension-1)/2)
    xstart=0;
  else
    xstart-=(arraydimension-1)/2;
  unsigned int xend = x;
  if (xend<g_array_size-(arraydimension-1)/2-1)
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
  if (yend<g_array_size-(arraydimension-1)/2-1)
    yend+=(arraydimension-1)/2;
  else
    yend=g_array_size-1;
  int i,j;
  
  for (i=ystart;i<yend;i++)
  {
    int rank = i/standard_array_size;
    printf("Rank %d: de-spraying (%u to %u,%u) in rank %d\n",mpi_myrank,xstart,xend,i,rank);
    pthread_mutex_lock(winphlock);
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,rank,0,winph);
    MPI_Win_fence(0,winph);
    MPI_Get(&(pheremoneLevel[arraystart]),arraylen,MPI_INT,rank,i*g_array_size+xstart,arraylen,MPI_INT,winph);
    for (j=arraystart; j < arraylen+arraystart;j++)
    {
      int spraylevel = 10;
      int distfrommid=i-(arraydimension-1)/2;
      if (distfrommid<0)
        distfrommid*=-1;
      spraylevel*=(j-(arraydimension-1)/2);
      if (spraylevel < 0)
        spraylevel*=-1;
      spraylevel=100-spraylevel-10*distfrommid;
      if (spraylevel < 0)
        spraylevel=0;
      pheremoneLevel[j]-=spraylevel;
    }
    MPI_Put(&(pheremoneLevel[arraystart]),arraylen,MPI_INT,rank,i*g_array_size+xstart,arraylen,MPI_INT,winph);
    MPI_Win_unlock(rank,winph);
    MPI_Win_fence(0,winph);
    pthread_mutex_unlock(winphlock);
  }
}
