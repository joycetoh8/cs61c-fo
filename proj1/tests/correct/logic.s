.data
.text
addi $t0, $0, 0
beq $t0, $0, label0
addi $t1, $0, 0
beq $t1, $0, label0
addi $t2, $0, 1
j label1
label0: addi $t2, $0, 0
label1: 
li $v0, 1
add $a0, $t2, $0
syscall
add $t2, $0, $0
addi $t3, $0, 0
beq $t3, $0, label2
addi $t4, $0, 2
beq $t4, $0, label2
addi $t5, $0, 1
j label3
label2: addi $t5, $0, 0
label3: 
li $v0, 1
add $a0, $t5, $0
syscall
add $t5, $0, $0
addi $t6, $0, 3
beq $t6, $0, label4
addi $t7, $0, 0
beq $t7, $0, label4
addi $t8, $0, 1
j label5
label4: addi $t8, $0, 0
label5: 
li $v0, 1
add $a0, $t8, $0
syscall
add $t8, $0, $0
addi $t9, $0, 4
beq $t9, $0, label6
addi $t0, $0, 5
beq $t0, $0, label6
addi $t1, $0, 1
j label7
label6: addi $t1, $0, 0
label7: 
li $v0, 1
add $a0, $t1, $0
syscall
add $t1, $0, $0
addi $t2, $0, 0
bne $t2, $0, label8
addi $t3, $0, 0
bne $t3, $0, label8
addi $t4, $0, 0
j label9
label8: addi $t4, $0, 1
label9: 
li $v0, 1
add $a0, $t4, $0
syscall
add $t4, $0, $0
addi $t5, $0, 0
bne $t5, $0, label10
addi $t6, $0, 2
bne $t6, $0, label10
addi $t7, $0, 0
j label11
label10: addi $t7, $0, 1
label11: 
li $v0, 1
add $a0, $t7, $0
syscall
add $t7, $0, $0
addi $t8, $0, 3
bne $t8, $0, label12
addi $t9, $0, 0
bne $t9, $0, label12
addi $t0, $0, 0
j label13
label12: addi $t0, $0, 1
label13: 
li $v0, 1
add $a0, $t0, $0
syscall
add $t0, $0, $0
addi $t1, $0, 4
bne $t1, $0, label14
addi $t2, $0, 5
bne $t2, $0, label14
addi $t3, $0, 0
j label15
label14: addi $t3, $0, 1
label15: 
li $v0, 1
add $a0, $t3, $0
syscall
add $t3, $0, $0
    li $v0 10
    syscall
