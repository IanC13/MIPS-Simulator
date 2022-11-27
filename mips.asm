addi $s0,$zero,2 # less than condition
addi $s1,$zero,1 # odd condition
addi $s6,$zero,0 #(starting number we are squaring from, 0)
addi $s7,$zero,99 #(last number we are squaring)
main:
	add $s2, $zero,$s6 # number we are squaring as argument
	add $s5,$zero,$s6 # Intermediate variable
	
	addi $sp,$sp,-4 #stack pointer adjust
	sw $ra,($sp) # store stack pointer
	jal square
	
	bne $s5,$s7, continue # check if we have squared 99 numbers yet
	
	#li $v1,10
	#syscall

continue:
	addi $s6,$s6,1
	j main # square next number 
	
square:
	blt $s2,$s0,lessThanTwo # if(num < 2)
	srl $s3,$s2,1 # i = num >> 1
	
	#check if odd
	andi $t0,$s2,1 # num & 1
	beq $t0,$s1,odd # checks if the number is odd and branches to that block if true
	j even # otherwise it is an even number and branch to that block
even:
	add $s2,$zero,$s3
	
	addi $sp,$sp,-4
	sw $ra,($sp)
	jal square
	
	sll $v0,$v0,2
	
	lw $ra,($sp)
	addi $sp,$sp,4
	jr $ra
	

lessThanTwo:
	add $v0,$zero,$s2 # The final result is stored in register $v0
	
	lw $ra,($sp)
	addi $sp,$sp,4
	jr $ra

odd:	
	add $s4,$zero,$s3
	sll $s4,$s4,2 # i << 2
	
	addi $sp,$sp,-4
	sw $s4,($sp)
	
	add $s2,$zero,$s3 # make num = i
	
	addi $sp,$sp,-4
	sw $ra,($sp)
	jal square
	
	lw $s4,($sp)
	addi $sp,$sp,4
	
	sll $v0,$v0,2 
	add $v0,$s4,$v0
	addi $v0,$v0,1
	
	lw $ra,($sp)
	addi $sp,$sp,4
	jr $ra
	