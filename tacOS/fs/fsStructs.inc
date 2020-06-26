;Auteur --> aiglematth

;Structures (dans le nom mais pas dans la forme)
inode:
	.meta:			db	1
	.directAddr1:		dq	1
	.directAddr2:		dq	1
	.directAddr3:		dq	1
	.directAddr4:		dq	1
	.directAddr5:		dq	1
	.directAddr6:		dq	1
	.directAddr7:		dq	1
	.directAddr8:		dq	1
	.directAddr9:		dq	1
	.directAddr10:		dq	1
	.directAddr11:		dq	1
	.directAddr12:		dq	1
	.indirectAddr1:	dq	1
	.indirectAddr2:	dq	1
	.indirectAddr3:	dq	1
inodeEnd:

addrBlockEntry:
	.type:	dq	1
	.addr:	dq	1
addrBlockEntryEnd:

dirBlockEntry:
	.name:		db	248
	.addrInode:	dq	1
dirBlockEntryEnd:

dirBlockSize	equ	dirBlockEntryEnd-dirBlockEntry
