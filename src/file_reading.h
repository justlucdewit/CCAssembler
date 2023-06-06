#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>
#include <stdlib.h>

// Define the assembly language opcodes
typedef struct sourcecode {
    char* data;
    size_t length;
} sourcecode_t;

// This function reads the contents of a file into a string view
// a string view is just a buffer that contains a length of the string
sourcecode_t read_file_contents(const char* filename) {
    // Open the file
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        exit(1);
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the file
    char* file_contents = (char*) malloc(sizeof(char) * (file_size + 1));
    size_t bytes_read = fread(file_contents, sizeof(char), file_size, file);

    // Null-terminate the file contents
    file_contents[bytes_read] = '\0';

    // Close the file
    fclose(file);

    // Return the file contents
    return (sourcecode_t) {
        .data = file_contents,
        .length = bytes_read
    };
}

#endif