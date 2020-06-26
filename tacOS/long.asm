;Auteur --> aiglematth

;RAPPEL : 
;La pile est de 0x1e00 à 0x2000

%include	"memory.inc"

bits	32

org	toLongMode

_start:
	call	longModeUp
	cmp	eax,0
	je	enterLongMode

	call	printBlockLongMode
	jmp	$


enterLongMode:
	;On entre en mode long !
	%include	"interractGdt64.inc"

bits	32
;Fonctions utiles
%include	"output32.inc"
%include	"output.inc"
%include	"longMode.inc"


printBlockLongMode:

	call	get_cursor
	mov	dl,[l_magenta]
	mov	dh,[black]
	mov	edi,blockMode
	mov	esi,screenBuffer
	xor	al,al
	mov	ah,1
	call	print

	ret
	
;DATA
%include	"gdt64.inc"
%include	"constantes.inc"

blockMode	db	"On ne peut pas passer en 64 bits...", 0

times	1024 - ($ - $$)	db	0

