#include "opcodes.h"
#include "file_reading.h"
#include "preprocessor.h"
#include "tokenizer.h"

void main() {
    // Initialize the opcode memory
    initialize_opcode_memory();

    // Define some opcodes for testing
    register_new_opcode("mov", 0x01, 2, (OPCODE_ARG_TYPES_T[]) {
        OPCODE_ARG_TYPE_REGISTER,
        OPCODE_ARG_TYPE_NUMERIC
    });

    register_new_opcode("add", 0x02, 3, (OPCODE_ARG_TYPES_T[]) {
        OPCODE_ARG_TYPE_REGISTER,
        OPCODE_ARG_TYPE_REGISTER
    });

    // Read the input file
    sourcecode_t sourcecode = read_file_contents("input.cca");
    preprocess_sourcecode(&sourcecode);
    token_list_t tokens = tokenize(sourcecode, (char*[]) {
        "a", "b", "c", "d", NULL
    }, (char*[]) {
        NULL
    });
    
    print_token_list(tokens);

    // Free up the memory
    free_opcode_memory();
}