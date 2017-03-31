	.text
	.globl  My_compare
	
My_compare:
	pushq   %rbp
	movq    %rsp, %rbp
	movq    %rdi, -8(%rbp)
	movl    %esi, -12(%rbp)
	movl    %edx, -16(%rbp)
	movl    -16(%rbp), %ecx
	movl    -12(%rbp), %eax
	movq    -8(%rbp), %rdx
	lock	cmpxchgl   %ecx, (%rdx)
	sete    %al
	movzbl  %al, %eax
	popq    %rbp
	ret
