#include "opcodes.h"
#include "file_reading.h"
#include "preprocessor.h"
#include "tokenizer.h"
#include "import_resolver.h"
#include "bytecode_generator.h"

int main(int argc, char* argv[]) {
    printf("Press Enter to exit...");
    getchar(); // Wait for user input

    if (argc <= 1) {
        printf("CC Assembler: \n\t Usage: cca <filename>.cca\n");
    }

    else {
        char* filename = argv[1];

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
        sourcecode_t sourcecode = read_file_contents(filename, ".cca");
        preprocess_sourcecode(&sourcecode);
        token_list_t tokens = tokenize(sourcecode, (char*[]) {
            "a", "b", "c", "d", NULL
        });

        token_list_t final_tokens = resolve_imports(tokens);

        // print_token_list(final_tokens);
        generate_bytecode(filename, final_tokens);

        printf("Compilation Successful");

        // Free up the memory
        free_opcode_memory();

        return 0;
    }
}
