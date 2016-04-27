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