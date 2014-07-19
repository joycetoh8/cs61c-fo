#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "parser.h"

/** Prints AST's string literals in the .data segment. */
void emit_strings_and_structs(AST *ast);
/** Prints the appropriate code to allocate space in the .data segment
 *  for whatever static memory the program uses. */
void emit_static_memory();
/** Prints AST's code for the .text segment to stdout if the AST
 *  is NOT a function definition. */
void emit_main(AST *ast);
/** Prints MIPS code which will exit the program. */
void emit_exit();
/** Prints AST's code for the .text segment to stdout if the AST
 *  IS a function definition. */
void emit_functions(AST *ast);

#endif
