#Ѱ�������е����ֵ��������max
.data
	array: .word 5,-4,67,-31,0,1,100
	max: .word 0
.text
main:
	la $t0,array  #$t0������array��ʼ��ֵ
	lw $t1,array  #����$t1����ľ������ֵ
	li $t2,0      #$t2�Ǳ������ݵ��α�
	li $t5,6      #6������ı߽�
loop:
	addi $t2,$t2,1
	addi $t3,$t0,$t2  #$t3����һ�����Ƚϵ������ڵ�λ��
	lw $t4,($t3)      #$t4�Ǽ�����$t1�Ƚϵ�����
	ble $t4,$t1,next
	#$t4��$t1��
	move $t1,$t4
next:
	beq $t2,$t5,save
	j loop
	
save:
	sw $t1,7($t0)     #�����ֵ������max��