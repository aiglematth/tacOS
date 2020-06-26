;Auteur --> aiglematth

bits	64

%include	"memory.inc"

org	longModeMem

longMode:
	;On est enfin en mode long !!
	
	mov	rsp,stack
	mov	rbp,rsp
	sub	rbp,sizeOfStack                  
    	
    	mov	dx,[l_blue]
    	call	clear
	
	;On va remapp les PIC, remplir notre IDT et la load
	call	reprogrammePic
	call	createIdt
	lidt	[idtDesc]

	call	enableIrq

        sti
	
	;Maintenant on va mettre en place le FileSystem
	call	initFs
	
	mov	dl,[l_blue]
	mov	dh,[black]
	mov	rdi,longModeBitch
	mov	esi,screenBuffer
	xor	rax,rax
	call	print64
	
	jmp	$

;Fonctions utiles
%include	"output.inc"
%include	"output64.inc"
%include	"func_idt.inc"
%include	"fs/fsLibrary.inc"

;Data
%include	"constantes.inc"
%include	"idt.inc"

longModeBitch	db	"On est en 64 bits !", 0

;END
times	31744-1024 - ($ - $$)	db	0
