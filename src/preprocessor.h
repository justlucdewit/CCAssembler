#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "file_reading.h"

// this will preprocess the source code.
// it takes a pointer to a sourcecode struct and modifies it.
// it will
// 1. remove comments
// TODO: 2. handle import
void preprocess_sourcecode(sourcecode_t* code) {
    char* new_buffer = (char*)malloc(sizeof(char) * code->length);
    size_t new_buffer_length = 0;

    // Remove comments (beginning with ;)
    for (size_t i = 0; i < code->length; i++) {
        if (code->data[i] == ';') {
            // Skip to the next line
            while (code->data[i] != '\n') {
                i++;
            }

            new_buffer[new_buffer_length] = '\n';
            new_buffer_length++;
        } else {
            new_buffer[new_buffer_length] = code->data[i];
            new_buffer_length++;
        }
    }

    // Add a null terminator to the end of the buffer
    new_buffer[new_buffer_length] = '\0';

    // Modify the sourcecode struct with the new data
    free(code->data);
    code->data = new_buffer;
    code->length = new_buffer_length;
}

#endif