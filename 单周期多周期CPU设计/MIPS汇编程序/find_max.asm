#寻找数组中的最大值并保存在max
.data
	array: .word 5,-4,67,-31,0,1,100
	max: .word 0
.text
main:
	la $t0,array  #$t0保存了array开始的值
	lw $t1,array  #假设$t1保存的就是最大值
	li $t2,0      #$t2是遍历数据的游标
	li $t5,6      #6是数组的边界
loop:
	addi $t2,$t2,1
	addi $t3,$t0,$t2  #$t3是下一个待比较的数据在的位置
	lw $t4,($t3)      #$t4是即将和$t1比较的数据
	ble $t4,$t1,next
	#$t4比$t1大
	move $t1,$t4
next:
	beq $t2,$t5,save
	j loop
	
save:
	sw $t1,7($t0)     #把最大值保存在max里