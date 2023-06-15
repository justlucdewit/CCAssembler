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

// This function generates bytecode from the token list
void generate_bytecode(char* filename, token_list_t tokens) {
    // input filename is like filename.cca
    // output filename should be filename.ccb
    char* output_filename = get_output_filame_from_input_filename(filename);

    // Open the output file	for binary writing
    FILE* output_file = fopen(output_filename, "wb");

    // TODO: emit the bytes to the file
    // emit the bytes 0x01 0x02 0x03
    // 0x01 is the opcode for mov
    // 0x02 is the register number
    // 0x03 is the number 3
    // c code:
    char bytes[] = {0x01, 0x02, 0x03};
    fwrite(bytes, sizeof(char), 3, output_file);

    // Close the file
    fclose(output_file);
}

#endif