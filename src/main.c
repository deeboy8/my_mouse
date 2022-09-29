#include "main.h"

void free_maze(map_t* map)
{
    for (int i = 0; i < map->rows; i++) {
        free(map->maze[i]);
    }
    free(map->maze);
}

int is_map_valid(is_valid *map_validation)
{
    if(map_validation->n_exit != 1 || map_validation->n_entrance != 1 || map_validation->n_square > 1000 )
    {
        return FALSE;
    } else
    {
        return TRUE;
    }
}

int main (int argc, char* argv[]) {
    if (argc == 2) {
        is_valid map_validation = {0};
        // import map and check if it is a valid map
        map_t map = import_map(argv[1], &map_validation);
        // print map 
        //print_map(&map);
        if(is_map_valid(&map_validation))
        {
            solve_map(&map);
        } else
        {
            printf("MAP ERROR\n");
        }
        free_maze(&map);
    }
    return 0;
}