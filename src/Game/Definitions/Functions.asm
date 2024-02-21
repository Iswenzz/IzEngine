; Exports
	global BG_EvaluateTrajectory
	global CG_TracePoint
	global Dvar_FindVar
	global R_AddCmdDrawText
	global R_AddCmdDrawStretchPic
	global R_TextWidth

SECTION .text

; void BG_EvaluateTrajectory(const trajectory_t* tr, int atTime, float* out);
BG_EvaluateTrajectory:
	push	ebp
	mov		ebp, esp
	pushad
	mov		ecx, dword [ebp + 10h]	; out
	mov		eax, dword [ebp + 0Ch]	; atTime
	push	dword [ebp + 8]			; tr
	call	dword [BG_EvaluateTrajectory_a]
	add		esp, 4
	popad
	pop		ebp
	ret

; dvar_s* Dvar_FindVar(const char* name);
Dvar_FindVar:
	push	ebp
	mov		ebp, esp
	pushad
	mov		edi, dword [ebp + 8]	; name
	call	dword [Dvar_FindVar_a]
	mov		[esp + 1Ch], eax
	popad
	pop		ebp
	ret

; void R_AddCmdDrawText(const char* text, int maxChars, Font_s* font, float x, float y,
;	float xScale, float yScale, float rotation, int style, float* color);
R_AddCmdDrawText:
	push    ebp
    mov     ebp, esp
    pushad
    mov     ecx, dword [ebp + 2Ch]       ; color
    push    dword [ebp + 28h]            ; style
    push    dword [ebp + 24h]            ; rotation
    push    dword [ebp + 20h]            ; yScale
    push    dword [ebp + 1Ch]            ; xScale
    push    dword [ebp + 18h]            ; y
    push    dword [ebp + 14h]            ; x
    push    dword [ebp + 10h]            ; font
    push    dword [ebp + 0Ch]            ; maxChars
    push    dword [ebp + 8]              ; text
    call    dword [R_AddCmdDrawText_a]
    add     esp, 24h
    popad
    pop     ebp
    ret

; void R_AddCmdDrawStretchPic(Material* material, float x, float y, float w, float h,
;	float null1, float null2, float null3, float null4, float* color);
R_AddCmdDrawStretchPic:
	push	ebp
	mov		ebp, esp
	pushad
	push	dword [ebp + 2Ch]		; color
	push	dword [ebp + 28h]		; null4
	push	dword [ebp + 24h]		; null3
	push	dword [ebp + 20h]		; null2
	push	dword [ebp + 1Ch]		; null1
	push	dword [ebp + 18h]		; h
	push	dword [ebp + 14h]		; w
	push	dword [ebp + 10h]		; y
	push	dword [ebp + 0Ch]		; x
	mov		eax, dword [ebp + 8]	; material
	call	dword [R_AddCmdDrawStretchPic_a]
	add		esp, 24h
	popad
	pop		ebp
	ret

; int R_TextWidth(const char* text, int maxChars, Font_s* font);
R_TextWidth:
	push	ebp
	mov		ebp, esp
	pushad
	push	dword [ebp + 10h]		; font
	push	dword [ebp + 0Ch]		; maxChars
	mov		eax, dword [ebp + 8]	; text
	call	dword [R_TextWidth_a]
	mov		[esp + 24h], eax
	add		esp, 8
	popad
	pop		ebp
	ret

SECTION .rdata

	BG_EvaluateTrajectory_a: dd 40BD70h
	CG_TracePoint_a: dd 459EF0h
    Dvar_FindVar_a: dd 56B5D0h
    R_AddCmdDrawText_a: dd 5F6B00h
    R_AddCmdDrawStretchPic_a: dd 5F65F0h
    R_TextWidth_a: dd 5F1EE0h
