#include <linux/linkage.h>

# syscall_handler_k routine in klib.c implements the actual system
# call handler. The arguments to syscall_handler_k are passed on the
# stack. Kernel's %fs segment register value is 216 that may be different
# from user applications. Kernel doesn't use %gs segment register.
# %ds and %es are the same for user and kernel.

#void syscall_handler_k(int id, void *arg);
# Save the caller-saved registers.
# Save the user's %fs register.
# set %fs to 216.
# pass user's arguments to syscall_handler_k
# restore %fs and caller-saved registers.
# return to user-mode.

#system call handler
ENTRY(syscall_handler)
	pushl %ebp
	movl %esp, %ebp
	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %fs
	movl $216, %ecx
	movl %ecx, %fs
	sub $8, %esp
	mov %edx, 4(%esp)
	mov %eax, (%esp)
	
	call syscall_handler_k

	add $8, %esp	
	popl %fs
	popl %edx
	popl %ecx
	popl %eax
	movl %ebp, %esp
	popl %ebp
	iret
ENDPROC(syscall_handler)
