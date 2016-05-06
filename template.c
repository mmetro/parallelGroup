/***************************************************************************/
/* Template for Final Project **********************************************/
/* Allen Wu                   **********************************************/
/* Kainoa Eastlack            **********************************************/
/* Michael Metrocavich        **********************************************/
/* Nevin Jacob                **********************************************/
/***************************************************************************/


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
 #define __LOCAL__


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

// #define DEBUGGER

/***************************************************************************/
/* Defines *****************************************************************/
/***************************************************************************/

#define __MATRIX_SIZE__ 10


#define TRUE 1
#define FALSE  0

typedef struct Cell {
  double foodRemaining;
  int pheremoneLevel;
  int occupancy;
} Cell;

// Not totally sure on the details of this one
// Allen, could you comment this
typedef enum e_AntState
{
    NOTHING = 0, SEARCHING = 1, EATING = 5 

} AntState;

typedef struct Ant {
  double foodEaten;
  unsigned int x, y;
  AntState state;
} Ant;

// MOVE_TO: increments a cell's occupancy
// MOVE_FROM: decrements a cell's occupancy
// SPRAY_PHEREMONE: increment pheremone levels
// no eat action, because it's assumed ants will always eat
typedef enum e_ActionType
{
    MOVE_TO, MOVE_FROM, SPRAY_PHEREMONE, SPRAY_FOUND, SPRAY_NEG, EAT

} ActionType;


// The action struct to be sent with exchange_cells_post
// Specifies an action that will modify a cell in the world rank
typedef struct AntAction {
  // no constructor for structs.  Use calloc or malloc
  //AntAction(ActionType type, int ax, int ay):

  // no support for this in C, needs to be manually initialized
  //action(type), x(ax), y(ay){}; //init

  ActionType action;
  unsigned int x, y;
} AntAction;



/***************************************************************************/
/* Global Vars *************************************************************/
/***************************************************************************/

Cell ** g_worldGrid;

unsigned int g_array_size = 0;
int g_num_threads = 0;
int g_num_ants = 0;
int g_total_food = 0;

double g_food_thresh_hold = 0.0;

int mpi_myrank = -1;
int mpi_commsize = -1;

// each rank owns some ants
unsigned int myNumAnts;
Ant * myAnts;

unsigned int actionCount;
AntAction *actionQueue;
// error if we have more than this many actions queued
unsigned int actionCountMax;

#ifdef __LOCAL__
  double clockrate = 2.523e9;
#else
  double clockrate = 1.6e9;
#endif

time_t start_cycle_time;
time_t mid_cycle_time;

int g_tick_counter = 0;


/***************************************************************************/
/* Function Decs ***********************************************************/
/***************************************************************************/

// Function to process arguments outside of main to keep it readable
void process_arguments(int argc, char* argv[]);

//init
void allocate_and_init_array();

//simulation
void run_farm();

//run one iteration
void run_tick();
void exchange_cells_pre();
void exchange_cells_post();
void update_total_food();


// helper functions
double GenRowVal(unsigned int rowNumber);
double GenAntVal(unsigned int antNumber);
void spray(int x, int y, int high, int low, int type);
void eat(int x, int y);
void check_highest_level(unsigned int x,unsigned  int y,unsigned  int * nx,unsigned  int * ny);
void queue_action(ActionType action, unsigned int x, unsigned int y);

// IO
void print_world();

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

// Example MPI startup and using CLCG4 RNG
  MPI_Init( &argc, &argv);
  MPI_Comm_size( MPI_COMM_WORLD, &mpi_commsize);
  MPI_Comm_rank( MPI_COMM_WORLD, &mpi_myrank);

  
// Init 16,384 RNG streams - each row and each ant has an independent stream
  InitDefault();

  printf("Rank %d of %d started.\n", mpi_myrank, mpi_commsize);

  MPI_Barrier( MPI_COMM_WORLD );
  // Read in and process the remaining arguments.
  process_arguments(argc, argv);

  MPI_Barrier( MPI_COMM_WORLD );
  // Allocate the chunk of the universe that we need for this rank, and
  // randomly initialize the values in that chunk using the proper RNG stream
  // for that chunk.
  allocate_and_init_array();

  MPI_Barrier( MPI_COMM_WORLD );

  if (mpi_myrank == 0)  {
    print_world();
    printf("Initialization complete. Ant farm started...\n\n");
  };

  MPI_Barrier( MPI_COMM_WORLD );


  if (mpi_myrank == 0) {
    start_time = get_Time;
  }

  //Run simulation 
  run_farm();  
  
  // // Barrier after completion
  MPI_Barrier( MPI_COMM_WORLD );

  if (mpi_myrank == 0) {
    mid_cycle_time = get_Time;

    compute_time = (mid_cycle_time - start_cycle_time) / clockrate;
    printf("Simulation duration:\t%f seconds\n", compute_time);
  }



  if (mpi_myrank == 0) {
    printf(" Simulation Complete!");
    print_world();
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
  // Argument 1 is the number of threads to use
  // Argument 2 is the size of matrix
  // Argument 3 is the number of ants
  // Argument 4 is the amount of food in the system 
  // Argument 5 is the food respawn chance.  It can be used to make the simulation endless, but should be left at 0 for now 

  // Check for 5 arguments
  if (argc != 6) {
    printf("Usage: \n%s <num of threads> <matrix size> <num of ants> <num of food> <respawn chance>\n", argv[0]);
    exit(1);
  }
  else
  {
    // Next, read in the number of threads
    if (mpi_myrank == 0) printf("Read in number of threads: %s\n", argv[1]);
    threads = atoi(argv[1]);

    // Read in the matrix size
    if (mpi_myrank == 0) printf("Read in matrix size: %s\n", argv[2]);
    size = atoi(argv[2]);

    // Read in the number of ants
    if (mpi_myrank == 0) printf("Read in number of ants: %s\n", argv[3]);
    ants = atoi(argv[3]);

    // Read in the total amount of food
    if (mpi_myrank == 0) printf("Read in amount of food: %s\n", argv[4]);
    food = atoi(argv[4]);

    // Read in the food respawn chance
    if (mpi_myrank == 0) printf("Read in food respawn chance %s\n", argv[5]);
    food_respawn = ((double)atoi(argv[5])) / 100.0;

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

    // check if there are more threads than rows per rank! must reduce if so!
    if (threads > size / mpi_commsize) {
        printf("Detected 'threads = %d' greater than rows per thread in each rank\n", threads);
        threads = size / mpi_commsize;
        printf("Reducing number of threads per rank to %d\n", threads);
    }

    if (mpi_commsize > size) {
      printf("ERROR: Cannot have more ranks than rows in the matrix. Please try again.\n");
      exit(1);
    }

    // Time to set the globals.
    g_array_size = size;
    g_num_threads = threads;
    g_num_ants = ants;
    g_total_food = food;
    g_food_thresh_hold = food_respawn;

    // each rank owns some ants
    myNumAnts = g_num_ants / mpi_commsize;
    if (mpi_myrank == mpi_commsize - 1)
      myNumAnts += g_num_ants % mpi_commsize;
  }
}


/***************************************************************************/
/* Function: allocate_and_init_array ***************************************/
/***************************************************************************/

void allocate_and_init_array()
{
  unsigned int row = 0;
  unsigned int col = 0;
  unsigned int i;
  unsigned int foodheap = g_total_food;
  unsigned int alloc_food = 0;
  unsigned sum = 0;
  // At most we can have myNumAnts*2 actions per tick.
  // An ant moving will make two actions: a MOVE_TO and a MOVE_FROM
  actionCountMax = myNumAnts*2;
  actionQueue = calloc(actionCountMax, sizeof(AntAction));
  actionCount = 0;

  // each rank has a local copy of the world
  // ranks update their local copies, gasking only for the pieces of the world near their ants

  g_worldGrid = calloc(g_array_size, sizeof(Cell *));
  for (row = 0; row < g_array_size; row++)
  {
    g_worldGrid[row] = calloc(g_array_size, sizeof(Cell));
    for (col = 0; col < g_array_size; col++)
    {
      if (foodheap > 0)
      {
        if (GenVal(row*col) < 0.25)
        {
          alloc_food =  (unsigned int)(foodheap/g_array_size);
          if (foodheap > alloc_food)
          {
            g_worldGrid[row][col].foodRemaining = alloc_food;
            foodheap -= alloc_food;
            sum += alloc_food;
          }
          else
          {
            g_worldGrid[row][col].foodRemaining = foodheap;
            sum += foodheap;
            foodheap = 0;
          }
        }
      }

      if (row == g_array_size-1 && col == g_array_size-1 && foodheap > 0)
      {
        g_worldGrid[row][col].foodRemaining = foodheap;
        sum += foodheap; foodheap = 0;
      }
      
      g_worldGrid[row][col].pheremoneLevel = 0;
      g_worldGrid[row][col].occupancy = 0;
    }

  }
  // printf("SUM = %d \n", sum);
  // initialize the ants
  myAnts = calloc(myNumAnts, sizeof(Ant));
  for(i = 0; i < myNumAnts; i++)
  {
    myAnts[i].foodEaten = 0;
    myAnts[i].state = 0;
    myAnts[i].x = (unsigned int) (GenAntVal(i) * g_array_size);
    myAnts[i].y = (unsigned int) (GenAntVal(i) * g_array_size);
    queue_action(MOVE_TO, myAnts[i].x, myAnts[i].y);
  }
  printf("Rank %d init  %d ants.\n", mpi_myrank, myNumAnts);
  exchange_cells_post();
  // TODO: how do we distribute food?  
  // Should be parallel deterministic, and needs to create an exact amount of food
}


/***************************************************************************/
/* Function: run_farm ******************************************************/
/***************************************************************************/

void run_farm() {
  int ticks = 0;
  while (g_total_food > 1) 
  {
    //if (mpi_myrank == 0)  print_world();
    // update local copies of the world
    exchange_cells_pre();
    MPI_Barrier( MPI_COMM_WORLD );

    //if (mpi_myrank == 0)  print_world();
    // run ant decisions
    run_tick();
    MPI_Barrier( MPI_COMM_WORLD );
    // if (mpi_myrank == 0) { printf("RAN TICK\n");};
    //if (mpi_myrank == 0)  print_world();
    // send ant decisions to world rank
    exchange_cells_post();
    MPI_Barrier( MPI_COMM_WORLD );;
    #ifdef DEBUGGER
    if (mpi_myrank == 0)  print_world();
    #endif
    //every 5 ticks
    if (ticks % 5 == 0)
    {
      update_total_food();
    }
    MPI_Barrier( MPI_COMM_WORLD );
    // if (mpi_myrank == 0) { printf("RAN ITERATION\n");};
  }
  // printf("LEFTOVER FOOD = %d \n", g_total_food);
}

/***************************************************************************/
/* Function: run_tick ******************************************************/
/***************************************************************************/
// run ant decisions
void run_tick() {
  unsigned int i;
  unsigned int x,y;
  unsigned int nx, ny;
  unsigned int sx, sy;
  AntAction aa;
  // printf("%d running \n", i);
  //loop through ants
  for(i = 0; i < myNumAnts; i++)
    {
      x = myAnts[i].x;
      y = myAnts[i].y;
      //printf("myrank = %d, i = %d, x = %d, y = %d\n", mpi_myrank, i, x,y);
    //if exists pheremone on current cell
      if (g_worldGrid[y][x].pheremoneLevel > 0)
      {
        //if exists food 
        if (g_worldGrid[y][x].foodRemaining > 0)
        {
          myAnts[i].state = EATING;
          //EAT FOOD
          // printf("%d is EATING \n", i);
          queue_action(EAT, x, y);
        
          //if enough food
          if (g_worldGrid[y][x].foodRemaining > g_worldGrid[y][x].occupancy)
          {
            //ant.food + 1
            myAnts[i].foodEaten++;
          }
          else
          {
            double split = g_worldGrid[y][x].foodRemaining/g_worldGrid[y][x].occupancy;
            //ant.food + split
            myAnts[i].foodEaten+= split;
            //SPRAY -1
            queue_action(SPRAY_NEG, x, y);
            myAnts[i].state = SEARCHING;
          }
        }
        //else
        else 
        {
          //check highest level
          check_highest_level(x,y,&nx,&ny); //pass nx, ny by reference
          //if this is highest
          if (x==nx && y==ny)
          {  
            queue_action(SPRAY_NEG, x, y);
            myAnts[i].state = SEARCHING;
          }
          else if (myAnts[i].state == NOTHING)
          {
            myAnts[i].state = SEARCHING;
            queue_action(SPRAY_FOUND, x, y);
          }
          else if (myAnts[i].state == SEARCHING)
          {
            myAnts[i].x = nx;
            myAnts[i].y = ny;
            queue_action(MOVE_TO, nx, ny);
            queue_action(MOVE_FROM, x, y);
          }
        }
      }
      else
      {
        //if exists food 
        if (g_worldGrid[y][x].foodRemaining > 0)
        {
          myAnts[i].state = EATING;
          myAnts[i].foodEaten++;
          //EAT FOOD
          // printf("%d is EATING \n", i);
          queue_action(EAT, x, y);
          queue_action(SPRAY_PHEREMONE, x, y);
        }
        else
        {
          //MOVE random
          myAnts[i].state = NOTHING;
          if ((GenAntVal(i * mpi_myrank) > .5))
            { sx = 1; } 
          else { sx = 0;}
          if ((GenAntVal(i * mpi_myrank) > .5)) 
          { sy = 1; } 
          else { sy = 0;}
          // printf("NX = %d \n",nx);
          // printf("NY = %d \n",ny);
          // myAnts[i].x = (myAnts[i].x+nx)%g_array_size;
          // myAnts[i].y = (myAnts[i].y+ny)%g_array_size;
          // printf("THIS HSIT %u \n %\n", (myAnts[i].y+ny)%g_array_size, (myAnts[i].x+nx)%g_array_size);
          
          if ((GenAntVal(i * mpi_myrank) > .5))
            { myAnts[i].x = (myAnts[i].x+sx)%g_array_size; }
          else 
            { myAnts[i].x = (myAnts[i].x-sx)%g_array_size;}
          
          if ((GenAntVal(i * mpi_myrank) > .5))
            { myAnts[i].y = (myAnts[i].y+sy)%g_array_size; }
          else 
            { myAnts[i].y = (myAnts[i].y-sy)%g_array_size;}
 
          queue_action(MOVE_TO, myAnts[i].x,myAnts[i].y);
          queue_action(MOVE_FROM, x,y);
        }
      }    
    }
    g_tick_counter++;
}


/***************************************************************************/
/* Function: exchange_cells_pre ********************************************/
/***************************************************************************/
// Ask world rank for rows
// update only the rows nearby our ants
void exchange_cells_pre() {
  unsigned int i,j;
  // The number of rows it needs from a rank
  unsigned int numRowsNeeded = 0;
  // Specifies which rows in the local copy it wants to update
  unsigned int * rowsNeeded = calloc(g_array_size, sizeof(unsigned int *));
  // Array of y values, specifying the requested rows.  Will only be numRowsNeeded in length
  unsigned int * rankMessageArray = calloc(g_array_size, sizeof(unsigned int ));

  if(mpi_myrank != 0)
  {
    // determine which rows we will request
    for(i = 0; i < myNumAnts; i++)
    {
      for(j = (myAnts[i].y - 1)% g_array_size; j != (myAnts[i].y + 2)% g_array_size; j = (j+1) % g_array_size)
      {
        if(rowsNeeded[j % g_array_size] == FALSE)
        {
          rankMessageArray[numRowsNeeded] = j % g_array_size;
          numRowsNeeded++;
          rowsNeeded[j  % g_array_size] = TRUE;
        }
      }
    }

    MPI_Status status;
    MPI_Request sendRequest1, recvRequest1;
    // tell world rank how many rows we are requesting, and the row numbers
    MPI_Isend(&numRowsNeeded, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, &sendRequest1);
    MPI_Isend(rankMessageArray, numRowsNeeded, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, &sendRequest1);

    // receive the rows
    MPI_Wait(&sendRequest1, &status);
    for(j = 0; j < numRowsNeeded; j++)
    {
      MPI_Recv(g_worldGrid[rankMessageArray[j]], g_array_size * (sizeof(Cell)/sizeof(char)), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }
  }
  else
  {
    // send the world rank's rows to the others
    MPI_Status status;
    for(i = 1; i < mpi_commsize; i++)
    {
      // determine what rows are requested
      MPI_Recv(&numRowsNeeded, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(rankMessageArray, numRowsNeeded, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &status);
      // send the requested rows
      for(j = 0; j < numRowsNeeded; j++)
      {
        //printf("i = %u, numRowsNeeded = %u, j = %u, rankMessageArray[j] = %u\n", i, numRowsNeeded, j, rankMessageArray[j]);
        MPI_Send(g_worldGrid[rankMessageArray[j]], g_array_size * (sizeof(Cell)/sizeof(char)), MPI_CHAR, i, 0, MPI_COMM_WORLD);
      }
    }
  }
  free(rowsNeeded);
  free(rankMessageArray);
}

/***************************************************************************/
/* Function: exchange_cells_post *******************************************/
/***************************************************************************/
// Send ant actions to world
// Will use AntAction
void exchange_cells_post() {
    unsigned int i, j;
    MPI_Status status;
    MPI_Request sendRequest1;
    // tell world rank how many actions we are sending, and then send the actions
    MPI_Isend(&actionCount, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, &sendRequest1);
    MPI_Isend(actionQueue, actionCount * (sizeof(AntAction)/sizeof(char)), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &sendRequest1);
    //printf("Sent %u actions to rank 0\n", actionCount);
    if(mpi_myrank == 0)
    {
      // receive actions from all ranks
      for(i = 0; i < mpi_commsize; i++)
      {
        // receive the number of actions in the queue
        unsigned int receive_actionCount;
        MPI_Recv(&receive_actionCount, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &status);

        //receive the action queue
        AntAction *receive_actionQueue = calloc(receive_actionCount, sizeof(AntAction));
        MPI_Recv(receive_actionQueue, receive_actionCount * (sizeof(AntAction)/sizeof(char)), MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);

        // apply the effect of each action to the world
        for(j = 0; j < receive_actionCount; j++)
        {
          unsigned int x = receive_actionQueue[j].x;
          unsigned int y = receive_actionQueue[j].y;
      

          switch(receive_actionQueue[j].action) {
            case MOVE_TO:
              g_worldGrid[y][x].occupancy++;
              break;
            case MOVE_FROM:
              g_worldGrid[y][x].occupancy--;
              break;
            case SPRAY_PHEREMONE:
              spray(x,y,5,2,1);     //edit the spray variables for test data
              break;
            case SPRAY_FOUND:
              spray(x,y,1,1,1);
              break;
            case SPRAY_NEG:
              spray(x,y,1,1,-1);  //  1,0 vs  1,1 
              break;
            case EAT:
              eat(x,y);
              break;
            default:
              break;
          }
        }


        free(receive_actionQueue);
      }
    }


   actionCount = 0;
   //printf("Exiting exchange_cells_post\n");
}

// generate a random value for the rank's nth row
// each row has its own random stream
double GenRowVal(unsigned int rowNumber)
{
  return GenVal(mpi_myrank * (g_array_size / mpi_commsize) + rowNumber);
}

// generate a random value for the rank's nth ant
// each ant has its own random stream
double GenAntVal(unsigned int antNumber)
{
  // The first g_array_size values are reserved for each row's stream
  return GenVal(g_array_size + mpi_myrank * (g_num_ants / mpi_commsize) + antNumber);
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
// +1 for food, -1 for no food


void spray(int x, int y, int high, int low, int type)
{
  if (g_worldGrid[y][x].pheremoneLevel != 0 || type == 1)
  {
    g_worldGrid[y][x].pheremoneLevel += (high*type);  
  } 

  if (low != 0)
  {
    g_worldGrid[y][(x+1)%g_array_size].pheremoneLevel += (low*type);
    g_worldGrid[y][(x-1)%g_array_size].pheremoneLevel += (low*type);
    g_worldGrid[(y-1)%g_array_size][x].pheremoneLevel += (low*type);
    g_worldGrid[(y+1)%g_array_size][x].pheremoneLevel += (low*type);

    g_worldGrid[(y-1)%g_array_size][(x+1)%g_array_size].pheremoneLevel += (low*type);
    g_worldGrid[(y-1)%g_array_size][(x-1)%g_array_size].pheremoneLevel += (low*type);
    g_worldGrid[(y+1)%g_array_size][(x-1)%g_array_size].pheremoneLevel += (low*type);
    g_worldGrid[(y+1)%g_array_size][(x+1)%g_array_size].pheremoneLevel += (low*type);
  }
}


/***************************************************************************/
/* Function: eat ***********************************************************/
/***************************************************************************/

//consumes food in current cell
void eat(int x, int y)
{
  if (g_worldGrid[y][x].foodRemaining > 0)
  {
    int split = g_worldGrid[y][x].occupancy;
    if (g_worldGrid[y][x].foodRemaining < split)
      { g_total_food -= g_worldGrid[y][x].foodRemaining; 
        // printf("REMAIN = %d\n", g_worldGrid[y][x].foodRemaining );
        g_worldGrid[y][x].foodRemaining = 0;}
    else
      { g_worldGrid[y][x].foodRemaining--;
        g_total_food--;}
    // printf("TOTAL = %d\n ", g_total_food);
  }
}

/***************************************************************************/
/* Function: update_total_food *********************************************/
/***************************************************************************/
//world rank sends remaining total to all ranks 
void update_total_food()
{
  int i;
  int temp_total_food;
  if (mpi_myrank == 0)
  {
    MPI_Request sendRequest1;
    for(i = 0; i < mpi_commsize; i++)
    {
      MPI_Isend(&g_total_food, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &sendRequest1);
    }
    #ifdef DEBUGGER
    printf("%d food left\n", g_total_food);
    for(i=0; i < 10; i++)
    {
      printf("Ant %d ate %f. \n", i, myAnts[i].foodEaten);
    }
    #endif
  }

  MPI_Status status;
  MPI_Recv(&temp_total_food, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, &status);
  g_total_food = temp_total_food;
}

/***************************************************************************/
/* Function: check_highest_level********************************************/
/***************************************************************************/
//finds highest pheremone level around x,y, or just x,y if already highest
void check_highest_level(unsigned int x,unsigned int y,unsigned int * nx,unsigned int * ny)
{
  *nx = x;
  *ny = y;
  int max_level = g_worldGrid[y][x].pheremoneLevel;

  if (g_worldGrid[y][(x+1)%g_array_size].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[y][(x+1)%g_array_size].pheremoneLevel; *nx = y; *nx = (x+1)%g_array_size;}
  if (g_worldGrid[y][(x-1)%g_array_size].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[y][(x-1)%g_array_size].pheremoneLevel; *nx = y; *nx = (x-1)%g_array_size;}
  if (g_worldGrid[(y-1)%g_array_size][x].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[(y-1)%g_array_size][x].pheremoneLevel; *nx = (y-1)%g_array_size; *nx = x;}
  if (g_worldGrid[(y+1)%g_array_size][x].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[(y+1)%g_array_size][x].pheremoneLevel; *nx = (y+1)%g_array_size; *nx = x;}

  if (g_worldGrid[(y-1)%g_array_size][(x+1)%g_array_size].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[(y-1)%g_array_size][(x+1)%g_array_size].pheremoneLevel; *nx = (y-1)%g_array_size; *nx = (x+1)%g_array_size;} 
  if (g_worldGrid[(y-1)%g_array_size][(x-1)%g_array_size].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[(y-1)%g_array_size][(x-1)%g_array_size].pheremoneLevel ; *nx = (y-1)%g_array_size; *nx = (x-1)%g_array_size;} 
  if (g_worldGrid[(y+1)%g_array_size][(x-1)%g_array_size].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[(y+1)%g_array_size][(x-1)%g_array_size].pheremoneLevel; *nx = (y+1)%g_array_size; *nx = (x-1)%g_array_size;} 
  if (g_worldGrid[(y+1)%g_array_size][(x+1)%g_array_size].pheremoneLevel > max_level) 
    { max_level = g_worldGrid[(y+1)%g_array_size][(x+1)%g_array_size].pheremoneLevel; *nx = (y+1)%g_array_size; *nx = (x+1)%g_array_size;} 

}


/***************************************************************************/
/* Function: print_world****************************************************/
/***************************************************************************/
//print out an ascii representation of the world
void print_world()
{
  unsigned int i,j;
  printf("--------\n");
  for(j = 0; j < g_array_size; j++)
  {
    for(i = 0; i < g_array_size; i++)
    {
      // printf("Num %d %d", i,);
      if(g_worldGrid[j][i].occupancy > 0)
      {
        printf("%2u", g_worldGrid[j][i].occupancy);
      }
      else
      {
        if (g_worldGrid[j][i].foodRemaining > 0){
          // printf("%.1f", g_worldGrid[j][i].foodRemaining);
          printf("++");  
        }
        else
          printf("__");
      }
    }
    printf("\n");
  }

}


/***************************************************************************/
/* Function: queue_action **************************************************/
/***************************************************************************/
//queues an action 
void queue_action(ActionType action, unsigned int x, unsigned int y)
{
  if (x > 1000 || y > 10000)
  {
    printf("ITS GOING CRAZZZZYYY.\n");
  }
  if(actionCount > actionCountMax)
  {
    printf("ERROR: Action queue is full.\n");
  }
  else
  {
    actionQueue[actionCount].action = action;
    actionQueue[actionCount].x = x;
    actionQueue[actionCount].y = y;
    actionCount++;
  }
}
