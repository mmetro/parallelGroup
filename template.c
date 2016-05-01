

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

#ifndef __TYPEDEF_H
#define __TYPEDEF_H

Cell ** g_worldGrid;

typedef struct Cell {
  double foodRemaining;
  int pheremoneLevel;
  int occupancy;
} Cell;

typedef struct Ant {
  double foodEaten;
  int x, y;
} Ant;

#endif

/***************************************************************************/
/* Global Vars *************************************************************/
/***************************************************************************/

unsigned int g_array_size = 0;
int g_num_threads = 0;
int g_num_ants = 0;
int g_total_food = 0;

int mpi_myrank = -1;
int mpi_commsize = -1;

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
void run_farm();

//run one iteration
void run_tick();
void exchange_cells();

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

  if (mpi_myrank == 0) {
    start_time = get_Time();
  }

// Init 16,384 RNG streams - each row has an independent stream
  InitDefault();

  mpi_log_debug("Rank %d of %d started.\n", mpi_myrank, mpi_commsize);

  // Read in and process the remaining arguments.
  process_arguments(argc, argv);

  // Allocate the chunk of the universe that we need for this rank, and
  // randomly initialize the values in that chunk using the proper RNG stream
  // for that chunk.
  allocate_and_init_array();

  mpi_log_debug("Initialization complete. Ant farm started...\n\n");

  MPI_Barrier( MPI_COMM_WORLD );

  //Run simulation 
  run_farm();  
  
  // // Barrier after completion
  MPI_Barrier( MPI_COMM_WORLD );

  if (mpi_myrank == 0) {
    mid_cycle_time = get_cycles();

    compute_time = (mid_cycle_time - start_cycle_time) / clockrate;
    printf("Simulation duration:\t%f seconds\n", compute_time);
  }



  if (mpi_myrank == 0) {
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
  // Argument 1 is the number of threads to use
  // Argument 2 is the size of matrix
  // Argument 3 is the number of ants
  // Argument 4 is the amount of food in the system  

  // Check for 5 arguments
  if (argc != 5) {
    mpi_log_error("Usage: \n%s <num of threads> <matrix size> <num of ants> <num of food>\n", argv[0]);
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
    mpi_log_debug("Read in number of ants: %s\n", argv[3]);
    food = atoi(argv[4]);

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

    // check if there are more threads than rows per rank! must reduce if so!
    if (threads > size / mpi_commsize) {
        mpi_log_debug("Detected 'threads = %d' greater than rows per thread in each rank\n", threads);
        threads = size / mpi_commsize;
        mpi_log_debug("Reducing number of threads per rank to %d\n", threads);
    }

    if (mpi_commsize > size) {
      mpi_log_error("ERROR: Cannot have more ranks than rows in the matrix. Please try again.\n");
      exit(1);
    }

    // Time to set the globals.
    g_array_size = size;
    g_num_threads = threads;
    g_num_ants = ants;
    g_total_food = food;
  }
}


/***************************************************************************/
/* Function: allocate_and_init_array ***************************************/
/***************************************************************************/

void allocate_and_init_array()
{
  unsigned int row = 0;
  unsigned int col = 0;
  unsigned int foodheap = g_total_food/mpi_commsize;
  //Cell ** g_worldGrid;

}


/***************************************************************************/
/* Function: run_farm ******************************************************/
/***************************************************************************/

void run_farm() {
  while (g_total_food > 0) 
  {
    // run ant decisions
    run_tick();

    MPI_Barrier( MPI_COMM_WORLD );

    
    exchange_cells();

    MPI_Barrier( MPI_COMM_WORLD );
  }
}

/***************************************************************************/
/* Function: run_tick ******************************************************/
/***************************************************************************/
// run ant decisions
void run_tick() {
 
}


/***************************************************************************/
/* Function: exchange_cells ************************************************/
/***************************************************************************/
// exchange ants and pheromones
void exchange_cells() {
 
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

