#ifndef __TYPEDEF_H
#define __TYPEDEF_H

Cell ** g_worldGrid;

typedef struct {
  double foodRemaining;
  int pheremoneLevel;
  int occupancy;
} Cell;

typedef struct {
  double foodEaten;
  int x, y;
} Ant;

#endif