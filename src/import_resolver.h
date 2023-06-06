#ifndef IMPORT_RESOLVER_H
#define IMPORT_RESOLVER_H

#include "opcodes.h"
#include "file_reading.h"
#include "preprocessor.h"
#include "tokenizer.h"

token_list_t resolve_imports(token_list_t old_tokens) {
    size_t token_count = 0;
    size_t token_capacity = 100;
    token_t* new_tokens = (token_t*)malloc(sizeof(token_t) * token_capacity);

    for (size_t i = 0; i < old_tokens.length; i++) {
        token_t token = old_tokens.tokens[i];

        // If its a keyword, resolve it
        if (token.type == TOKEN_KEYWORD && strcmp(token.data, "#import") == 0) {
            // Get the next token, assume its of type string
            token_t next_token = old_tokens.tokens[++i];
            if (next_token.type != TOKEN_STRING) {
                printf("Error: Expected string after #import, got %s\n", stringify_token_type(next_token.type));
                exit(1);
            }

            // Read the sourcecode from the file
            sourcecode_t sourcecode = read_file_contents(next_token.data);
            preprocess_sourcecode(&sourcecode);
            token_list_t tokens = tokenize(sourcecode, (char*[]) {
                "a", "b", "c", "d", NULL
            });

            token_list_t final_tokens = resolve_imports(tokens);

            // Add the final tokens to the tokens list
            for (size_t j = 0; j < final_tokens.length; j++) {
                if (token_count == token_capacity) {
                    token_capacity *= 2;
                    new_tokens = (token_t*)realloc(new_tokens, sizeof(token_t) * token_capacity);
                }

                new_tokens[token_count++] = final_tokens.tokens[j];
            }
        }

        // If its not a keyword, just carry it
        else {
            if (token_count == token_capacity) {
                token_capacity *= 2;
                new_tokens = (token_t*)realloc(new_tokens, sizeof(token_t) * token_capacity);
            }

            new_tokens[token_count++] = token;
        }
    }

    return (token_list_t) {
        .length = token_count,
        .tokens = new_tokens
    };
}

#endif