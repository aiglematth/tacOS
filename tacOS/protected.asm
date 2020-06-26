;Auteur --> aiglematth

%include	"memory.inc"

bits	16

org	toProtecMode

_start:
	;Protected mode
	%include	"interractGdt.inc"
	
	jmp	codeSeg:protected

bits	32
protected:
	;Contexte
	mov	ax,dataSeg
 	mov	ds,ax
	mov	es,ax
	mov 	fs,ax
	mov 	gs,ax
	mov 	ss,ax
	
	mov	ebp,stack
	mov	esp,ebp
	sub	esp,sizeOfStack

	call	printBanner
	
	jmp	toLongMode
	
;Fonctions utiles
%include	"output32.inc"
%include	"output.inc"

printBanner:
	mov	dx,[l_magenta]
	call	clear
	
	mov	dl,[l_magenta]
	mov	dh,[black]
	mov	edi,banner
	mov	esi,screenBuffer
	xor	eax,eax
	call	print
	
	ret

;DATA
%include	"gdt.inc"
%include	"constantes.inc"

banner		db	"On est en protected mode ! (32 bits...)", 0

times	1024 - ($-$$)	db	0
