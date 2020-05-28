	.globl	__Z11AddContactsP14ContactDetails
	.align	4, 0x90
__Z11AddContactsP14ContactDetails:      ## @_Z11AddContactsP14ContactDetails
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	$0, -12(%rbp)
	cmpq	$0, -8(%rbp)
	je	LBB0_11
## BB#1:
	movl	$8, %eax
	movl	%eax, %edi
	callq	__Znwm
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	je	LBB0_9
## BB#2:
	movq	-8(%rbp), %rax
	cmpq	$0, (%rax)
	je	LBB0_7
## BB#3:
	movq	-8(%rbp), %rax
	movq	(%rax), %rdi
	callq	_strlen
	movq	%rax, %rdi
	callq	__Znam
	movq	-24(%rbp), %rdi
	movq	%rax, (%rdi)
	movq	-24(%rbp), %rax
	cmpq	$0, (%rax)
	je	LBB0_5
## BB#4:
	movq	-24(%rbp), %rax
	movq	(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	(%rax), %rsi
	callq	_strcpy
	movq	%rax, -32(%rbp)         ## 8-byte Spill
	jmp	LBB0_6
LBB0_5:
	movl	$1, -12(%rbp)
LBB0_6:
	jmp	LBB0_8
LBB0_7:
	movl	$2, -12(%rbp)
LBB0_8:
	jmp	LBB0_10
LBB0_9:
	movl	$1, -12(%rbp)
LBB0_10:
	jmp	LBB0_12
LBB0_11:
	movl	$2, -12(%rbp)
LBB0_12:
	movl	-12(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
