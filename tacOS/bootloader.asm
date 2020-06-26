;Auteur --> aiglematth

%include	"memory.inc"

bits	16

org	origine

_bootloader:
	mov	bp,0x8000 
	xor	ax,ax     
	mov	es,ax     
	mov 	ds,ax     
	mov 	ss,ax     
	mov 	sp,bp     

	;Check disk
	mov	ah,0
	int	0x13

	;Load kernel
	mov	[disk],dl
	
	mov	ah,2
	mov	al,64 ; 64*512
	mov	bx,toProtecMode
	xor	ch,ch
	mov	cl,2
	xor	dh,dh
	mov	dl,[disk]
	int	0x13
	
	jc	badEnd
	jmp	toProtecMode

	badEnd:
		mov	si,msgErr
		call	print
		jmp	$
		
print:
	;SI --> PTR to the string

	loop_al_not_zero:
		mov	al,[si]
		inc 	si
		or 	al,al
		jnz 	afficher

	jmp	end

	afficher:
		mov	ah,0x0e
		mov	bh,0x00
		mov	bl,0x07
		int	0x10
		jmp	loop_al_not_zero

	end:
		nop	
	ret

;DATA
msgErr	db	"Erreur...", 10, 0
disk	db	0

;On fini notre programme par aa55 (c'est la signature recherchée par le bios)
times	510 - ($-$$)	db	0
dw	0xaa55
