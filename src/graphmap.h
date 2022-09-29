#ifndef GRAPHMAP
#define GRAPHMAP

#include <stddef.h>
#include <stdlib.h>


typedef struct graph_node
{
    struct graph_node *right;
    struct graph_node *left;
    char cell_type; // ' ' or '1' or '2' or '*'
    int distance;   // the estimated distance from the cell to the exit
    int cost;   // the number of steps from the entrance to the cell
    int priority;  // distance + cost combined
    int position;   // position number within the map represented as linked lost
    int in_pq;    // if 1, the node is in the priority queue; if 0, it needs to be added to the queue
    struct graph_node *prior_step;
} node;

typedef struct linked_list
{
    node *head;
    node *tail;
} llist;

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

typedef struct {
  int exit_x;
  int exit_y;
} exit_data;


extern int process_path(llist *linked_list, exit_data *ex_data, map_t *map);

#endif