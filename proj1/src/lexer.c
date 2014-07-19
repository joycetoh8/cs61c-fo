#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexer.h"
#include "util/util.h"
#include "parser.h"

#define INIT_BUFFER_SIZE 256
#define OPEN_PAREN 40
#define CLOSE_PAREN 41
#define SPACE 32
#define DOUBLE_QUOTE 34
#define LINE_FEED 10
# define TAB 9
char* lexer_keywords[] = {"and","or","+","-","*","/","lt","eq","function","struct","arrow","None","assign","if","while","for","sequence","intprint","stringprint","readint"};

void init_lex(lexer *luthor) {
    luthor->file = NULL;
    luthor->buffer = NULL;
    luthor->type = token_SENTINEL;
    luthor->buff_len = 0;
}

void open_file(lexer *lex, char *filename) {
    if (lex) {
	lex->file = fopen(filename, "r");
	if (!lex->file) {
	    fatal_error("Could not read input file.\n");
	}
	lex->buff_len = INIT_BUFFER_SIZE;
	lex->buffer = safe_calloc(INIT_BUFFER_SIZE * sizeof(char));
    }
}

void close_file(lexer *lex) {
    if (lex) {
	fclose(lex->file);
	free(lex->buffer);
	lex->buff_len = 0;
	lex->buffer = NULL;
    }
}

bool isInt(char *buffer) {
    if (*buffer < 48 || *buffer > 57) {
        if (*buffer != '-') {
            return false;
        }
    }
    for (int i = 1; i < strlen(buffer); i++) {
        if (*(buffer + i) < 48 || *(buffer + i) > 57) {
            return false;
        }

    }
    return true;
}

bool isKeyword(char *buffer) {
    int size = 20;
    for (int i = 0; i < size; i++) {
        if (!strcmp(buffer,lexer_keywords[i])) {
            return true;
        }
    }
    return false;
}

bool isValidName(char *buffer) {
    // variable names can only start with letters or underscore, but can later have letters, numbers and underscore. 
    if ((*buffer < 65 || *buffer > 90) && (*buffer < 97 || *buffer > 122) && *buffer != 95) {
        return false;
    }
    for (int i = 1; i < strlen(buffer); i++) {
        if ((*(buffer + i) < 48 || *(buffer + i) > 57) && (*(buffer + i) < 65 || *(buffer + i) > 90) && (*(buffer + i) < 97 || *(buffer + i) > 122) && *(buffer + i) != 95) {
            return false;
        }
    }
    return true;
}

void read_token(lexer *lex) {
    /* TODO: Implement me. */
    /* HINT: fgetc() and ungetc() could be pretty useful here. */
        bool type_set = false;
        char empty_string[] = "";
        int size = 0;
        char *temp = safe_malloc(sizeof(char));
        strcpy(temp,empty_string);
        int curr = fgetc(lex->file);
        //printf("First fgetc'ed: %c\n",curr);
        char endChar = SPACE;
        while (curr == LINE_FEED || curr == SPACE || curr == TAB) { // If the next character is a line feed or space, keep getting the next character until a valid character is reached.
            curr = fgetc(lex->file);
        }
        if (curr == EOF && feof(lex->file)) { // If we reach the end of the file return from the function.
            strcpy(lex->buffer, temp);
            free(temp);
            lex->buff_len = size;
            lex->type = token_END;
            type_set = true;
            return;
        }
        if (curr == OPEN_PAREN) { // If the next character is an open parenthesis, put a space after it so we stop getting more characters. Set type.
            ungetc(SPACE,lex->file);
            lex->type = token_OPEN_PAREN;
            type_set = true;
        } else if (curr == CLOSE_PAREN) { // If the next character is a close parenthesis, put a space after it so we stop getting more characters. Set type.
            ungetc(SPACE, lex->file);
            lex->type = token_CLOSE_PAREN;
            type_set = true;
        } else if (curr == DOUBLE_QUOTE) { // If the next character is double quotes, we want to end on the next double quote to show up. Set type.
            curr = fgetc(lex->file);
            //printf("After DOUBLE_QUOTE fgetc'ed: %c\n",curr);
            endChar = DOUBLE_QUOTE;
            lex->type = token_STRING;
            type_set = true;
        }
        while (curr != endChar && curr != LINE_FEED && curr != EOF) { // Keep getting characters and putting it in our temp string until the correct end character is reached.
            temp = safe_realloc(temp,(size + 2)*sizeof(char));
            size++;
            *(temp + size - 1) = curr;
            *(temp + size) = '\0';
            curr = fgetc(lex->file);
            //printf("Just fgetc'ed: %c\n",curr);
            if (curr == CLOSE_PAREN || curr == OPEN_PAREN) { // If we reach a close/open parenthesis before the end character, put the parenthesis back (for later) and add a space for fgetc to get next.
                if (!(type_set && lex->type == token_STRING)) {
                    ungetc(curr,lex->file);
                    ungetc(SPACE,lex->file);
                    curr = fgetc(lex->file);
                    //printf("Just fgetc'ed: %c\n",curr);
                }
            }
        }
        if (curr == EOF) {
            if (endChar != SPACE) {
                fatal_error("Reached EOF before complete token");
            }
        }
        lex->buffer = safe_realloc(lex->buffer, (size + 1)*sizeof(char));
        strcpy(lex->buffer, temp);
        free(temp);
        lex->buff_len = size;
        if (type_set == false) {
            if (isKeyword(lex->buffer)) {
                lex->type = token_KEYWORD;
                type_set = true;
            } else if (isInt(lex->buffer)) {
                lex->type = token_INT;
                type_set = true;
            } else if (isValidName(lex->buffer)) {
                lex->type = token_NAME;
                type_set = true;
            } else {
                fatal_error("Invalid token type");
            }
        }
    
}

token_type peek_type(lexer *lex) {
    if (!lex) {
	return token_SENTINEL;
    }
    if (lex->type == token_SENTINEL) {
	read_token(lex);
    }
    return lex->type;
}

char *peek_value(lexer *lex) {
    if (!lex) {
	return NULL;
    }
    if (lex->type == token_SENTINEL) {
	read_token(lex);
    }
    return lex->buffer;
}
