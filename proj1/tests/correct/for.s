.data
i: .word 0
.text
la $t0, i
addi $t1, $0, 0
sw $t1, 0($t0)
add $t2, $0, $0
addi $t3, $0, 0
label0: 
la $t4, i
lw $t4, 0($t4)
addi $t5, $0, 10
slt $t6, $t4, $t5
bne $t6, $0, label1
j label2
label1: 
la $t7, i
lw $t7, 0($t7)
li $v0, 1
add $a0, $t7, $0
syscall
add $t7, $0, $0
la $t8, i
la $t9, i
lw $t9, 0($t9)
addi $t0, $0, 1
add $t1, $t9, $t0
sw $t1, 0($t8)
j label0
label2: 
    li $v0 10
    syscall
