#include "process_map.h"

void print_map(map_t* map) {
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            printf("%c", map->maze[i][j]);
        }
        printf("\n");
    }
}

void initialize_maze(map_t* map) { ///// return a pointer or leave as void 
      // create rows of type char*
    map->maze = malloc(sizeof(char*) * map->rows);
    assert(map->maze != NULL);

    // create n number of char pointers per row
    for (int i = 0; i < map->rows; i++) {
        map->maze[i] = malloc(sizeof(char) * map->cols);
    }
} ////////////////// create free function

void parse_string(char* string, map_t* map) {
    sscanf(string, "%dx%d%c%c%c%c%c", &map->rows, &map->cols, &map->boundary, &map->open, &map->path, &map->e, &map->ex);
    
    /* printf("row = %d, col = %d, b = %c, o = _%c_, p = _%c_, e = %c, ex = %c\n",
        map->rows, map->cols, map->boundary, map->open, map->path, map->e, map->ex);*/
}

map_t import_map(char* maze, is_valid *map_validation) {
    int fd = open(maze, O_RDONLY);
    assert(fd != -1);

    // initialize map_t struct to zero
    map_t map = {0};

    // read first line of file and set matrix parameters
    char* string = my_readline(fd);
    parse_string(string, &map);
    free(string);

    // allocate memory for "2D maze"
    initialize_maze(&map);

    string = my_readline(fd);
    int row = 0;
    // import maze from text file
    do {
        for (int column = 0; column < map.cols; column++) {
            map.maze[row][column] = string[column];  /// double p = single p aka 2d array to a single string
            if(string[column] == '1')
            {
                map_validation->n_entrance++;
            }
            if(string[column] == '2')
            {
                map_validation->n_exit++;
            }     
        }
        row++;
        free(string);
    } while ((string = my_readline(fd)) != NULL);
    free(string);
    map_validation->n_square = map.rows * map.cols;
    return map;
}