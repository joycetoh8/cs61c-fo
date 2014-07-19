.data
.text
addi $t0, $0, 3
addi $t1, $0, 6
add $t2, $t0, $t1
li $v0, 1
add $a0, $t2, $0
syscall
add $t2, $0, $0
addi $t3, $0, 3
addi $t4, $0, 6
sub $t5, $t3, $t4
li $v0, 1
add $a0, $t5, $0
syscall
add $t5, $0, $0
addi $t6, $0, 6
addi $t7, $0, 3
div $t8, $t6, $t7
li $v0, 1
add $a0, $t8, $0
syscall
add $t8, $0, $0
addi $t9, $0, 3
addi $t0, $0, 6
mult $t9, $t0
mflo $t1
li $v0, 1
add $a0, $t1, $0
syscall
add $t1, $0, $0
    li $v0 10
    syscall
