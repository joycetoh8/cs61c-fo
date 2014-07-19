.data
.text
addi $t0, $0, 5
move $a0, $t0
jal func_fib
move $t1, $v0
li $v0, 1
add $a0, $t1, $0
syscall
add $t1, $0, $0
    li $v0 10
    syscall
