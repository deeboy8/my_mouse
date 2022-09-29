#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>

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

char* my_readline(int fd);
extern map_t import_map(char* maze, is_valid *map_validation);
void parse_string(char* string, map_t* map);
void initialize_maze(map_t* map);
void print_map(map_t* map);

extern char* my_readline(int fd);

#endif 