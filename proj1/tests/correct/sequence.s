.data
string0: .asciiz "\n"
.text
addi $t0, $0, 0
li $v0, 1
add $a0, $t0, $0
syscall
add $t0, $0, $0
la $t1, string0
li $v0, 4
add $a0, $t1, $0
syscall
add $t1, $0, $0
addi $t2, $0, 3
li $v0, 1
add $a0, $t2, $0
syscall
add $t2, $0, $0
    li $v0 10
    syscall
