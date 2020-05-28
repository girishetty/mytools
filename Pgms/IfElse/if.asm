	.globl	__Z12AddContacts_P14ContactDetails
	.align	4, 0x90
__Z12AddContacts_P14ContactDetails:     ## @_Z12AddContacts_P14ContactDetails
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp3:
	.cfi_def_cfa_offset 16
Ltmp4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp5:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	$2, -12(%rbp)
	cmpq	$0, -8(%rbp)
	je	LBB1_8
## BB#1:
	movl	$8, %eax
	movl	%eax, %edi
	movl	$1, -12(%rbp)
	callq	__Znwm
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	je	LBB1_7
## BB#2:
	movl	$2, -12(%rbp)
	movq	-8(%rbp), %rax
	cmpq	$0, (%rax)
	je	LBB1_6
## BB#3:
	movl	$1, -12(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdi
	callq	_strlen
	movq	%rax, %rdi
	callq	__Znam
	movq	-24(%rbp), %rdi
	movq	%rax, (%rdi)
	movq	-24(%rbp), %rax
	cmpq	$0, (%rax)
	je	LBB1_5
## BB#4:
	movq	-24(%rbp), %rax
	movq	(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	(%rax), %rsi
	callq	_strcpy
	movl	$0, -12(%rbp)
	movq	%rax, -32(%rbp)         ## 8-byte Spill
LBB1_5:
	jmp	LBB1_6
LBB1_6:
	jmp	LBB1_7
LBB1_7:
	jmp	LBB1_8
LBB1_8:
	movl	-12(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
