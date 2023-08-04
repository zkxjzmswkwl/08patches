.code
	ALIGN 16

	GetNtGlobal PROC
		sub rsp, 28h
		xor rax, rax
		mov rax, gs:[60h]       
		mov rax, [rax + 0BCh]
		and rax, 70h
		add rsp, 28h
		ret
	GetNtGlobal ENDP

	SetNtGlobal PROC
		sub rsp, 28h
		mov rax, gs:[60h]
		mov [rax + 0BCh], rcx
		add rsp, 28h
		ret
	SetNtGlobal ENDP

END