#ifndef MAIN
#define MAIN
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct {
  int rows;
  int cols;
  char boundary;
  char open;
  char path;
  char e;
  char ex;
  char** maze;
} map_t;

typedef struct
{
    int n_exit;
    int n_entrance;
    int n_square;
} is_valid;

extern map_t import_map(char* maze, is_valid *map_validation);
extern void print_map(map_t* map);
extern int solve_map(map_t *map);

#endif