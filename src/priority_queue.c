#include "priority_queue.h"

int calculate_cell_distance(node *cell, exit_data *ex_data, map_t *map)
{
    if(cell->cell_type != '*' || cell->cell_type != '1' || cell->cell_type != '2' || cell->distance == -1)
    {
        int cell_x = cell->position / map->cols + 1;
        int cell_y = cell->position - (ex_data->exit_x - 1) * map->cols;
        cell->distance = abs(cell_x - ex_data->exit_x) + abs(cell_y - ex_data->exit_y);
    }

    return 0;
}

node *find_entrance(llist *linked_list)
{
    node *temp_node = linked_list->head;
    while(temp_node->cell_type != '1')
    {
        temp_node = temp_node->right;
    }

    return temp_node;
}

node *starting_point(llist *linked_list, exit_data *ex_data, map_t *map) //starting point is the cell right below the entrance
{
    node *entrance_node = find_entrance(linked_list);
    
    node *starting_node = entrance_node;
    // cell position + # of cols
    int starting_position = starting_node->position + map->cols;
    
    while(starting_node->position < starting_position)
    {
        starting_node = starting_node->right;
    }
    starting_node->prior_step = entrance_node;
    starting_node->cost = 1;
    calculate_cell_distance(starting_node, ex_data, map);
    starting_node->priority = starting_node->cost + starting_node->distance;
    
    return starting_node;
}

node *find_up(map_t *map, node *current_cell)
{
    node *up = current_cell;
    int up_position = current_cell->position - map->cols;
    while(up->position != up_position)
    {
        up = up->left;
    }

    if(up->cell_type == '*' || up->cell_type == '1' || up->in_pq == 1) 
    {
        return NULL;
    }

    return up;
}

node *find_down(map_t *map, node *current_cell)
{
    node *down = current_cell;
    int down_position = current_cell->position + map->cols;
    while(down->position != down_position)
    {
        down = down->right;
    }

    if(down->cell_type == '*' || down->cell_type == '1' || down->in_pq == 1)
    {
        return NULL;
    }
    return down;
}

node *find_left( node *current_cell)
{
    if(current_cell->left->cell_type == '*' || current_cell->left->cell_type == '1' || current_cell->left->in_pq == 1)
    {
        return NULL;
    }
    return current_cell->left;
}

node *find_right( node *current_cell)
{
    if(current_cell->right->cell_type == '*' || current_cell->right->cell_type == '1' || current_cell->right->in_pq == 1)
    {
        return NULL;
    }
    return current_cell->right;
}

int calculate_cell_data(node **neighbours, exit_data *ex_data, map_t *map, node *current_cell)
// cell data includes
// • distance to the exit
// • cost from the entrace
// • priority = cost + distance
{
    for(int i = 0; i < ADJUSTED_CELLS_NUMBER; i++)
    {
        if(neighbours[i] != NULL && neighbours[i]->priority && neighbours[i]->cell_type != '2')
        {
            calculate_cell_distance(neighbours[i], ex_data, map);
            neighbours[i]->cost = current_cell->cost + 1;
            neighbours[i]->priority = neighbours[i]->cost + neighbours[i]->distance;
        }
    }
    return 0;
}

node **find_neighbour_cells(map_t *map, node *current_cell, exit_data *ex_data)
{
    node **neighbours = malloc(sizeof(node) * 4); //the initial order is [ up > left> right> down ]
    neighbours[0] = find_up(map, current_cell);
    neighbours[1] = find_left(current_cell);
    neighbours[2] = find_right(current_cell);
    neighbours[3] = find_down(map, current_cell);
    calculate_cell_data(neighbours, ex_data, map, current_cell);
    return neighbours;
}

/*
void print_out_neighbours(node **neighbours)
{
    for(int i = 0; i < ADJUSTED_CELLS_NUMBER; i++)
    {
        printf("\n");
        if(neighbours[i] == NULL)
        {
            printf("neighbours[%i] is NULL \n", i);
        } else
        {
            printf("neighbours[%i]->priority is %i\n", i, neighbours[i]->priority);
        }

        printf("\n");
    }
}
*/
/*
typedef struct pqnode_s {
    node *cell;
    struct pqnode_s* next;
} pqnode_t;

typedef struct path_s {
    node *cell;
    struct path_s* right;
    struct path_s* left;
} path_t;
*/
path_t *init_path_node(void)
{
    path_t *new_path = malloc(sizeof(path_t));
    new_path->cell = NULL;
    new_path->right = NULL;
    new_path->left = NULL;

    return new_path;
}

void free_path(path_t *path)
{
    while(path != NULL)
    {
        path_t *delete = path;
        path = path->right;
        free(delete);
    }
}

int if_connected(path_t *path_node, node *cell, map_t *map)
{
    //check up
    node *up = path_node->cell;
    int up_position = path_node->cell->position - map->cols;
    while(up->position != up_position)
    {
        up = up->left;
    }
    if(up == cell)
    {
        return TRUE;
    }
    //check left
    if(path_node->cell->left == cell)
    {
        return TRUE;
    }
    //check right
    if(path_node->cell->right == cell)
    {
        return TRUE;
    }
    //check down
    node *down = path_node->cell;
    int down_position = path_node->cell->position + map->cols;
    while(down->position != down_position)
    {
        down = down->right;
    }
    if(down == cell)
    {
        return TRUE;
    }

    return FALSE;
}

path_t *check_connection(path_t *path_node, node *cell, map_t *map)
{
    //if(path_node->cell != NULL) printf("path_node is ok\n");
    //if(cell != NULL) printf("cell is ok\n");
    //printf("path_node->cell->position is %i\n", path_node->cell->position);
    //printf("cell->position is %i\n", cell->position);
    while(path_node->cell != cell)// || !if_connected(path_node, cell, map))
    {
        if(if_connected(path_node, cell, map)) break;
      //printf("ifconnected is %i\n", !if_connected(path_node, cell, map));
        //printf("here 3\n");
        //printf("path_node->cell->position is %i\n", path_node->cell->position);
        path_t *temp = path_node;
        path_node = path_node->left;
        free(temp);
    }
    return path_node;
}

void insert_path_node(path_t *path, node *cell, map_t *map)
{
    //printf("\n\ninsert_path_node\n\n");
    if(path->right == NULL && path->cell == NULL)
    // if the path is empty and there is only one path node with no cell attached
    // the cell is attached to the path
    {
        path->cell = cell;
    } else
    {
        path_t *temp = path;
        while(temp->right != NULL)
        // find the last node on the path
        {
            temp = temp->right;
        }
        temp = check_connection(temp, cell, map); 
        //check if the last node is the cell or if it is conencted to the cell
        //if it is the cell then skip init new path node
        //if it is connected then add a new node

        if(temp->cell != cell)
        {
            path_t *new_node = init_path_node();
            new_node->cell = cell;
            new_node->left = temp;
            temp->right = new_node;
        }
    }
}

pqnode_t *init_pq_node(void)
{
    pqnode_t *new_pq = malloc(sizeof(pqnode_t));
    new_pq->cell = NULL;
    new_pq->next = NULL;

    return new_pq;
}

void free_pq(pqnode_t *pq)
{
    while(pq != NULL)
    {
        pqnode_t *delete = pq;
        pq = pq->next;
        free(delete);
    }
}

pqnode_t *insert_pq_node(pqnode_t *pq, node *cell)
{
    // check if pq is empty
    if(pq->cell == NULL && pq->next == NULL)
    {
        pq->cell = cell;
    } else
    {
        pqnode_t *new_pq_node = init_pq_node();
        new_pq_node->cell = cell;
        if(new_pq_node->cell->priority <= pq->cell->priority) // if we need to attach the new node to the front of pq
        {
            new_pq_node->next = pq;
            pq = new_pq_node;
        } else // if we need to insert the new node elsewhere within pq
        {
            pqnode_t *temp = pq;
            while(temp->next != NULL && temp->next->cell->priority < new_pq_node->cell->priority)
            {
                temp = temp->next;
            }
            new_pq_node->next = temp->next;
            temp->next = new_pq_node;
        }
    }

    cell->in_pq = 1;
    return pq;
}

void print_path(path_t *path)
{
    path_t *temp = path;
    printf("path:\n");
    while(temp != NULL)
    {
        printf("the node is %i\n", temp->cell->position);
        temp = temp->right;
    }
    printf("\n");
}

void print_pq(pqnode_t *pq)
{
    pqnode_t *temp = pq;
    printf("pq:\n");
    while(temp != NULL)
    {
        printf("the node is %i\n", temp->cell->position);
        printf("the priority is %i\n", temp->cell->priority);
        printf("the cost is %i\n", temp->cell->cost);
        printf("the distance is %i\n", temp->cell->distance);
        printf("\n");
        temp = temp->next;
    }
    printf("\n");
}

pqnode_t *pop_pq_node(pqnode_t *pq)
{
    pqnode_t *delete = pq;
    pq = pq->next;
    free(delete);
    return pq;
}

int assess_neighbours(node **neighbours)
{
    for(int i = 0; i < ADJUSTED_CELLS_NUMBER; i++)
    {
        if(neighbours[i] != NULL && neighbours[i]->cell_type == '2')
        {
            return TRUE;
        }
    }
    return FALSE;
}

void mark_path(path_t *path)
{
    path_t *temp = path;
    while(temp != NULL)
    {
        temp->cell->cell_type = 'o';
        temp = temp->right;
    }
}

void print_solved_map(llist *linked_list, map_t *map)
{
    node *temp = linked_list->head;
    int count = 0;
    while(temp != NULL)
    {
        printf("%c", temp->cell_type);
        count++;
        temp = temp->right;
        if(count == map->cols)
        {
            count = 0;
            printf("\n");
        }
    }
}


pqnode_t * pass_neighbours_in_pq(pqnode_t *pq, node **neighbours)
{
    for(int i = ADJUSTED_CELLS_NUMBER - 1; i > -1; i--)
    {
        if(neighbours[i] != NULL)
        {
            pq = insert_pq_node(pq, neighbours[i]);
        }
    }
    return pq;
}

int is_deadend(node **neighbours)
{
    int count_null = 0;
    for(int i = 0; i < ADJUSTED_CELLS_NUMBER; i++)
    {
        if(neighbours[i] == NULL)
        {
            count_null++;
        }
    }
    if(count_null == 4)
    {
        return TRUE;
    } else
    {
        return FALSE;
    }
}

int process_path(llist *linked_list, exit_data *ex_data, map_t *map)
{ 
    node *current_cell = starting_point(linked_list, ex_data, map);
    node **neighbours = find_neighbour_cells(map, current_cell, ex_data);
    
    path_t *path = init_path_node();
    insert_path_node(path, current_cell, map);

    pqnode_t *pq = init_pq_node();
    pq = insert_pq_node(pq, current_cell);

    while(pq != NULL)
    {
        if(assess_neighbours(neighbours))
        {
            mark_path(path);
            break;
        }

        pq = pass_neighbours_in_pq(pq, neighbours); // inserting into pq according to priority and directionality
       // printf("\n\n");
       // print_pq(pq);
       // printf("\n\n");
        repeat:
        current_cell = pq->cell; // change the current cell to th first node within pq
        free(neighbours); // free the array of neigbour cells
        neighbours = find_neighbour_cells(map, current_cell, ex_data); // gather neighbours for the new current cell
        
        if(is_deadend(neighbours))//if neighbours array is null pop pq node and move to a new current cell (within pq node)
        {
            pq = pop_pq_node(pq); // pop the first node from pq
            goto repeat;
        }
        insert_path_node(path, current_cell, map);
    } 

    if(pq != NULL)
    {
        mark_path(path);
        print_solved_map(linked_list, map);
    }
    //print_path(path);
    //print_pq(pq);

    //print_out_neighbours(neighbours);

    free_pq(pq);
    free_path(path);
    free(neighbours);

    return 0;
}