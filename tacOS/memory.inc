;Auteur --> aiglematth
;Allé on définit des constantes pour que notre code soit modulable !

origine	equ	0x7c00
toProtecMode	equ	0x7e00
stack		equ	0x2000
sizeOfStack	equ	0x1000
toLongMode	equ	0x8200
longModeMem	equ	0x8600
pagination	equ	0x10000
