.text

# Decodes a quadtree to the original matrix
#
# Arguments:
#     quadtree (qNode*)
#     matrix (void*)
#     matrix_width (int)
#
# Recall that quadtree representation uses the following format:
#     struct qNode {
#         int leaf;
#         int size;
#         int x;
#         int y;
#         int gray_value;
#         qNode *child_NW, *child_NE, *child_SE, *child_SW;
#     }

quad2matrix:
	# Your code here
	addi sp sp -36
	sw ra 0(sp)
	sw s0 -4(sp) # quadtree
	sw s1 -8(sp) # matrix
	sw s2 -12(sp) # matrix_width
	sw s3 -16(sp) # qNode->gray_value
	sw s4 -20(sp) # x + size
	sw s5 -24(sp) # y + size
	sw s6 -28(sp) # current_x
	sw s7 -32(sp) # current_y
	add s0 a0 zero
	add s1 a1 zero
	add s2 a2 zero
	beq s0 zero done
	lw t0 0(s0)
	beq t0 zero else
	lw s3 16(s0)
	lw t1 4(s0)
	lw s5 12(s0)
	add s7 s5 zero
	add s5 s5 t1
out_loop:
	bge s7 s5 done
	lw t1 4(s0)
	lw s4 8(s0)
	add s6 s4 zero
	add s4 s4 t1
inner_loop:
	bge s6 s4 inner_end
	add a0 s2 zero
	add a1 s6 zero
	add a2 s7 zero
	jal ra get_index
	add a0 a0 s1
	sb s3 0(a0)
	addi s6 s6 1
	jal zero inner_loop
inner_end:
	addi s7 s7 1
	jal zero out_loop
else:
    lw a0 20(s0) # *child_NW
	add a1 s1 zero
	add a2 s2 zero
	jal ra quad2matrix
    lw a0 24(s0) # *child_NE
	add a1 s1 zero
	add a2 s2 zero
	jal ra quad2matrix
    lw a0 28(s0) # *child_SE
	add a1 s1 zero
	add a2 s2 zero
	jal ra quad2matrix
    lw a0 32(s0) # *child_SW
	add a1 s1 zero
	add a2 s2 zero
	jal ra quad2matrix
done:
	lw ra 0(sp)
	lw s0 -4(sp) # quadtree
	lw s1 -8(sp) # matrix
	lw s2 -12(sp) # matrix_width
	lw s3 -16(sp) # qNode->gray_value
	lw s4 -20(sp) # x + size
	lw s5 -24(sp) # y + size
	lw s6 -28(sp) # current_x
	lw s7 -32(sp) # current_y
	addi sp sp 36
	jr ra

# Arguments:
#     matrix_width (int)
#     x (int)
#     y (int)
#
# Returns:
#     index (int)
get_index:
    mul a0 a0 a2
	add a0 a0 a1
	jr ra