.data
string0: .asciiz "hello world"
.text
la $t0, string0
li $v0, 4
add $a0, $t0, $0
syscall
add $t0, $0, $0
    li $v0 10
    syscall
