.data
string0: .asciiz "struct0"
struct0: .word 0, 0, 0, 0, 0, 0
string1: .asciiz "struct1"
struct1: .word 0, 0, 0, 0, 0, 0
string2: .asciiz "struct2"
struct2: .word 0, 0, 0, 0
string3: .asciiz "struct3"
struct3: .word 0, 0, 0, 0, 0, 0
string4: .asciiz "struct4"
struct4: .word 0, 0, 0, 0, 0
string5: .asciiz "struct5"
struct5: .word 0, 0, 0, 0, 0, 0
string6: .asciiz "struct6"
string7: .asciiz "hi"
string8: .asciiz "string"
string9: .asciiz "\n"
struct6: .word 0, 0, 0, 0
string10: .asciiz "struct7"
struct7: .word 0, 0, 0, 0, 0, 0
string11: .asciiz "struct8"
struct8: .word 0, 0, 0, 0, 0, 0
string12: .asciiz "struct9"
struct9: .word 0, 0, 0, 0
string13: .asciiz "struct10"
struct10: .word 0, 0, 0, 0, 0, 0
string14: .asciiz "struct11"
struct11: .word 0, 0, 0, 0, 0
string15: .asciiz "struct12"
struct12: .word 0, 0, 0, 0, 0, 0
.text
la $t0, struct0
addi $t1, $0, 0
sw $t1, 0($t0)
addi $t2, $0, 1
sw $t2, 4($t0)
addi $t3, $0, 2
sw $t3, 8($t0)
addi $t4, $0, 3
sw $t4, 12($t0)
addi $t5, $0, 4
sw $t5, 16($t0)
addi $t6, $0, 5
sw $t6, 20($t0)
lw $t1, 0($t0)
li $v0, 1
add $a0, $t1, $0
syscall
add $t1, $0, $0
la $t2, struct1
addi $t3, $0, 0
sw $t3, 0($t2)
addi $t4, $0, 1
sw $t4, 4($t2)
addi $t5, $0, 2
sw $t5, 8($t2)
addi $t6, $0, 3
sw $t6, 12($t2)
addi $t7, $0, 4
sw $t7, 16($t2)
addi $t8, $0, 5
sw $t8, 20($t2)
lw $t3, 4($t2)
li $v0, 1
add $a0, $t3, $0
syscall
add $t3, $0, $0
la $t4, struct2
addi $t5, $0, 0
sw $t5, 0($t4)
addi $t6, $0, 1
sw $t6, 4($t4)
addi $t7, $0, 2
sw $t7, 8($t4)
addi $t8, $0, 3
sw $t8, 12($t4)
lw $t5, 8($t4)
li $v0, 1
add $a0, $t5, $0
syscall
add $t5, $0, $0
la $t6, struct3
addi $t7, $0, 0
sw $t7, 0($t6)
addi $t8, $0, 1
sw $t8, 4($t6)
addi $t9, $0, 2
sw $t9, 8($t6)
addi $t0, $0, 3
sw $t0, 12($t6)
addi $t1, $0, 4
sw $t1, 16($t6)
addi $t2, $0, 5
sw $t2, 20($t6)
lw $t7, 12($t6)
li $v0, 1
add $a0, $t7, $0
syscall
add $t7, $0, $0
la $t8, struct4
addi $t9, $0, 0
sw $t9, 0($t8)
addi $t0, $0, 1
sw $t0, 4($t8)
addi $t1, $0, 2
sw $t1, 8($t8)
addi $t2, $0, 3
sw $t2, 12($t8)
addi $t3, $0, 4
sw $t3, 16($t8)
lw $t9, 16($t8)
li $v0, 1
add $a0, $t9, $0
syscall
add $t9, $0, $0
la $t0, struct5
addi $t1, $0, 0
sw $t1, 0($t0)
addi $t2, $0, 1
sw $t2, 4($t0)
addi $t3, $0, 2
sw $t3, 8($t0)
addi $t4, $0, 3
sw $t4, 12($t0)
addi $t5, $0, 4
sw $t5, 16($t0)
addi $t6, $0, 5
sw $t6, 20($t0)
lw $t1, 20($t0)
li $v0, 1
add $a0, $t1, $0
syscall
add $t1, $0, $0
la $t2, struct6
addi $t3, $0, 0
sw $t3, 0($t2)
la $t4, string7
sw $t4, 4($t2)
la $t5, string8
sw $t5, 8($t2)
la $t6, string9
sw $t6, 12($t2)
lw $t3, 12($t2)
li $v0, 4
add $a0, $t3, $0
syscall
add $t3, $0, $0
la $t4, struct7
addi $t5, $0, 0
sw $t5, 0($t4)
addi $t6, $0, 1
sw $t6, 4($t4)
addi $t7, $0, 2
sw $t7, 8($t4)
addi $t8, $0, 3
sw $t8, 12($t4)
addi $t9, $0, 4
sw $t9, 16($t4)
addi $t0, $0, 5
sw $t0, 20($t4)
lw $t5, 0($t4)
li $v0, 1
add $a0, $t5, $0
syscall
add $t5, $0, $0
la $t6, struct8
addi $t7, $0, 0
sw $t7, 0($t6)
addi $t8, $0, 1
sw $t8, 4($t6)
addi $t9, $0, 2
sw $t9, 8($t6)
addi $t0, $0, 3
sw $t0, 12($t6)
addi $t1, $0, 4
sw $t1, 16($t6)
addi $t2, $0, 5
sw $t2, 20($t6)
lw $t7, 4($t6)
li $v0, 1
add $a0, $t7, $0
syscall
add $t7, $0, $0
la $t8, struct9
addi $t9, $0, 0
sw $t9, 0($t8)
addi $t0, $0, 1
sw $t0, 4($t8)
addi $t1, $0, 2
sw $t1, 8($t8)
addi $t2, $0, 3
sw $t2, 12($t8)
lw $t9, 8($t8)
li $v0, 1
add $a0, $t9, $0
syscall
add $t9, $0, $0
la $t0, struct10
addi $t1, $0, 0
sw $t1, 0($t0)
addi $t2, $0, 1
sw $t2, 4($t0)
addi $t3, $0, 2
sw $t3, 8($t0)
addi $t4, $0, 3
sw $t4, 12($t0)
addi $t5, $0, 4
sw $t5, 16($t0)
addi $t6, $0, 5
sw $t6, 20($t0)
lw $t1, 12($t0)
li $v0, 1
add $a0, $t1, $0
syscall
add $t1, $0, $0
la $t2, struct11
addi $t3, $0, 0
sw $t3, 0($t2)
addi $t4, $0, 1
sw $t4, 4($t2)
addi $t5, $0, 2
sw $t5, 8($t2)
addi $t6, $0, 3
sw $t6, 12($t2)
addi $t7, $0, 4
sw $t7, 16($t2)
lw $t3, 16($t2)
li $v0, 1
add $a0, $t3, $0
syscall
add $t3, $0, $0
la $t4, struct12
addi $t5, $0, 0
sw $t5, 0($t4)
addi $t6, $0, 1
sw $t6, 4($t4)
addi $t7, $0, 2
sw $t7, 8($t4)
addi $t8, $0, 3
sw $t8, 12($t4)
addi $t9, $0, 4
sw $t9, 16($t4)
addi $t0, $0, 5
sw $t0, 20($t4)
lw $t5, 20($t4)
li $v0, 1
add $a0, $t5, $0
syscall
add $t5, $0, $0
    li $v0 10
    syscall
