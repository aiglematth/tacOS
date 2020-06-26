;Auteur --> aiglematth

clear:
	;DX --> Couleur de fond
	mov	esi,screenBuffer
	shl	dx,12
	
	.loopClear:
		mov	[esi],dx
		add	esi,2
		cmp	esi,0xb8fa0
		jl	.loopClear
	
	ret

create_good_color:
	;DL --> couleur de fond
	;DH --> couleur de police
	;Return DL avec bon assemblage
	
	shl	dl,4
	or	dl,dh
	
	ret

print_char:
	;ESI --> Adresse du screenBuffer
	;AL  --> Colonne
	;AH  --> Ligne
	;BX  --> valeur du char à afficher
	;DL  --> couleurs de fond et de police (resultat de create_good_color)
	
	xor	bh,bh
	or	bh,dl
	
	cmp	al,0
	je	.afterColonne
	.loopColonne:
		add	esi,2
		dec	al
		cmp	al,0
		je	.afterColonne
		jmp	.loopColonne
		
	.afterColonne:
		nop
	
	cmp	ah,0
	je	.afterLigne
	.loopLigne:
		add	esi,160
		dec	ah
		cmp	ah,0
		je	.afterLigne
		jmp	.loopLigne

	.afterLigne:
		nop
	
	mov	[esi],bx

	ret
