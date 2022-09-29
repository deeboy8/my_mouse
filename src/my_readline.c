#include <stdlib.h>
#include <unistd.h>

#define READLINE_READ_SIZE 1001

struct s_readline {
    ssize_t count;
    ssize_t n_read;
    char buffer[READLINE_READ_SIZE];
} t_readline;

char* my_readline(int fd) {
    static struct s_readline readline = {0};
    struct s_readline *readline_ptr = &readline;    
    int index = 0;

    //read entire file into provided buffer space at first call to readline()
    if (readline_ptr->count == 0) {
        readline_ptr->n_read = read(fd, &readline_ptr->buffer, READLINE_READ_SIZE);
        if (readline_ptr->n_read == -1 || readline_ptr->n_read == 0)
            return NULL;
    }

    //terminate function and return NULL if EOF is reached
    if (readline_ptr->count >= readline_ptr->n_read)
        return NULL;

    //allocate memory for 
    char* string = malloc(sizeof(string) * readline_ptr->n_read);
    if (string == NULL) {
        return NULL;
    }

    string[index++] = readline_ptr->buffer[readline_ptr->count];
    while ((readline_ptr->buffer[readline_ptr->count++]) != '\n') { 
        if (readline_ptr->count == readline_ptr->n_read) {
            string[index] = '\0';
            return string;
        }
        string[index] = readline_ptr->buffer[readline_ptr->count];
        index++;
    }
    string[index] = '\0';

    return string;
}