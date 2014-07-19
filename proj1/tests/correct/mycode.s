.data
string0: .asciiz "A truly nasty example of scoping in LIFC."
string1: .asciiz "Credit for it goes to Ulysse Manceron."
mycode: .word 0
.text
la $t0, string0
la $t1, string1
la $t2, mycode
addi $t3, $0, 21
sw $t3, 0($t2)
la $t4, mycode
lw $t4, 0($t4)
addi $t5, $0, 2
mult $t4, $t5
mflo $t6
move $a0, $t6
jal func_mycode
move $t7, $v0
    li $v0 10
    syscall
