	.file	"syscall.c"
	.section	.rodata
.LC0:
	.string	"syscall done."
.LC1:
	.string	"cse231"
.LC2:
	.string	"/dev/%s"
.LC3:
	.string	"unable to open dev"
.LC4:
	.string	"ioctl REGISTER_SYACALL error"
.LC5:
	.string	"syscall done!"
.LC6:
	.string	"syscall failed!"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$276, %esp
	movl	%ecx, %eax
	movl	4(%eax), %eax
	movl	%eax, -268(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	movl	$3, -264(%ebp)
	movl	$.LC0, -260(%ebp)
	pushl	$.LC1
	pushl	$.LC2
	pushl	$100
	leal	-240(%ebp), %eax
	pushl	%eax
	call	snprintf
	addl	$16, %esp
	movl	%eax, -256(%ebp)
	leal	-240(%ebp), %edx
	movl	-256(%ebp), %eax
	addl	%edx, %eax
	movb	$0, (%eax)
	subl	$8, %esp
	pushl	$0
	leal	-240(%ebp), %eax
	pushl	%eax
	call	open
	addl	$16, %esp
	movl	%eax, -252(%ebp)
	cmpl	$0, -252(%ebp)
	jns	.L2
	subl	$12, %esp
	pushl	$.LC3
	call	perror
	addl	$16, %esp
	movl	$0, %eax
	jmp	.L8
.L2:
	subl	$4, %esp
	pushl	$0
	pushl	$-2147195903
	pushl	-252(%ebp)
	call	ioctl
	addl	$16, %esp
	testl	%eax, %eax
	je	.L4
	subl	$12, %esp
	pushl	$.LC4
	call	perror
	addl	$16, %esp
	call	abort
.L4:
	leal	-140(%ebp), %eax
	movl	$746153288, (%eax)
	movl	$1701339936, 4(%eax)
	movl	$1970236259, 8(%eax)
	movl	$2037194868, 12(%eax)
	movl	$2003136032, 16(%eax)
	movl	$1937339168, 20(%eax)
	movl	$544040308, 24(%eax)
	movl	$1819042147, 28(%eax)
	movw	$33, 32(%eax)
	movl	$33, -256(%ebp)
	leal	-140(%ebp), %edx
	movl	-256(%ebp), %eax
	addl	%edx, %eax
	movb	$0, (%eax)
	leal	-140(%ebp), %eax
	movl	%eax, -248(%ebp)
	movl	-256(%ebp), %eax
	movl	%eax, -244(%ebp)
	subl	$8, %esp
	leal	-248(%ebp), %eax
	pushl	%eax
	pushl	-264(%ebp)
	call	syscall_u
	addl	$8, %esp
	movl	-244(%ebp), %eax
	subl	$4, %esp
	pushl	%eax
	pushl	-260(%ebp)
	leal	-140(%ebp), %eax
	pushl	%eax
	call	memcmp
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L5
	subl	$12, %esp
	pushl	$.LC5
	call	puts
	addl	$16, %esp
	jmp	.L6
.L5:
	subl	$12, %esp
	pushl	$.LC6
	call	puts
	addl	$16, %esp
.L6:
	subl	$4, %esp
	pushl	$0
	pushl	$-2147195902
	pushl	-252(%ebp)
	call	ioctl
	addl	$16, %esp
	testl	%eax, %eax
	je	.L7
	subl	$12, %esp
	pushl	$.LC4
	call	perror
	addl	$16, %esp
	call	abort
.L7:
	subl	$12, %esp
	pushl	-252(%ebp)
	call	close
	addl	$16, %esp
	movl	$0, %eax
.L8:
	movl	-12(%ebp), %ecx
	xorl	%gs:20, %ecx
	je	.L9
	call	__stack_chk_fail
.L9:
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
