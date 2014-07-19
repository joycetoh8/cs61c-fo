.data
x: .word 0
y: .word 0
.text
la $t0, x
addi $t1, $0, 5
sw $t1, 0($t0)
la $t2, x
lw $t2, 0($t2)
li $v0, 1
add $a0, $t2, $0
syscall
add $t2, $0, $0
la $t3, y
la $t4, x
la $t5, x
lw $t5, 0($t5)
addi $t6, $0, 1
add $t7, $t5, $t6
sw $t7, 0($t4)
sw $t7, 0($t3)
la $t8, x
lw $t8, 0($t8)
li $v0, 1
add $a0, $t8, $0
syscall
add $t8, $0, $0
la $t9, y
lw $t9, 0($t9)
li $v0, 1
add $a0, $t9, $0
syscall
add $t9, $0, $0
    li $v0 10
    syscall
