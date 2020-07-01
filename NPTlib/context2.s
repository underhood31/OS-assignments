.text
.globl context_switch

context_switch:
	push %ebx
	push %esi
	push %edi
	push %ebp
	mov 20(%esp), %eax       # prev
	mov 24(%esp), %ecx       # next
	mov %esp, (%eax)        # prev->esp = esp
	mov (%ecx), %esp        # esp = next->esp
	pop %ebp
	pop %edi
	pop %esi
	pop %ebx
	ret 
	
	
	
	.text
.globl context_switch

context_switch:
	push %ebx
	push %esi
	push %edi
	push %ebp
	mov 28(%esp), %ebx      #later
	mov $0, %esi
	cmp %ebx, %esi             #later
	jne skip1               #later
	mov 20(%esp), %eax       # prev
skip1:                      #later
	mov 24(%esp), %ecx       # next
	cmp %ebx, %esi             #later
	jne skip2               #later
	mov %esp, (%eax)        # prev->esp = esp
skip2:
	mov (%ecx), %esp        # esp = next->esp
	pop %ebp
	pop %edi
	pop %esi
	pop %ebx
	ret
