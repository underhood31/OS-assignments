
# syscall_u(int syscall_id, void *ptr);

#	syscall_u takes syscall_id and a
#	user buffer as argument. syscall passes the 
#	syscall_id and buffer to the syscall_handler
#	present at vector 15 in the IDT.
	
.text
.global syscall_u
.type syscall_u, @function
syscall_u:

	push %ebp
	mov %esp, %ebp
	movl 8(%ebp), %eax
	movl 12(%ebp), %ecx
	int $15
	mov %ebp, %esp
	pop %ebp

# probably you don't need to write
# anything here, but if your logic
# demands, please feel free to do it.

ret

.size syscall_u, .-syscall_u
