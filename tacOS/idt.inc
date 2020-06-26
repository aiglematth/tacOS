;Auteur --> aiglematth

bits	64

;CONSTANTES

;Types
int64type	equ	10001110b
err64type	equ	10001110b

;Addresses
offsetKernel		equ	0x0008
picCommandeMaitre	equ	0x20
picDataMaitre		equ	0x21
picCommandeEsclave	equ	0xa0
picDataEsclave		equ	0xa1
keyboardControl	equ	0x64
keyboardData		equ	0x60

;Commandes
EOI	equ	0x20


;DATA
;Entrée de la table IDT
idtEntry:
	.offsetLow	dw	0x0000
	.selector	dw	offsetKernel
	.zero		db	0x00
	.type		db	0x00
	.offsetMid	dw	0x0000
	.offsetEnd	dd	0x00000000
	.zeroEnd	dd	0x00000000

;Table IDT
idtTable:
	times	256*16	db	0
idtEnd:

;Descripteur de la table
idtDesc:
	.size	dw	idtEnd - idtTable - 1
	.addr	dq	idtTable

