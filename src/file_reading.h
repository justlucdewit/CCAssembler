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
sourcecode_t read_file_contents(const char* filename, const char* extension) {
    // Check if the file extension is correct
    size_t filename_length = strlen(filename);
    size_t extension_length = strlen(extension);

    // Throw error if filename does not end with extension
    if (filename_length < extension_length) {
        printf("Error: File '%s' does not end with extension '%s'\n", filename, extension);
        exit(1);
    }

    // Check if the file extension is correct
    for (size_t i = 0; i < extension_length; i++) {
        if (filename[filename_length - extension_length + i] != extension[i]) {
            printf("Error: File '%s' does not end with extension '%s'\n", filename, extension);
            exit(1);
        }
    }

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