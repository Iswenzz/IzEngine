; Exports
	global R_AddCmdDrawText

SECTION .text

; const char* R_AddCmdDrawText(const char* text, int maxChars, Font_s* font, float x, float y,
;	float xScale, float yScale, float rotation, int style, float* color);
R_AddCmdDrawText:
	push	ebp
	mov		ebp, esp
	push	dword [ebp + 28h]			; style
	sub		esp, 14h
	fld		dword [ebp + 24h]			; rotation
	fstp	dword [esp + 10h]
	fld		dword [ebp + 20h]			; yScale
	fstp	dword [esp + 0Ch]
	fld		dword [ebp + 1Ch]			; xScale
	fstp	dword [esp + 8]
	fld		dword [ebp + 18h]			; y
	fstp	dword [esp + 4]
	fld		dword [ebp + 14h]			; x
	fstp	dword [esp]
	push	dword [ebp + 10h]			; font
	push	dword [ebp + 0Ch]			; maxChars
	push	dword [ebp + 8]				; text
	mov		ecx, dword [ebp + 2Ch]		; color
	call	dword [R_AddCmdDrawText_a]
	add		esp, 24h
	cmp		ebp, esp
	pop		ebp
	ret

SECTION .rdata

    R_AddCmdDrawText_a: dd 5F6B00h
