#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include "tokenizer.h"
#include <stdio.h>

char* get_output_filame_from_input_filename(char* input_filename) {
    size_t filename_length = strlen(input_filename);
    char* output_filename = (char*) malloc(sizeof(char) * (filename_length + 1));
    strcpy(output_filename, input_filename);
    output_filename[filename_length - 1] = 'b';
    output_filename[filename_length] = '\0';
    return output_filename;
}

void bytecode_error_check(char error, char* expected, char* got) {
    if (!error)
        return;

    printf("expected %s, got '%s'", expected, got);
    exit(1);
}

void write_byte_to_buffer(uint8_t byte, char** output_buffer, size_t* output_buffer_capacity, size_t* output_buffer_length) {
    if (*output_buffer_length + 1 >= *output_buffer_capacity) {
        // If the buffer is full, allocate more memory (e.g., double the capacity).
        *output_buffer_capacity *= 2;
        *output_buffer = (char*)realloc(*output_buffer, *output_buffer_capacity);
    }

    // Add the byte to the buffer.
    (*output_buffer)[*output_buffer_length] = (char)byte;
    (*output_buffer_length)++;
}

// This function generates bytecode from the token list
void generate_bytecode(char* filename, token_list_t tokens) {
    // input filename is like filename.cca
    // output filename should be filename.ccb
    char* output_filename = get_output_filame_from_input_filename(filename);

    // Open the output file	for binary writing
    FILE* output_file = fopen(output_filename, "wb");

    // Create dynamic buffer
    size_t output_buffer_length = 0;
    size_t output_buffer_capacity = 1024;
    char* output_buffer = (char*) malloc(sizeof(char) * output_buffer_capacity);

    // Loop over each token in the token list
    for (size_t i = 0; i < tokens.length; i++) {
        // Get the token
        token_t token = tokens.tokens[i];

        // Write the token type to the buffer
        token_type_t tokenType = token.type;

        // raw keyword
        if (tokenType == TOKEN_KEYWORD && strcmp(token.data, "raw")) {
            token_t stringToken = tokens.tokens[++i];

            // Error handling
            bytecode_error_check(
                stringToken.type != TOKEN_STRING,
                "string after keyword",
                stringify_token_type(stringToken.type)
            );

            // Emit the bytes of the string into the bytecode
            for (int char_index = 0; char_index < stringToken.length; char_index++) {
                uint8_t byte = stringToken.data[char_index];
                write_byte_to_buffer(byte, &output_buffer, &output_buffer_capacity, &output_buffer_length);
            }
        }

        // normal instruction
        else if (tokenType == TOKEN_INSTRUCTION) {

        }

        else {
            puts("FOUND NON INSTRUCTION TOKEN");
        }
    }

    // write buffer to file
    fwrite(output_buffer, sizeof(char), output_buffer_length, output_file);

    // Close the file
    fclose(output_file);
}

#endif
