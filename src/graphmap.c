#include "graphmap.h"
#include <stdio.h>

node *init_node(char *map_cell)
{
    node *new_node = malloc(sizeof(node));
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->cell_type = map_cell[0];
    new_node->distance = -1;
    new_node->cost = -1;
    new_node->priority = -1;
    new_node->position = 0;
    new_node->in_pq = 0;
    new_node->prior_step = NULL;

    return new_node;
}

llist *init_list(void)
{
    llist *new_list = malloc(sizeof(llist));
    new_list->head = NULL;
    new_list->tail = NULL;

    return new_list;
}

int add_link(llist *linked_list, char *map_cell)
{
    if(linked_list->head == NULL)
    {
        node *new_node = init_node(map_cell);
        new_node->position = 1;
        linked_list->head = new_node;
        linked_list->tail = new_node;
    } else
    {
        node *new_node = init_node(map_cell);
        new_node->position = linked_list->tail->position + 1;
        linked_list->tail->right = new_node;
        new_node->left = linked_list->tail;
        linked_list->tail = new_node;
    }
    return 0;
}

void print_maze(llist *linked_list)
{
    node *temp_node = linked_list->head;
    while(temp_node != NULL)
    {
        printf("%c", temp_node->cell_type);
        temp_node = temp_node->right;
    }
}


void free_list(llist *linked_list)
{
    node *temp_node = linked_list->head;
    while(temp_node != NULL)
    {
        node *free_node = temp_node;
        temp_node = temp_node->right;
        free(free_node);
    }
    free(linked_list);
}

int generate_map_linked_list(llist *linked_list, map_t *map)
{
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            add_link(linked_list, &map->maze[i][j]);
        }
    }

    return 0;
}

exit_data *init_exit_data(llist *linked_list, map_t *map)
{
    exit_data *ex_data = malloc(sizeof(exit_data));
    node *temp_node = linked_list->tail;
    while(temp_node->cell_type != '2')
    {
        temp_node = temp_node->left;
    }
    ex_data->exit_x = temp_node->position / map->cols + 1;
    ex_data->exit_y = temp_node->position - (ex_data->exit_x - 1) * map->cols;

    return ex_data;
}

int solve_map(map_t *map)
{
    llist *linked_list = init_list();
    generate_map_linked_list(linked_list, map);
    exit_data *ex_data = init_exit_data(linked_list, map);

    process_path(linked_list, ex_data, map);

    free(ex_data);
    free_list(linked_list);

    return 0;
}