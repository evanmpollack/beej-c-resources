#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define STARTING_SIZE 5
#define INPUT_PATH "/home/evanm/Desktop/Projects/beejs-c/exercises/my-getlines/input.txt"

int my_getlines(char** line, size_t size, FILE* fp);
int grow(char** line, size_t size);

int main(void) {
    FILE* fp = fopen(INPUT_PATH, "r");
    if (fp == NULL) {
        return -1;
    }
    char* line = NULL;
    int bytes_read = 0;
    while (bytes_read != -1) {
        bytes_read = my_getlines(&line, 0, fp);
        printf("%s\n", line);
    }
    free(line);
    fclose(fp);
    return 0;
}

/**
 * Personal implementation of getline().
 * This implementation works as follows:
 *  - Allocate 5 bytes if the size is 0 AND the line is empty
 *  - Get each character until a terminating character is reached (new line or EOF) and write it to the allocated buffer
 *  - if the number of characters ever reaches the amount of space allocated, double the amount of bytes allocated
 *  - Once the end is reached, shrink the buffer to the number of characters read from the file + 1 to be able to set nul-terminator
 * 
 * NOTE: unlike the glibc implementation, I combine the ideas of bytes read and sentinel values. 
 *  For example, instead of passing in a pointer to size and writing it each time more space is allocated, 
 *  I either return the bytes read directly or -1 for issue reading more
 */
int my_getlines(char** line, size_t size, FILE* fp) {
    if (fp == NULL)
        return -1;
    if (*line == NULL && size == 0)
        size = grow(line, size);
    if (size == -1)
        return size;
    
    int c;
    int count = 0;
    bool end_reached = false;
    do {
        c = fgetc(fp);
        if (c == EOF)
            end_reached = true;
        if (c == '\n' || c == EOF)
            break; 
        // Error here because I was indexing the char**, not char*, leading to an error when count > 0. 
        // This is undefined behavior because I was actually telling the computer to go up in memory 'count' bytes, read 8 bytes and inerpret it as a pointer,
        // and dereference the pointer that's in there. Well, what if there's not a pointer in there? 
        (*line)[count] = c;
        count++;
        if (count == size)
            size = grow(line, size);
        if (size == -1)
            break;
    } while (true);
    *line = realloc(*line, count + 1);
    if (*line == NULL)
        return -1;
    (*line)[count] = '\0';
    return (!end_reached) ? count : -1;
}

int grow(char** line, size_t size) {
    size = (*line != NULL && size != 0) ? (2 * size) : STARTING_SIZE;
    *line = realloc(*line, size);
    return (*line != NULL) ? size : -1;
}