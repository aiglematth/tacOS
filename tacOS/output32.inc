;Auteur --> aiglematth

get_cursor:
	;Permet de trouver où est le dernier char affiché
	;Retour : 
	;AL --> Colonne
	;AH --> Ligne
	
	push	ebp
	mov	ebp,esp
	
	pusha
	
	mov	eax,80
	mov	ecx,0xfa0
	
	.loop:
		mov	bx,word [screenBuffer + ecx]
		cmp	bl,0
		jne	.endLoop
		dec	ecx
		dec	ecx
		
	.endLoop:
		inc	ecx
	
	;AL = ECX % 80 AH = ECX // 80
	div	ecx
	mov	ah,al
	mov	al,dl
	
	popa
	
	leave
	ret

print:
	;ESI --> Adresse de screenBuffer
	;EDI --> adresse chaine de chars
	;DL  --> couleur de fond
	;DH  --> couleur de police
	;AL --> Colonne
	;AH --> Ligne
	
	call	create_good_color
	
	;Initialisation des registres
	xor	ecx,ecx
	
	.loopPrint:
		mov	bx,[edi + ecx]
		and	bx,255
		inc	ecx
		cmp	bx,0
		je	.endPrint
		cmp	bx,0xa
		je	.newLine
		jmp	.afficher
		
		.newLine:
			inc	ah
			xor	al,al
			jmp	.loopPrint
			
		.afficher:
			call	print_char
			inc	al
			jmp	.loopPrint
	
	
	.endPrint:
		nop
	ret
