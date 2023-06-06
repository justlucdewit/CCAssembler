#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "file_reading.h"
#include "opcodes.h"

typedef enum token_type {
    TOKEN_INSTRUCTION,
    TOKEN_LABEL,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_REGISTER,
    TOKEN_UNKNOWN
} token_type_t;

const char* stringify_token_type(token_type_t type) {
    if (type == TOKEN_INSTRUCTION)
        return "TOKEN_INSTRUCTION";
    if (type == TOKEN_LABEL)
        return "TOKEN_LABEL";
    if (type == TOKEN_KEYWORD)
        return "TOKEN_KEYWORD";
    if (type == TOKEN_STRING)
        return "TOKEN_STRING";
    if (type == TOKEN_NUMBER)
        return "TOKEN_NUMBER";
    if (type == TOKEN_REGISTER)
        return "TOKEN_REGISTER";
    if (type == TOKEN_UNKNOWN)
        return "TOKEN_UNKNOWN";
}

typedef struct token {
    char* data;
    token_type_t type;
    size_t length;
} token_t;

typedef struct token_list {
    token_t* tokens;
    size_t length;
} token_list_t;

int check_if_string_in_list(char* string, char** list) {
    for (size_t i = 0; list[i] != NULL; i++) {
        if (strcmp(string, list[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

// keywords start with #
int check_if_string_is_keyword(char* string) {
    if (string[0] == '#') {
        return 1;
    }

    return 0;
}

// labels end with :
int check_if_string_is_label(char* string) {
    if (string[strlen(string) - 1] == ':') {
        return 1;
    }

    return 0;
}

// Supports hex, binary, octal, and decimal
int check_if_string_is_number(char* string) {

    // Check if the string is hex like '0x123' or '0x69420'
    if (strlen(string) > 2 && string[0] == '0' && string[1] == 'x') {
        for (size_t i = 2; i < strlen(string); i++) {
            if (string[i] < '0' | string[i] > '9' && string[i] < 'a' | string[i] > 'f' && string[i] < 'A' | string[i] > 'F') {
                return 0;
            }
        }

        return 1;
    }

    // Check if the string is binary like '0b1010' or '0b1101001010101000'
    if (strlen(string) > 2 && string[0] == '0' && string[1] == 'b') {
        for (size_t i = 2; i < strlen(string); i++) {
            if (string[i] != '0' && string[i] != '1') {
                return 0;
            }
        }

        return 1;
    }

    // Check if the string is octal like '0o123' or '0o69420'
    if (strlen(string) > 2 && string[0] == '0' && string[1] == 'o') {
        for (size_t i = 2; i < strlen(string); i++) {
            if (string[i] < '0' | string[i] > '7') {
                return 0;
            }
        }

        return 1;
    }

    // Check if the string is normal decimal like '123' or '69420'
    else {
        for (size_t i = 0; i < strlen(string); i++) {
            if (string[i] < '0' | string[i] > '9') {
                return 0;
            }
        }

        return 1;
    }
}

int check_if_string_is_instruction(char* string) {
    for (size_t i = 0; i < opcode_size; i++) {
        if (strcmp(string, opcodes[i].name) == 0) {
            return 1;
        }
    }

    return 0;
}

// Takes a sourcecode buffer and turns it into a list of tokens
token_list_t tokenize(sourcecode_t code, char* register_names[]) {
    size_t token_count = 0;
    size_t token_capacity = 100;
    token_t* tokens = (token_t*)malloc(sizeof(token_t) * token_capacity);

    size_t token_buffer_size = 0;
    size_t token_buffer_capacity = 100;
    char* token_buffer = (char*)malloc(sizeof(char) * token_buffer_capacity);

    // Loop through the source code
    for (size_t i = 0; i < code.length; i++) {
        // If we encounter a space, we have a token
        if (code.data[i] == ' ' | code.data[i] == '\n' | code.data[i] == '\t') {    
            // add null terminator, make sure buffer is big enough
            if (token_buffer_size >= token_buffer_capacity) {
                token_buffer_capacity *= 2;
                token_buffer = (char*)realloc(token_buffer, sizeof(char) * token_buffer_capacity);
            }

            token_buffer[token_buffer_size] = '\0';

            // Determine the token type
            token_type_t token_type = TOKEN_UNKNOWN;
            if (check_if_string_in_list(token_buffer, register_names))
                token_type = TOKEN_REGISTER;
            if (check_if_string_is_keyword(token_buffer))
                token_type = TOKEN_KEYWORD;
            if (check_if_string_is_label(token_buffer))
                token_type = TOKEN_LABEL;
            if (check_if_string_is_number(token_buffer))
                token_type = TOKEN_NUMBER;
            if (check_if_string_is_instruction(token_buffer))
                token_type = TOKEN_INSTRUCTION;

            // Add the token to the list, but only if it isnt only a null terminator, using strlen
            if (strlen(token_buffer) > 0) {
                tokens[token_count] = (token_t) {
                    .data = token_buffer,
                    .type = token_type,
                    .length = token_buffer_size
                };

                token_count++;
            }

            // Reset the token buffer
            token_buffer = (char*)malloc(sizeof(char) * token_buffer_capacity);
            token_buffer_size = 0;
        } else if (code.data[i] == '"') {
            // Reset the token buffer
            token_buffer = (char*)malloc(sizeof(char) * token_buffer_capacity);
            token_buffer_size = 0;

            // Add the string token to the list
            token_buffer = (char*)malloc(sizeof(char) * token_buffer_capacity);
            token_buffer_size = 0;

            // Loop until we find the closing "
            i++;
            while (code.data[i] != '"') {
                // grow buffer if needed
                if (token_buffer_size >= token_buffer_capacity) {
                    token_buffer_capacity *= 2;
                    token_buffer = (char*)realloc(token_buffer, sizeof(char) * token_buffer_capacity);
                }

                // Add the character to the token buffer
                token_buffer[token_buffer_size] = code.data[i];
                token_buffer_size++;
                i++;
            }

            // add null terminator, make sure buffer is big enough
            if (token_buffer_size >= token_buffer_capacity) {
                token_buffer_capacity *= 2;
                token_buffer = (char*)realloc(token_buffer, sizeof(char) * token_buffer_capacity);
            }

            token_buffer[token_buffer_size] = '\0';

            // Add the token to the list, but only if it isnt only a null terminator, using strlen
            if (strlen(token_buffer) > 0) {
                tokens[token_count] = (token_t) {
                    .data = token_buffer,
                    .type = TOKEN_STRING,
                    .length = token_buffer_size
                };

                token_count++;
            }

            // Reset the token buffer
            token_buffer = (char*)malloc(sizeof(char) * token_buffer_capacity);
            token_buffer_size = 0;
        } else {
            // grow buffer if needed
            if (token_buffer_size >= token_buffer_capacity) {
                token_buffer_capacity *= 2;
                token_buffer = (char*)realloc(token_buffer, sizeof(char) * token_buffer_capacity);
            }

            // Add the character to the token buffer
            token_buffer[token_buffer_size] = code.data[i];
            token_buffer_size++;
        }
    }

    // Add the last token to the list if it exists
    if (token_buffer_size > 0) {
        token_type_t token_type = TOKEN_UNKNOWN;
        if (check_if_string_in_list(token_buffer, register_names))
            token_type = TOKEN_REGISTER;
        if (check_if_string_is_keyword(token_buffer))
            token_type = TOKEN_KEYWORD;
        if (check_if_string_is_label(token_buffer))
            token_type = TOKEN_LABEL;
        if (check_if_string_is_number(token_buffer))
            token_type = TOKEN_NUMBER;
        if (check_if_string_is_instruction(token_buffer))
            token_type = TOKEN_INSTRUCTION;
            
        tokens[token_count] = (token_t) {
            .data = token_buffer,
            .type = token_type,
            .length = token_buffer_size
        };

        token_count++;
    }

    // Return the token list
    return (token_list_t) {
        .tokens = tokens,
        .length = token_count
    };
}

void print_token_list(token_list_t tokens) {
    for (size_t i = 0; i < tokens.length; i++) {
        printf("<token value=\"%s\" type=\"%s\" />\n", tokens.tokens[i].data, stringify_token_type(tokens.tokens[i].type));
    }
}

#endif