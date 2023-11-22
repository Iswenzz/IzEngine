; Exports
	global CG_TracePoint
	global Dvar_FindVar
	global R_AddCmdDrawText
	global R_AddCmdDrawStretchPic
	global R_TextWidth

SECTION .text

; void CG_TracePoint(pmove_t* pm, trace_t* trace, const float* start,
;	const float* mins, const float* maxs, const float* end, int passEntityNum, int contentMask);
CG_TracePoint:
	push	ebp
	mov		ebp, esp
	push	esi
	push	dword [ebp + 24h]		; contentMask
	push	dword [ebp + 20h]		; passEntityNum
	push	dword [ebp + 1Ch]		; end
	push	dword [ebp + 18h]		; maxs
	push	dword [ebp + 14h]		; mins
	push	dword [ebp + 10h]		; start
	push	dword [ebp + 0Ch]		; trace
	mov		esi, dword [ebp + 8]	; pm
	call	dword [CG_TracePoint_a]
	add		esp, 1Ch
	pop		esi
	pop		ebp
	ret

; dvar_s* Dvar_FindVar(const char* name);
Dvar_FindVar:
	push	ebp
	mov		ebp, esp
	push	edi
	mov		edi, dword [ebp + 8]	; name
	call	dword [Dvar_FindVar_a]
	pop		edi
	pop		ebp
	ret

; void R_AddCmdDrawText(const char* text, int maxChars, Font_s* font, float x, float y,
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
	pop		ebp
	ret

; void R_AddCmdDrawStretchPic(Material* material, float x, float y, float w, float h,
;	float null1, float null2, float null3, float null4, float* color);
R_AddCmdDrawStretchPic:
	push	ebp
	mov		ebp, esp
	push	ebx
	push	esi
	push	edi
	push	dword [ebp + 2Ch]		; color
	mov		eax, dword [ebp + 8]	; material
	sub		esp, 20h
	fld		dword [ebp + 28h]		; null4
	fstp	dword [esp + 1Ch]
	fld		dword [ebp + 24h]		; null3
	fstp	dword [esp + 18h]
	fld		dword [ebp + 20h]		; null2
	fstp	dword [esp + 14h]
	fld		dword [ebp + 1Ch]		; null1
	fstp	dword [esp + 10h]
	fld		dword [ebp + 18h]		; h
	fstp	dword [esp + 0Ch]
	fld		dword [ebp + 14h]		; w
	fstp	dword [esp + 8]
	fld		dword [ebp + 10h]		; y
	fstp	dword [esp + 4]
	fld		dword [ebp + 0Ch]		; x
	fstp	dword [esp]
	call	dword [R_AddCmdDrawStretchPic_a]
	add		esp, 24h
	pop		edi
	pop		esi
	pop		ebx
	pop		ebp
	ret

; int R_TextWidth(const char* text, int maxChars, Font_s* font);
R_TextWidth:
	push	ebp
	mov		ebp, esp
	push	dword [ebp + 10h]		; font
	push	dword [ebp + 0Ch]		; maxChars
	mov		eax, dword [ebp + 8]	; text
	call	dword [R_TextWidth_a]
	add		esp, 8
	pop		ebp
	ret

SECTION .rdata

	CG_TracePoint_a: dd 20A3E1CCh
    Dvar_FindVar_a: dd 56B5D0h
    R_AddCmdDrawText_a: dd 5F6B00h
    R_AddCmdDrawStretchPic_a: dd 5F65F0h
    R_TextWidth_a: dd 5F1EE0h
