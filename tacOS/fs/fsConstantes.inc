;Auteur --> aiglematth

blockSize	equ	512     ;Octets
nbrInodes	equ	32768

;Memory
addrBitMap	equ	0x20000
sizeBitMap	equ	8388608 ;Octets

addrInodeTable	equ	0x820000
sizeInodeTable	equ	nbrInodes*121 ;NbrInode*TailleInode

premierBlock	equ	0xbe8000
