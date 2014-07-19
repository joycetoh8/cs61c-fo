#include "parser.h"
#include "lexer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/** An array of the different string values of keywords. */
char *keywords[] = {"and", "or", "+", "-", "*", "/", "lt", "eq", 
		    "function", "struct", "arrow", "assign", "if", 
		    "while", "for", "sequence", "intprint", "stringprint",
		    "readint"};
/** Sister array of keywords. Keeps track of the corresponding enum. */
int enums[] = {node_AND, node_OR, node_PLUS, node_MINUS, node_MUL, node_DIV,
	       node_LT, node_EQ, node_FUNCTION, node_STRUCT, 
	       node_ARROW, node_ASSIGN, node_IF, node_WHILE, node_FOR, 
	       node_SEQ, node_I_PRINT, node_S_PRINT, node_READ_INT};
int nargs[] = {2,2,2,2,2,2,2,2,2,3,2,2,3,2,4,1,1,1,0}; // instead of 2 as node_STRUCT's minimum, I used 3 because I add a "struct#" later on
/** A hashmap used for more efficient lookups of (keyword, enum) pairs. */
smap *keyword_str_to_enum;
smap *func_decls; // declaration of functions
smap *stack_decls;
int num_paren = 0;;
int num_strings = 0;
int num_structs = 0;
char **decls_arr;
int size_of_decls_arr = 0;


/** Initializes keyword_str_to_enum so that it contains a map
 *  from the string value of a keyword to its corresponding enum. */
void initialize_keyword_to_enum_mapping();
void initialize_calls_to_num_args_mapping();
void free_ast();
void free_ast_lst();


void parse_init() {
    decls = smap_new();
    stack_sizes = smap_new();
    num_args = smap_new();
    strings = smap_new();
    keyword_str_to_enum = smap_new();
    initialize_keyword_to_enum_mapping();
    initialize_calls_to_num_args_mapping();
    func_decls = smap_new();
    stack_decls = smap_new();
}

void parse_close() {
    //smap_del_contents(decls); // if this line is used we will need to strcpy strings into decls key's
    smap_del(decls);
    smap_del(stack_sizes);
    smap_del(num_args);
    smap_del(strings);
    smap_del(keyword_str_to_enum);
    smap_del(func_decls);
    //smap_del_contents(stack_decls);
    smap_del(stack_decls);
    num_paren = 0;
    num_strings = 0;
    num_structs = 0;
}

AST *build_ast (lexer *lex) {
    /* TODO: Implement me. */
    /* Hint: switch statements are pretty cool, and they work 
     *       brilliantly with enums. */
    if (lex) {
        AST *ast = safe_malloc(sizeof(AST));
        token_type ast_type;
        char *ast_val;
        read_token(lex);
        if (peek_type(lex) == token_CLOSE_PAREN) {
            num_paren--;
            if (num_paren < 0) {
                fatal_error("Extra close parenthesis");
            } else {
                return NULL;
            }
        } else if (peek_type(lex) == token_END) {
            if (num_paren != 0) {
                fatal_error("Uneven number of parenthesis");
            } else {
                return NULL;
            }
        } else if (peek_type(lex) == token_OPEN_PAREN) {
            AST *struct_num; // for when we get a node_STRUCT
            num_paren++;
            read_token(lex);
            ast_type = peek_type(lex);
            ast_val = peek_value(lex);
            if (ast_type == token_CLOSE_PAREN) {
                fatal_error("Empty pair of parenthesis");
            } else if (ast_type == token_NAME) { //any variable name that comes after ( will be a function call
                ast->type = node_CALL;
            } else if (ast_type == token_KEYWORD) { // if it's a keyword, use smap
                if (!strcmp(ast_val,"None")) {
                    ast->type = node_INT;
                    ast_val = "0";
                } else {
                    ast->type = lookup_keyword_enum(ast_val);
                    if (ast->type == node_STRUCT) {
                        struct_num = safe_malloc(sizeof(AST));
                        struct_num->type = node_STRING;
                        struct_num->children = NULL;
                        struct_num->last_child = NULL;
                        struct_num->val = safe_malloc(8*sizeof(char));
                        sprintf(struct_num->val,"struct%d",num_structs);
                        num_structs++;
                    }
                }
            } else if (ast_type == token_INT || ast_type == token_STRING) {
                fatal_error("INT/STRING lone in parenthesis");
            } else if (ast_type == token_OPEN_PAREN) {
                fatal_error("Two consecutive open parenthesis");
            } else if (ast_type == token_END) {
                fatal_error("Abrupt end after open parenthesis");
            }
           
            ast->val = safe_malloc((strlen(ast_val) + 1) * sizeof(char));
            strcpy(ast->val, ast_val);
            ast->children = safe_malloc(sizeof(AST_lst));
            AST_lst *children = ast->children;
            // somewhere here fit the struct_num child in!!
            if (ast->type == node_STRUCT) {
                ast->last_child = children;
                children->val = struct_num;
                children->next = safe_malloc(sizeof(AST_lst));
                children = children->next;
            }
            AST *temp = build_ast(lex);
            if (temp == NULL && ast->type != node_STRUCT) {
                ast->children = NULL;
                ast->last_child = NULL;
            } else {
                while (temp != NULL) {
                    ast->last_child = children;
                    children->val = temp;
                    children->next = safe_malloc(sizeof(AST_lst));
                    children = children->next;
                    temp = build_ast(lex);
                }
                (ast->last_child)->next = NULL;
            }
            free(children);// free next of last child for which we malloced space earlier, thinking there existed a next.
            if (ast->type == node_FUNCTION) {
                ((ast->children)->val)->type = node_VAR;
                smap_put(num_args,((ast->children)->val)->val,AST_lst_len(((ast->children)->val)->children));
            }
        } else {
            ast_type = peek_type(lex);
            ast_val = peek_value(lex);
            if (ast_type == token_NAME) {
                ast->type = node_VAR;
            } else if (ast_type == token_KEYWORD) {
                if (!strcmp(ast_val,"None")) {
                    ast->type = node_INT;
                    ast_val = "0";
                } else {
                    ast->type = lookup_keyword_enum(ast_val);
                }
            }
            ast->val = safe_malloc((strlen(ast_val) + 1) * sizeof(char));
            strcpy(ast->val, ast_val);
            ast->children = NULL;
            ast->last_child = NULL;
        }
        switch (ast_type) {
            case token_INT:
                ast->type = node_INT;
                break;
            case token_STRING:
                ast->type = node_STRING;
                break;
        }
        return ast;
    } 
    return NULL;
}

void free_ast(AST *ptr) {
    /* TODO: Implement me. */
    if (ptr == NULL) {
        return;
    } else {
        free_ast_lst(ptr->children);
        free(ptr->val);
        free(ptr);
    }
}

void free_ast_lst(AST_lst *ptr) {
    if (ptr == NULL) {
        return;
    } else {
        free_ast(ptr->val);
        free_ast_lst(ptr->next);
        free(ptr);
    }
}

void check_tree_shape(AST *ptr) {
    /* TODO: Implement me. */
    /* Hint: This function is just asking to be table-driven */
    if (ptr) {
        AST_lst *children = ptr->children;
        if (ptr->type == node_FUNCTION) {
            AST *first_child = children->val;
            AST_lst *grandchildren = first_child->children;
            while (grandchildren) {
                if ((grandchildren->val)->type != node_VAR) {
                    fatal_error("Function definition arguments don't take in VAR arguments");
                }
                check_tree_shape(grandchildren->val);
                grandchildren = grandchildren->next;
            }
            children = children->next;
        }
        while (children) {
            check_tree_shape(children->val);
            children = children->next;
        }
        node_type ptr_type = ptr->type;
        if (ptr_type == node_INT || ptr_type == node_STRING || ptr_type == node_VAR) {
            if (AST_lst_len(ptr->children) != 0) {
                fatal_error("INT/STRING/VAR type have children");
            }
        } else if (ptr_type == node_STRUCT || ptr_type == node_SEQ) {
            if (AST_lst_len(ptr->children) < smap_get(num_args,ptr->val)) {
                fatal_error("STRUCT/SEQ don't have enough children");
            }
        } else if (smap_get(num_args,ptr->val) == -1) {
            fatal_error("Function called is not defined");
        } else if (AST_lst_len(ptr->children) != smap_get(num_args,ptr->val)) {
            fatal_error("Wrong number of children");
        }
    }
}

void gather_decls(AST *ast, char *env, int is_top_level, int can_declare_function) {
    /* TODO: Implement me. */
    /* Hint: switch statements are pretty cool, and they work 
     *       brilliantly with enums. */
    if (ast) {
        smap_increment(stack_sizes,env,0);
        AST_lst *children = ast->children;
        switch (ast->type) {
            case node_STRING:
                smap_put(strings,ast->val, num_strings);//(intptr_t) ast->val);
                num_strings++;
                // don't need to check children because shouldn't have children
                break;
            case node_VAR:
                if (!is_top_level) { // if we're not in the top level stack
                    if (smap_get(stack_decls,ast->val) == -1) { // check the stack declarations first
                        if (smap_get(decls,ast->val) == -1) { // otherwise check the global declarations
                            fatal_error("Variable called is not defined"); // if not in either stack or global declarations -> ERROR
                        }
                    }
                } else { // otherwise directly check global declarations
                    if (smap_get(decls,ast->val) == -1) {
                        fatal_error("Variable called is not defined");
                    }
                }
                break;
            case node_CALL:
                if (smap_get(func_decls,ast->val) == -1) {
                    fatal_error("Function called is not defined");
                }
                break;
            case node_FUNCTION:
                if (!can_declare_function) {
                    fatal_error("Tried to declare function when not at top level");
                }
                AST *first_child = children->val;
                if (smap_get(func_decls,first_child->val) != -1) {
                    fatal_error("Tried to declare function that already exists");
                } else {
                    smap_put(func_decls,first_child->val,smap_get(stack_sizes,env)); // still not sure about value either, maybe just set to 1
                }
                smap_put(stack_sizes,first_child->val,0); // initialized stack_size for this function
                is_top_level = 0;
                smap_del(stack_decls); // clean out whatever was in stack_decls from a previous function declaration
                stack_decls = smap_new();
                AST_lst *grandchildren = first_child->children;
                while (grandchildren) {
                    smap_put(stack_decls,(grandchildren->val)->val,smap_get(stack_sizes,first_child->val)); // again, not sure aboue value yet
                    smap_increment(stack_sizes,first_child->val,1); // increment only by one cause I'm just gonna count the number of words
                    grandchildren = grandchildren->next;
                }
                children = children->next;
                break;
            case node_ASSIGN:
                if (!strcmp(env,"if") || !strcmp(env,"for") || !strcmp(env,"while")) { // can be assigned to but just can't be declared
                    if (!is_top_level) {
                        if (smap_get(stack_decls,(children->val)->val) == -1 && smap_get(decls,(children->val)->val) == -1) {
                            fatal_error("Tried to declare variable in IF/FOR/WHILE");
                        }
                    } else {
                        if (smap_get(decls,(children->val)->val) == -1) {
                            fatal_error("Tried to declare variable in IF/FOR/WHILE");
                        }
                    }
                }
                if (!is_top_level) { // if we're not in the top level stack
                    if (smap_get(stack_decls,(children->val)->val) == -1 && smap_get(decls,(children->val)->val) == -1) { // if it's not in either stack or global declarations
                        smap_put(stack_decls,(children->val)->val,smap_get(stack_sizes,env)); // put it in the stack_declarations, still unsure about value
                        smap_increment(stack_sizes,env,1); // increment the stack size
                    }
                } else { // else we're just in the global stack frame
                    if (smap_get(decls,(children->val)->val) == -1) { // if not there
                        if (size_of_decls_arr == 0) {
                            decls_arr = safe_malloc(sizeof(char *));
                        } else {
                            decls_arr = safe_realloc(decls_arr,(size_of_decls_arr + 1) * sizeof(char *));
                        }
                        decls_arr[size_of_decls_arr] = (children->val)->val;
                        size_of_decls_arr++;
                        smap_increment(stack_sizes,env,1);  // increment stack size of env = "" global frame...should equal size_of_decls_arr;
                    }
                    smap_put(decls,(children->val)->val, smap_get(stack_sizes,env)); // still not sure about value
                }
                break;
        }
        can_declare_function = 0;
        while (children) {
            if (ast->type == node_IF || ast->type == node_WHILE || ast->type == node_FOR) {
                gather_decls(children->val,ast->val,is_top_level, can_declare_function);
            } else if (ast->type == node_FUNCTION) {
                gather_decls(children->val,((ast->children)->val)->val,is_top_level, can_declare_function);
            } else {
                gather_decls(children->val,env,is_top_level, can_declare_function);
            }
            children = children->next;
        }
    }

}


node_type lookup_keyword_enum(char *str) {
    if (smap_get(keyword_str_to_enum, keywords[0]) == -1) {
	initialize_keyword_to_enum_mapping();
    }
    return smap_get(keyword_str_to_enum, str);
}

void initialize_keyword_to_enum_mapping() {
    /* Note that enums is an *array*, not a pointer, so this
     * sizeof business is reasonable. */
    size_t num_keywords = sizeof(enums) / sizeof(int);
    for (size_t i = 0; i < num_keywords; i += 1) {
	smap_put(keyword_str_to_enum, keywords[i], enums[i]);
    }
}

void initialize_calls_to_num_args_mapping() {
    size_t num_calls = sizeof(nargs) / sizeof(int);
    for (size_t i = 0; i < num_calls; i++) {
        smap_put(num_args,keywords[i],nargs[i]);
    }
}

size_t AST_lst_len(AST_lst *lst) {
    int num_fields = 0;
    while (lst) {
	num_fields += 1;
	lst = lst->next;
    }
    return num_fields;
}


smap *decls;
smap *stack_sizes;
smap *num_args;
smap *strings;
