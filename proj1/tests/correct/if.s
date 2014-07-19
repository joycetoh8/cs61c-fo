.data
string3: .asciiz "success\n"
string2: .asciiz "fail\n"
string2: .asciiz "fail\n"
string3: .asciiz "success\n"
.text
addi $t0, $0, 1
bne $t0, $0, label0
la $t2, string2
li $v0, 4
add $a0, $t2, $0
syscall
add $t2, $0, $0
move $t1, $t2
j label1
label0: 
la $t3, string3
li $v0, 4
add $a0, $t3, $0
syscall
add $t3, $0, $0
move $t1, $t3
label1: 
addi $t2, $0, 0
bne $t2, $0, label2
la $t4, string3
li $v0, 4
add $a0, $t4, $0
syscall
add $t4, $0, $0
move $t3, $t4
j label3
label2: 
la $t5, string2
li $v0, 4
add $a0, $t5, $0
syscall
add $t5, $0, $0
move $t3, $t5
label3: 
    li $v0 10
    syscall
