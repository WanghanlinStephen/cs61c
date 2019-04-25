.data

lfsr:
	.align 4
	.word 0x1

.text

# Implements a 16-bit lfsr
#
# Arguments: None
lfsr_random:
	la t0 lfsr
	lhu a0 0(t0)

	# Your Code Here
	add t1 zero zero
	addi t2 zero 16
loop:
	bge t1 t2 end
	srli t3 a0 0
	srli t4 a0 2
	xor t3 t3 t4
	srli t4 a0 3
	xor t3 t3 t4
	srli t4 a0 5
	xor t3 t3 t4
	srli a0 a0 1
	slli t3 t3 15
	or a0 a0 t3
	slli a0 a0 16
	srli a0 a0 16
	addi t1 t1 1
	jal zero loop
end:
	la t0 lfsr
	sh a0 0(t0)
	jr ra
