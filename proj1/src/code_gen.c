#include "code_gen.h"
#include "parser.h"
#include "util/smap.h"
#include "util/util.h"
#include <stdio.h>
#include <stdlib.h>

/** A counter to prevent the issuance of duplicate labels. */
unsigned label_count = 0;
/** True iff the data segment has already been partially printed. */
int data_seg_opened = 0;
/** True iff the text segment has already been partially printed. */
int text_seg_opened = 0;

int curr_t_reg = 9;
smap *t_regs;
char *t_registers[] = {"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9"};



void emit_strings_helper(AST *ast) {
	if (ast) {
		if (ast->type == node_STRING) {
			printf("string%d: .asciiz \"%s\"\n",smap_get(strings,ast->val),ast->val);
		}
		AST_lst *children = ast->children;
		while (children) {
			emit_strings_helper(children->val);
			children = children->next;
		}
	}
}

void emit_structs_helper(AST *ast) {
	if (ast) {
		if (ast->type == node_STRUCT) {
			int num_children = AST_lst_len((ast->children)->next);
			printf("%s: .word", ((ast->children)->val)->val);
			for (int i = 0 ; i < num_children; i++) {
				if (i == 0) {
					printf(" 0");
				} else {
					printf(", 0");
				}
			}
			printf("\n");
		}
		AST_lst *children = ast->children;
		while (children) {
			emit_structs_helper(children->val);
			children = children->next;
		}
	}
}
void emit_strings_and_structs(AST *ast) {
    /* TODO: Implement me. */
    if (!data_seg_opened) {
    	printf(".data\n");
    	data_seg_opened = 1;
    }
    if (ast) {
    	emit_strings_helper(ast);
    	emit_structs_helper(ast);
    } 
}

void emit_static_memory() {
    /* TODO: Implement me. */
    // go into smap *decls and iterate through keys
    // initialize to zero, can change later by loading address then sw into that address with offset 0.
    // e.g. la $t0, n
    //		sw $a0, 0($t0) // n = $a0
    // how to allocate space for stacks (same for strings)....? 
    // i think you're gonna have to look at length of kids and store that much space. 
    // e.g. var: .word 0, 0, 0, ... or.....
    // use pointers to the structs. which will be created later on in the stack pointer.

    /*
	for (size_t i = 0; i < decls->num_buckets; i++) {
		bucket *buck = decls->buckets + i;
		for (size_t j = 0; j < buck->num_pairs; j++) {
			pair *p = buck->pairs + j;
			printf("%s: .word 0\n",p->key);
		}
	} */
	if (!data_seg_opened) {
    	printf(".data\n");
    	data_seg_opened = 1;
    }
	for (size_t i = 0; i < size_of_decls_arr; i++) {
		printf("%s: .word 0\n",decls_arr[i]);
	}
}

void increment_curr_t_reg() {
	if (curr_t_reg < 9) {
		curr_t_reg++;
	} else if (curr_t_reg == 9) {
		curr_t_reg = 0;
	}
}

//bool all_free() {

//}

void next_t_reg() {
	char *t_reg_name = safe_malloc(4*sizeof(char));
	do {
		increment_curr_t_reg();
		sprintf(t_reg_name,"$t%d",curr_t_reg);
	} while (smap_get(t_regs,t_reg_name) == 1);
	free(t_reg_name);
}

void occupy(int t_reg) {
	char *t_reg_name = safe_malloc(4*sizeof(char));
	sprintf(t_reg_name,"$t%d",t_reg);
	smap_put(t_regs,t_reg_name,1);
	free(t_reg_name);
}

void free_t_reg(int t_reg) {
	char *t_reg_name = safe_malloc(4*sizeof(char));
	sprintf(t_reg_name,"$t%d",t_reg);
	smap_put(t_regs,t_reg_name,0);
	free(t_reg_name);
}

void emit_main_helper(AST *ast) { // argh. what happens if i use up all my t registers?!
	if (ast) {
		char *op_name;
		int t_reg1;
		int t_reg2;
		AST_lst *children;
		switch (ast->type) {
			case node_INT:
				next_t_reg();
				occupy(curr_t_reg);
				char *t_reg_name_int = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_int,"$t%d",curr_t_reg);
				printf("addi %s, $0, %s\n",t_reg_name_int,ast->val);
				free(t_reg_name_int);
				break;
			case node_STRING:
				next_t_reg();
				occupy(curr_t_reg);
				int str_num = smap_get(strings, ast->val);
				char *t_reg_name_str = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_str,"$t%d",curr_t_reg);
				printf("la %s, string%d\n",t_reg_name_str,str_num);
				free(t_reg_name_str);
				break;
			case node_VAR:
				next_t_reg();
				occupy(curr_t_reg);
				char *t_reg_name_var = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_var,"$t%d",curr_t_reg);
				printf("la %s, %s\n",t_reg_name_var,ast->val);
				printf("lw %s, 0(%s)\n",t_reg_name_var,t_reg_name_var);
				free(t_reg_name_var);
				break;
			case node_CALL:
				children = ast->children;
				for (int a = 0; a < smap_get(num_args,ast->val); a++) {
					emit_main_helper(children->val);
					t_reg1 = curr_t_reg;
					char *t_reg_name_call1 = safe_malloc(4*sizeof(char));
					sprintf(t_reg_name_call1,"$t%d",t_reg1);

					printf("move $a%d, %s\n", a, t_reg_name_call1);
					free_t_reg(t_reg1);
					free(t_reg_name_call1);

					children = children->next;
				}
				printf("jal func_%s\n",ast->val);
				next_t_reg();
				occupy(curr_t_reg);
				t_reg2 = curr_t_reg;
				char *t_reg_name_call2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_call2,"$t%d",t_reg2);

				printf("move %s, $v0\n", t_reg_name_call2);

				free(t_reg_name_call2);
				break;
			case node_AND:
				emit_main_helper((ast->children)->val);
				t_reg1 = curr_t_reg;
				char *t_reg_name_and1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_and1,"$t%d",t_reg1);

				printf("beq %s, $0, label%d\n",t_reg_name_and1,label_count); // first is false! jump to false label
				int label_num_and1 = label_count;
				label_count++;

				emit_main_helper(((ast->children)->next)->val);
				t_reg2 = curr_t_reg;
				char *t_reg_name_and2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_and2,"$t%d",t_reg2);

				next_t_reg();
				occupy(curr_t_reg);
				char *t_reg_name_and = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_and,"$t%d",curr_t_reg);

				printf("beq %s, $0, label%d\n",t_reg_name_and2, label_num_and1); // second is also false! jump to false label
				printf("addi %s, $0, 1\n",t_reg_name_and); // and result is true!
				printf("j label%d\n",label_count); // jump to continuing label
				int label_num_and2 = label_count;
				label_count++;
				printf("label%d: addi %s, $0, 0\n",label_num_and1,t_reg_name_and); // false label. and result is false!
				printf("label%d: \n",label_num_and2); // continuing label

				free_t_reg(t_reg1);
				free_t_reg(t_reg2);
				free(t_reg_name_and1);
				free(t_reg_name_and2);
				free(t_reg_name_and);
				break;
			case node_OR:
				emit_main_helper((ast->children)->val);
				t_reg1 = curr_t_reg;
				char *t_reg_name_or1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_or1,"$t%d",t_reg1);

				printf("bne %s, $0, label%d\n",t_reg_name_or1,label_count); // first is true! jump to true label
				int label_num_or1 = label_count;
				label_count++;

				emit_main_helper(((ast->children)->next)->val);
				t_reg2 = curr_t_reg;
				char *t_reg_name_or2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_or2,"$t%d",t_reg2);

				next_t_reg();
				occupy(curr_t_reg);
				char *t_reg_name_or = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_or,"$t%d",curr_t_reg);

				printf("bne %s, $0, label%d\n",t_reg_name_or2, label_num_or1); // second is true! jump to true label
				printf("addi %s, $0, 0\n",t_reg_name_or); // or result is false!
				printf("j label%d\n",label_count); // jump to continuing label
				int label_num_or2 = label_count;
				label_count++;
				printf("label%d: addi %s, $0, 1\n",label_num_or1,t_reg_name_or); // true label. or result is true!
				printf("label%d: \n",label_num_or2); // continuing label

				free_t_reg(t_reg1);
				free_t_reg(t_reg2);
				free(t_reg_name_or1);
				free(t_reg_name_or2);
				free(t_reg_name_or);
				break;
			case node_PLUS:
			case node_MINUS:
			case node_MUL:
			case node_DIV:
				if (ast->type == node_PLUS) {
					op_name = "add";
				} else if (ast->type == node_MINUS) {
					op_name = "sub";
				} else if (ast->type == node_MUL) {
					op_name = "mult";
				} else if (ast->type == node_DIV) {
					op_name = "div";
				}
				emit_main_helper((ast->children)->val);
				t_reg1 = curr_t_reg;
				char *t_reg_name_op1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_op1,"$t%d",t_reg1);

				emit_main_helper(((ast->children)->next)->val);
				t_reg2 = curr_t_reg;
				char *t_reg_name_op2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_op2,"$t%d",t_reg2);

				next_t_reg();
				occupy(curr_t_reg);
				char *t_reg_name_op = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_op,"$t%d",curr_t_reg);

				if (ast->type == node_MUL) {
					printf("%s %s, %s\n",op_name,t_reg_name_op1,t_reg_name_op2);
					printf("mflo %s\n",t_reg_name_op);
				} else {
					printf("%s %s, %s, %s\n",op_name,t_reg_name_op,t_reg_name_op1,t_reg_name_op2);
				}

				free_t_reg(t_reg1);
				free_t_reg(t_reg2);
				free(t_reg_name_op1);
				free(t_reg_name_op2);
				free(t_reg_name_op);
				break;
			case node_LT:
				emit_main_helper((ast->children)->val);	
				t_reg1 = curr_t_reg;
				char *t_reg_name_lt1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_lt1,"$t%d",t_reg1);

				emit_main_helper(((ast->children)->next)->val);	
				t_reg2 = curr_t_reg;
				char *t_reg_name_lt2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_lt2,"$t%d",t_reg2);

				next_t_reg();
				occupy(curr_t_reg);
				char *t_reg_name_lt = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_lt,"$t%d",curr_t_reg);

				printf("slt %s, %s, %s\n",t_reg_name_lt,t_reg_name_lt1,t_reg_name_lt2);
				free_t_reg(t_reg1);
				free_t_reg(t_reg2);
				free(t_reg_name_lt1);
				free(t_reg_name_lt2);
				free(t_reg_name_lt);
				break;
			case node_EQ:
				emit_main_helper((ast->children)->val);	
				t_reg1 = curr_t_reg;
				char *t_reg_name_eq1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_eq1,"$t%d",t_reg1);

				emit_main_helper(((ast->children)->next)->val);	
				t_reg2 = curr_t_reg;
				char *t_reg_name_eq2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_eq2,"$t%d",t_reg2);

				next_t_reg();
				occupy(curr_t_reg);
				char *t_reg_name_eq = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_eq,"$t%d",curr_t_reg);

				printf("beq %s, %s, label%d\n", t_reg_name_eq1, t_reg_name_eq2, label_count);
				int label_num_eq1 = label_count;
				label_count++;

				printf("addi %s, $0, 0\n", t_reg_name_eq);
				printf("j label%d\n", label_count);
				int label_num_eq2 = label_count;
				label_count++;

				printf("label%d: addi %s, $0, 1\n",label_num_eq1, t_reg_name_eq);
				printf("label%d: \n", label_num_eq2);

				free_t_reg(t_reg1);
				free_t_reg(t_reg2);
				free(t_reg_name_eq1);
				free(t_reg_name_eq2);
				free(t_reg_name_eq);
				break;
			case node_FUNCTION:
				break;
			case node_STRUCT:
				next_t_reg();
				occupy(curr_t_reg);
				t_reg1 = curr_t_reg;
				char *t_reg_name_struct = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_struct,"$t%d",t_reg1);

				printf("la %s, %s\n", t_reg_name_struct,((ast->children)->val)->val);

				children = (ast->children)->next;
				int offset = 0;
				char *t_reg_name_struct_val = safe_malloc(4*sizeof(char));
				while (children) {
					emit_main_helper(children->val);
					int t_reg_val = curr_t_reg;
					sprintf(t_reg_name_struct_val,"$t%d",t_reg_val);
					printf("sw %s, %d(%s)\n",t_reg_name_struct_val,offset*4,t_reg_name_struct);
					offset++;
					children = children->next;
					free_t_reg(t_reg_val);
				}

				// don't free t_reg1 because return value of struct is the pointer to the struct
				free(t_reg_name_struct_val);
				free(t_reg_name_struct);
				curr_t_reg = t_reg1;
				break;
			case node_ARROW:
				emit_main_helper((ast->children)->val);
				t_reg1 = curr_t_reg;
				char *t_reg_name_arrow1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_arrow1,"$t%d",t_reg1);

				int index = atoi((((ast->children)->next)->val)->val);

				next_t_reg();
				occupy(curr_t_reg);
				t_reg2 = curr_t_reg;
				char *t_reg_name_arrow2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_arrow2,"$t%d",t_reg2);

				printf("lw %s, %d(%s)\n",t_reg_name_arrow2,index * 4, t_reg_name_arrow1);

				free_t_reg(t_reg1); // don't free t_reg2 because that's the return value of arrow, the field accessed
				free(t_reg_name_arrow1);
				free(t_reg_name_arrow2);
				break;
			case node_ASSIGN:
				next_t_reg();
				occupy(curr_t_reg);
				t_reg1 = curr_t_reg;
				char *t_reg_name_assign1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_assign1,"$t%d",t_reg1);

				printf("la %s, %s\n", t_reg_name_assign1, ((ast->children)->val)->val);

				emit_main_helper(((ast->children)->next)->val);
				t_reg2 = curr_t_reg;
				char *t_reg_name_assign2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_assign2,"$t%d",t_reg2);

				printf("sw %s, 0(%s)\n",t_reg_name_assign2,t_reg_name_assign1);

				free_t_reg(t_reg1); // don't free t_reg2 because that's the return value of assign.
				free(t_reg_name_assign1);
				free(t_reg_name_assign2);
				break;
			case node_IF:
				emit_main_helper((ast->children)->val);
				t_reg1 = curr_t_reg;
				char *t_reg_name_if1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_if1,"$t%d",t_reg1);

				printf("bne %s, $0, label%d\n", t_reg_name_if1, label_count);
				int label_num_if1 = label_count; // true label
				label_count++;

				next_t_reg();
				occupy(curr_t_reg);
				int t_reg_if = curr_t_reg;
				char *t_reg_name_if = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_if,"$t%d",t_reg_if);

				emit_main_helper((((ast->children)->next)->next)->val);
				t_reg2 = curr_t_reg;
				char *t_reg_name_if2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_if2,"$t%d",t_reg2);

				printf("move %s, %s\n", t_reg_name_if, t_reg_name_if2);

				free_t_reg(t_reg2);

				printf("j label%d\n", label_count);
				int label_num_if2 = label_count; // continue label
				label_count++;

				printf("label%d: \n", label_num_if1);

				emit_main_helper(((ast->children)->next)->val);
				t_reg2 = curr_t_reg;
				sprintf(t_reg_name_if2,"$t%d",t_reg2);

				printf("move %s, %s\n", t_reg_name_if, t_reg_name_if2);
				printf("label%d: \n",label_num_if2);

				free_t_reg(t_reg1);
				free_t_reg(t_reg2);
				free(t_reg_name_if1);
				free(t_reg_name_if2);
				free(t_reg_name_if);
				curr_t_reg = t_reg_if;
				break;
			case node_WHILE:
				next_t_reg();
				occupy(curr_t_reg);
				t_reg1 = curr_t_reg;
				char *t_reg_name_while1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_while1,"$t%d",t_reg1);

				printf("add %s, $0, $0\n", t_reg_name_while1);

				printf("label%d: \n", label_count);
				int label_num_while1 = label_count; // predicate label
				label_count++;

				emit_main_helper((ast->children)->val);
				t_reg2 = curr_t_reg;
				char *t_reg_name_while2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_while2,"$t%d",t_reg2);

				printf("bne %s, $0, label%d\n", t_reg_name_while2, label_count);
				int label_num_while2 = label_count; // body label
				label_count++;

				printf("j label%d\n", label_count);
				int label_num_while3 = label_count; // continue label
				label_count++;

				printf("label%d: \n", label_num_while2);

				emit_main_helper(((ast->children)->next)->val);
				free_t_reg(curr_t_reg);
				printf("j label%d\n", label_num_while1);
				printf("label%d: \n", label_num_while3);

				free_t_reg(t_reg2);
				free(t_reg_name_while1);
				free(t_reg_name_while2);
				curr_t_reg = t_reg1;
				break;
			case node_FOR:
				next_t_reg();
				occupy(curr_t_reg);
				t_reg1 = curr_t_reg;
				char *t_reg_name_for1 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_for1,"$t%d",t_reg1);

				printf("add %s, $0, $0\n", t_reg_name_for1);

				emit_main_helper((ast->children)->val);
				free_t_reg(curr_t_reg);
				printf("label%d: \n", label_count);
				int label_num_for1 = label_count; // predicate label
				label_count++;

				emit_main_helper(((ast->children)->next)->val);
				t_reg2 = curr_t_reg;
				char *t_reg_name_for2 = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_for2,"$t%d",t_reg2);

				printf("bne %s, $0, label%d\n", t_reg_name_for2, label_count);
				int label_num_for2 = label_count; // body label
				label_count++;

				printf("j label%d\n", label_count);
				int label_num_for3 = label_count; // continue label
				label_count++;

				printf("label%d: \n", label_num_for2);

				emit_main_helper(((((ast->children)->next)->next)->next)->val);
				free_t_reg(curr_t_reg);
				emit_main_helper((((ast->children)->next)->next)->val);
				free_t_reg(curr_t_reg);

				printf("j label%d\n", label_num_for1);
				printf("label%d: \n", label_num_for3);

				free_t_reg(t_reg2);
				free(t_reg_name_for1);
				free(t_reg_name_for2);
				curr_t_reg = t_reg1;
				break;
			case node_SEQ:
				children = ast->children;
				while (children) {
					emit_main_helper(children->val); // can't use emit_main because will get rid of all registers
					children = children->next;
				}
				// while return value of last expression through un-freed register
				break;
			case node_I_PRINT:
				emit_main_helper((ast->children)->val);
				t_reg1 = curr_t_reg;
				char *t_reg_name_i_print = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_i_print,"$t%d",t_reg1);

				printf("li $v0, 1\n");
				printf("add $a0, %s, $0\n",t_reg_name_i_print);
				printf("syscall\n");
				printf("add %s, $0, $0\n", t_reg_name_i_print);

				// don't free t_reg1 = 0 because return value is None
				free(t_reg_name_i_print);
				break;
			case node_S_PRINT:
				emit_main_helper((ast->children)->val);
				t_reg1 = curr_t_reg;
				char *t_reg_name_s_print = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_s_print,"$t%d",t_reg1);

				printf("li $v0, 4\n");
				printf("add $a0, %s, $0\n",t_reg_name_s_print);
				printf("syscall\n");
				printf("add %s, $0, $0\n", t_reg_name_s_print);

				// don't free t_reg1 = 0 because return value is None
				free(t_reg_name_s_print);
				break;
			case node_READ_INT:
				printf("li $v0, 5\n");
				printf("syscall\n");

				next_t_reg();
				occupy(curr_t_reg);

				t_reg1 = curr_t_reg;
				char *t_reg_name_read_int = safe_malloc(4*sizeof(char));
				sprintf(t_reg_name_read_int,"$t%d",t_reg1);

				printf("add %s, $0, $v0\n", t_reg_name_read_int);

				free(t_reg_name_read_int);
				// don't free t_reg1 because it's the return value
				break;
		}
	}
}

void initialize_t_regs() {
	t_regs = smap_new();
    for (size_t i = 0; i < 10; i++) {
        smap_put(t_regs,t_registers[i],0);
    }
}

void close_t_regs() {
	smap_del(t_regs);
}

void emit_main(AST *ast) {
    /* TODO: Implement me. */
    if (ast) {
    	if (text_seg_opened) {
	    	close_t_regs(); // double check this again. not sure whats going on
	    } else {
	    	text_seg_opened = 1;
	    	printf(".text\n");
	    }
    }
    initialize_t_regs();
	emit_main_helper(ast);
}

void emit_exit() {
    printf("    li $v0 10\n");
    printf("    syscall\n");
}

void emit_functions(AST *ast) {
    /* TODO: Implement me. */
}
