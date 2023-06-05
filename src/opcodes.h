#ifndef OPCODES_H
#define OPCODES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum OPCODE_ARG_TYPES {
    OPCODE_ARG_TYPE_NUMERIC,
    OPCODE_ARG_TYPE_REGISTER,
} OPCODE_ARG_TYPES_T;

typedef struct opcode {
    char* name;
    int8_t code;
    size_t num_args;
    OPCODE_ARG_TYPES_T* args;
} opcode_t;

// Set up some dynamic memory for the opcodes
size_t opcode_size = 0;
size_t opcode_capacity = 100;
opcode_t* opcodes = NULL;

// Function that initializes the opcode memory
// this memory will store all the possible opcodes
// in this assembly language
void initialize_opcode_memory() {
    opcodes = (opcode_t*) malloc(sizeof(opcode_t) * opcode_capacity);
}

// Function that frees the opcode memory
// at the end of the program
void free_opcode_memory() {
    free(opcodes);
}

// This function registers a new opcode by inserting it into the
// opcode memory
void register_new_opcode(const char* opcode_name, int8_t opcode_code, size_t num_args, OPCODE_ARG_TYPES_T* args) {
    // Resize if needed
    if (opcode_size == opcode_capacity) {
        opcode_capacity *= 2;
        opcodes = (opcode_t*) realloc(opcodes, sizeof(opcode_t) * opcode_capacity);
    }

    // Set values
    opcodes[opcode_size].name = (char*) malloc(sizeof(char) * strlen(opcode_name));
    strcpy(opcodes[opcode_size].name, opcode_name);
    opcodes[opcode_size].code = opcode_code;
    opcodes[opcode_size].num_args = num_args;
    opcodes[opcode_size].args = (OPCODE_ARG_TYPES_T*)  malloc(sizeof(OPCODE_ARG_TYPES_T) * num_args);
    memcpy(opcodes[opcode_size].args, args, sizeof(OPCODE_ARG_TYPES_T) * num_args);

    // Increment size
    opcode_size++;
}

#endif