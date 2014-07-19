.data
string0: .asciiz "\n"
x: .word 0
.text
la $t0, x
addi $t1, $0, 10
sw $t1, 0($t0)
add $t2, $0, $0
label0: 
la $t3, x
la $t4, x
lw $t4, 0($t4)
addi $t5, $0, 1
sub $t6, $t4, $t5
sw $t6, 0($t3)
bne $t6, $0, label1
j label2
label1: 
la $t7, x
lw $t7, 0($t7)
la $t8, string0
li $v0, 4
add $a0, $t8, $0
syscall
add $t8, $0, $0
add $t9, $t7, $t8
li $v0, 1
add $a0, $t9, $0
syscall
add $t9, $0, $0
j label0
label2: 
    li $v0 10
    syscall
