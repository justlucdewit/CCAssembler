#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdbool.h>
#include "file_reading.h"

void preprocessor_remove_comments(sourcecode_t* code) {
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

void preprocessor_replace_escape_chars(sourcecode_t* code) {
    char* new_buffer = (char*)malloc(sizeof(char) * code->length);
    size_t new_buffer_length = 0;
    bool inside_string_literal = false;  // Track whether we are inside a string literal

    for (size_t i = 0; i < code->length; i++) {
        if (code->data[i] == '"') {
            inside_string_literal = !inside_string_literal;  // Toggle the inside_string_literal flag
        }

        if (inside_string_literal && code->data[i] == '\\') {
            // Check for escape sequences within string literals
            if (i + 1 < code->length) {
                // Replace the escape sequence with the corresponding character
                switch (code->data[i + 1]) {
                    case 'n':
                        new_buffer[new_buffer_length] = '\n';
                        break;
                    case 't':
                        new_buffer[new_buffer_length] = '\t';
                        break;
                    // Add more escape sequences as needed
                    default:
                        // If it's an unknown escape sequence, keep the '\' as is
                        new_buffer[new_buffer_length] = '\\';
                        break;
                }
                i++;  // Skip the next character
            } else {
                // If '\' is the last character within a string literal, keep it as is
                new_buffer[new_buffer_length] = code->data[i];
            }
        } else {
            new_buffer[new_buffer_length] = code->data[i];
        }
        new_buffer_length++;
    }

    new_buffer[new_buffer_length] = '\0';

    free(code->data);
    code->data = new_buffer;
    code->length = new_buffer_length;
}

// this will preprocess the source code.
// it takes a pointer to a sourcecode struct and modifies it.
// it will
// 1. remove comments
// TODO: 2. handle import
void preprocess_sourcecode(sourcecode_t* code) {
    preprocessor_remove_comments(code);
    preprocessor_replace_escape_chars(code);
}

#endif
