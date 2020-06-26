;Auteur --> aiglematth

bits	64

;Includes
%include	"utils.inc"

;Macros
%macro	sendEoi	0
	mov	al,EOI
	out	picCommandeMaitre,al
%endmacro

%macro	sendEoiAll	0
	mov	al,EOI
	out	picCommandeEsclave,al
	out	picCommandeMaitre,al
%endmacro

%macro	addInt	3
	;%1 --> adresse de l'int
	;%2 --> Numéro de l'int
	;%3 --> Type de l'int
	mov	rax,%1
	mov	rbx,%2
	mov	rcx,%3
	call	createInt
%endmacro

%macro	addErr	2
	;%1 --> Erreur
	;%2 --> Numéro de l'err
	mov	rax,%1
	mov	rbx,%2
	mov	rcx,err64type
	call	createInt
%endmacro

%macro	addIrq	2
	;%1 --> Addr handler
	;%2 --> Numéro de l'irq
	mov	rax,%1
	mov	rbx,%2
	mov	rcx,int64type
	call	createInt
%endmacro

;Fonctions
reprogrammePic:
	push	rbp
	mov	rbp,rsp
	
	pusha64
	
	mov	al,0x11
	out	picCommandeMaitre,al
	out	picCommandeEsclave,al
	call	temp
	
	mov	al,0x20
	out	picDataMaitre,al
	add	al,8
	out	picDataEsclave,al
	call	temp
	
	mov	al,0x04
	out	picDataMaitre,al
	mov	al,0x02
	out	picDataEsclave,al
	call	temp
	
	mov	al,0x01
	out	picDataMaitre,al
	out	picDataEsclave,al
	call	temp

	;On rend toutes les IRQ valides !
	call disableIrq

	popa64

	leave
	ret

disableIrq:
	mov	al,0xff
	out	picDataMaitre,al
	out	picDataEsclave,al
	ret

enableIrq:
	mov	al,0x00
	out	picDataMaitre,al
	out	picDataEsclave,al
	ret

enableKeyboard:
	mov	al,11111101b
	out	picDataMaitre,al
	ret

temp:
	ret
	
createInt:
	;RAX --> Adresse de l'interruption
	;RBX --> Numéro de l'interruption
	;RCX --> Type et attributs de l'interruption (en vérité, loge dans CL)
	
	push	rbp
	mov	rbp,rsp
	
	push	rdx
	
	;On remplit la structure d'une entrée de l'idt correctement
	mov	[idtEntry.offsetLow],ax
	mov	[idtEntry.type],cl
	
	mov	cx,cs
	mov	[idtEntry.selector],cx
	
	shr	rax,16
	mov	[idtEntry.offsetMid],ax
	
	shr	rax,16
	mov	[idtEntry.offsetEnd],eax
	
	;On va mettre la bonne addresse dans rbx
	;RBX = RBX*16
	mov	rax,16
	mul	rbx
	mov	rbx,rax
	
	;RBX = RBX + addrOfIdtTable
	mov	rax,idtTable
	add	rbx,rax
	
	;On la load dans l'idt
	mov	cx,[idtEntry.offsetLow]
	mov	[rbx],cx
	
	mov	cx,[idtEntry.selector]
	mov	[rbx + 2],cx
	
	mov	[rbx + 4],byte 0x00
	
	mov	cl,[idtEntry.type]
	mov	[rbx + 5],cl
	
	mov	cx,[idtEntry.offsetMid]
	mov	[rbx + 6],cx
	
	mov	ecx,[idtEntry.offsetEnd]
	mov	[rbx + 8],ecx
	
	xor	ecx,ecx
	mov	[rbx + 12],ecx
	
	pop	rdx
	
	leave
	ret

createIdt:
	push	rbp
	mov	rbp,rsp
	
	pusha64
	
	;Gestion des erreurs (int 0-31)
	addErr	divZero,0
	addErr	debug,1
	addErr	nonMask,2
	addErr	break,3
	addErr	overflow,4
	addErr	bound,5
	addErr	invOp,6
	addErr	devNotAvai,7
	addErr	doubleFault,8
	addErr	defaultErr,9
	addErr	invalidTss,10
	addErr	segmNotPresent,11
	addErr	stackSegmentFault,12
	addErr	genProtectFault,13
	addErr	pageFault,14
	addErr	defaultErr,15
	addErr	floatExcept,16
	addErr	alignCheck,17
	addErr	machineCheck,18
	addErr	floatExcept,19
	addErr	virtExcept,20
	addErr	defaultErr,21
	addErr	defaultErr,22
	addErr	defaultErr,23
	addErr	defaultErr,24
	addErr	defaultErr,25
	addErr	defaultErr,26
	addErr	defaultErr,27
	addErr	defaultErr,28
	addErr	defaultErr,29
	addErr	securityExcept,30
	addErr	defaultErr,31
	
	;Gestion des irq (int 32-47)
	addIrq	progTime,32
	addIrq	keyboard,33
	addIrq	cascade,34
	addIrq	com2,35
	addIrq	com1,36
	addIrq	lpt2,37
	addIrq	floppyDisk,38
	addIrq	lpt1,39
	addIrq	cmos,40
	addIrq	free,41
	addIrq	free,42
	addIrq	free,43
	addIrq	mouse,44
	addIrq	fpu,45
	addIrq	primHardDisk,46
	addIrq	secHardDisk,47
	
	;Gestion de mes interruptions
	addInt	test,0x80,int64type
	
	popa64
	
	leave
	ret

test:
	pusha64
	mov dword [0xB8000],"I T "
	popa64
	iretq

;Handlers pour les erreurs
divZero:
	pusha64
	mov 	dword [0xB8000],"E 0 "
	popa64
	iretq

debug:
	pusha64
	mov 	dword [0xB8000],"E 1 "
	popa64
	iretq

nonMask:
	pusha64
	mov 	dword [0xB8000],"E 2 "
	popa64
	iretq

break:
	pusha64
	mov 	dword [0xB8000],"E 3 "
	popa64
	iretq

overflow:
	pusha64
	mov 	dword [0xB8000],"E 4 "
	popa64
	iretq

bound:
	pusha64
	mov 	dword [0xB8000],"E 5 "
	popa64
	iretq

invOp:
	pusha64
	mov 	dword [0xB8000],"E 6 "
	popa64
	iretq

devNotAvai:
	pusha64
	mov 	dword [0xB8000],"E 7 "
	popa64
	iretq

doubleFault:
	pusha64
	mov 	dword [0xB8000],"E 8 "
	popa64
	iretq

invalidTss:
	pusha64
	mov	dword [0xB8000],"E 1 "
	mov	dword [0xB8004],"0   " 
	popa64
	iretq

segmNotPresent:
	pusha64
	mov	dword [0xB8000],"E 1 "
	mov	dword [0xB8004],"1   "
	popa64
	iretq

stackSegmentFault:
	pusha64
	mov	dword [0xB8000],"E 1 "
	mov	dword [0xB8004],"2   "
	popa64
	iretq

genProtectFault:
	pusha64
	mov	dword [0xB8000],"E 1 "
	mov	dword [0xB8004],"3   "
	popa64
	iretq

pageFault:
	pusha64
	mov	dword [0xB8000],"E 1 "
	mov	dword [0xB8004],"4   "
	popa64
	iretq

alignCheck:
	pusha64
	mov	dword [0xB8000],"E 1 "
	mov	dword [0xB8004],"7   "
	popa64
	iretq

machineCheck:
	pusha64
	mov	dword [0xB8000],"E 1 "
	mov	dword [0xB8004],"8   "
	popa64
	iretq

virtExcept:
	pusha64
	mov	dword [0xB8000],"E 2 "
	mov	dword [0xB8004],"0   "
	popa64
	iretq

securityExcept:
	pusha64
	mov	dword [0xB8000],"E 3 "
	mov	dword [0xB8004],"0   "
	popa64
	iretq

floatExcept:
	pusha64
	mov 	dword [0xB8000],"E ! "
	popa64
	iretq

defaultErr:
	pusha64
	mov 	dword [0xB8000],"E ? "
	popa64
	iretq

;Handlers pour les IRQ
progTime:
	pusha64
	mov 	dword [0xB8000],"I 0 "
	sendEoi
	popa64
	iretq

keyboard:
	pusha64
	mov	dword [0xB8000],"I 1 "
	sendEoi
	popa64
	iretq
	
cascade:
	pusha64
	mov 	dword [0xB8000],"I 2 "
	sendEoi
	popa64
	iretq	

com2:
	pusha64
	mov 	dword [0xB8000],"I 3 "
	sendEoi
	popa64
	iretq

com1:
	pusha64
	mov 	dword [0xB8000],"I 4 "
	sendEoi
	popa64
	iretq

lpt2:
	pusha64
	mov 	dword [0xB8000],"I 5 "
	sendEoi
	popa64
	iretq

floppyDisk:
	pusha64
	mov 	dword [0xB8000],"I 6 "
	sendEoi
	popa64
	iretq

lpt1:
	pusha64
	mov 	dword [0xB8000],"I 7 "
	sendEoi
	popa64
	iretq

cmos:
	pusha64
	mov 	dword [0xB8000],"i 0 "
	sendEoiAll
	popa64
	iretq

free:
	pusha64
	mov 	dword [0xB8000],"i ? "
	sendEoiAll
	popa64
	iretq

mouse:
	pusha64
	mov 	dword [0xB8000],"i 4 "
	sendEoiAll
	popa64
	iretq

fpu:
	pusha64
	mov 	dword [0xB8000],"i 5 "
	sendEoiAll
	popa64
	iretq

primHardDisk:
	pusha64
	mov 	dword [0xB8000],"i 6 "
	sendEoiAll
	popa64
	iretq

secHardDisk:
	pusha64
	mov 	dword [0xB8000],"i 7 "
	sendEoiAll
	popa64
	iretq
