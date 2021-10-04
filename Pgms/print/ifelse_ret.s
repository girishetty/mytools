	.file	"ifelse_ret.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"val is: %d"
	.text
	.p2align 4,,15
	.globl	if_else_test
	.type	if_else_test, @function
if_else_test:
.LFB0:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	cmpl	$1, %edi
	je	.L5
.L3:
.L1:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	.cfi_restore_state
	cmpl	$1, %esi
	jne	.L3
	cmpl	$1, %edx
	.p2align 4,,2
	jne	.L3
	movb	$5, %sil
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movl	$5, %eax
	jmp	.L1
	.cfi_endproc
.LFE0:
	.size	if_else_test, .-if_else_test
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
