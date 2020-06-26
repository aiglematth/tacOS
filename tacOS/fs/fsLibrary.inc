;Auteur --> aiglematth

;Includes
%include	"fs/fsConstantes.inc"
%include	"fs/fsStructs.inc" 

;Macros

;Fonctions
;INIT ZONE
createRoot:
	;Permet de créer le repertoire racine
	push	rbp
	mov	rbp,rsp
	
	pusha64
	
	;On va aller mettre le premier bit de la bitmap à 1 car on alloue le premier bloc à la racine
	mov	byte [addrBitMap],10000000b
	
	;Maintenant on va créer l'Inode principal
	mov	byte  [inode.meta],11001111b ;11tttrwx --> 11 001(directory) 111(readable,writable,executable)
	mov	qword [inode.directAddr1],premierBlock
	
	;Et le load à la première place de l'inodeTable (on rappelle qu'elle est remplie de 0)
	mov	al,[inode.meta]
	mov	byte  [addrInodeTable + 0],al
	mov	rax,[inode.directAddr1]
	mov	qword [addrInodeTable + 1],rax
	
	;On va initialiser le DirBlock avec le "." qui représente le rep courant
	;On le met d'abord à 0
	xor	rbx,rbx
	
	.loopDirBlock:
		mov	qword [inode.directAddr1 + rbx],0x0000000000000000
		add	rbx,8
		cmp	rbx,blockSize
		jl	.loopDirBlock
	
	;Maintenant on crée la première entrée (de 256 octets sur 512) (le /)
	mov	byte  [inode.directAddr1],'/'
	mov	qword [inode.directAddr1 + (dirBlockEntry.addrInode - dirBlockEntry)],addrInodeTable
	
	;Puis l'entrée secondaire qui représente le rep courant (le .)
	mov	byte  [inode.directAddr1 + dirBlockSize],'.'
	mov	qword [inode.directAddr1 + dirBlockSize + (dirBlockEntry.addrInode - dirBlockEntry)],addrInodeTable
	
	popa64
	
	leave
	ret

initFs:
	;Permet d'initialiser un FS comme spécifié dans /doc/implementation.fs
	;On se sert des constantes de fsConstantes.inc, si besoin modifier les values
	
	push	rbp
	mov	rbp,rsp
	
	pusha64
	
	;Init de la bitmap à 0
	mov	rax,addrBitMap
	add	rax,sizeBitMap
	xor	rbx,rbx
	
	.loopBitMap:
		mov	qword [addrBitMap + rbx],0x0000000000000000
		add	rbx,8
		cmp	rbx,rax
		jl	.loopBitMap
	
	;Init de la inodeTable à 0 pour commencer
	mov	rax,addrInodeTable
	add	rax,sizeInodeTable
	xor	rbx,rbx
	
	.loopInode:
		mov	qword [addrInodeTable + rbx],0x0000000000000000
		add	rbx,8
		cmp	rbx,rax
		jl	.loopInode
	
	;Creation du premier inode (la racine /)
	;call	createRoot
	
	popa64
	
	leave
	ret
