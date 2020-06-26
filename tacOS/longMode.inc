;Auteur --> aiglematth

;Peut-on utiliser l'instruction cpuid ? On va voir le bit 21 (22eme) du registre des drapeaux
isCpuidValid:
	;EAX = 0 si oui, sinon -1
	
	push	ebp
	mov	ebp,esp
	
	push	ecx

	;Récuperation du flag	
	pushfd
	pop	eax

	;Sauvegarde...
	mov	ecx,eax
	
	xor	eax,1 << 21
	
	;On va vérifier si le bit est re returné par le système
	push	eax
	popfd
	
	pushfd
	pop	eax
	
	;Restauration...
	push	ecx
	popfd
	
	;On regarde si ce sont les mêmes...
	;Si oui, le bit a été re retourné par le sys, cpuid n'est pas supporté
	cmp	eax,ecx
	
	jne	.isValid
	
	.isNotValid:
		mov	eax,-1
		jmp	.end
	
	.isValid:
		xor	eax,eax
		jmp	.end
	
	;Maintenant on test que la fonction située à 0x80000000 fonctionne
	mov	eax,0x80000000
	cpuid
	
	cmp	eax,0x80000001 ;La fonction est correcte
	jl	.isNotValid
	
	mov	eax,0x80000001
	cpuid
	
	and	edx,1 << 29
	cmp	edx,1 << 29 ;On vérifie le bit du Long Mode
	jne	.isNotValid
	
	jmp	.isValid
	
	.end:
		nop
		
	pop	ecx
	
	leave
	ret

pagingDown:
	;Met le bit de pagination (most significant bit hein monsieur Favry de cr0) à 0
	push	ebp
	mov	ebp,esp
	
	push	eax
	
	mov	eax,cr0
	and	eax,01111111111111111111111111111111b ;--> On met le poids fort à 0
	mov	cr0,eax
	
	pop	eax
	
	leave
	ret

pagingUp:
	;Met le bit de pagination (most significant bit hein monsieur Favry de cr0) à 1
	push	ebp
	mov	ebp,esp
	
	push	eax
	
	mov	eax,cr0
	or	eax,1 << 31 ;--> On met le dernier bit (poids fort) à 1
	mov	cr0,eax
	
	pop	eax
	
	leave
	ret

pagingLongModeUp:
	;Met le bit de pagination mode long 1
	push	ebp
	mov	ebp,esp
	
	push	eax
	
	mov	eax,cr4
	or	eax,1 << 5 
	mov	cr4,eax
	
	pop	eax
	
	leave
	ret

longModeBitUp:
	;Met le bit du mode long dans les registres de models specifiques à 1
	
	push	ebp
	mov	ebp,esp
	
	pusha
	
	mov	ecx,0xc0000080  ;L'adresse de notre registre de model specifique
	rdmsr			;Met dans edx:eax le rms de l'adresse dans ecx
	or	eax,1 << 8	;On met le bit 8 (9eme) qui est le LME (long mode enable) à 1
	wrmsr
	
	popa
	
	leave
	ret

clearTables:
	;On met la mémoire à 0 sur la taille dont on aura besoin pour la pagination
	
	push	ebp
	mov	ebp,esp
	
	push	ecx
	push	eax
	
	mov	edi,pagination
	mov	cr3,edi
	xor	eax,eax
	mov	ecx,4096
	rep	stosd
	mov	edi,cr3
	
	pop	eax
	pop	ecx
	
	leave
	ret

longModeUp:
	;Allé on active enfin le mode long...
	;On en a bavé mais on va enfin pouvoir utiliser nos registres habituels
	;Retour :
	;EAX = 0 si le long mode est activé, sinon -1
	
	push	ebp
	mov	ebp,esp
	
	call	isCpuidValid
	cmp	eax,0
	jne	.isNotValid
	
	call	pagingDown
	
	call	clearTables
	
	;On prépare les adresses des tables en les faisant correctement pointer
	mov	dword [edi],pagination + 0x1003
	add	edi,0x1000
	mov	dword [edi],pagination + 0x2003
	add	edi,0x1000
	mov	dword [edi],pagination + 0x3003
	add	edi,0x1000
	
	;On met les pages avec les droites rwx
	mov ebx,3
	mov ecx,512

	.mettreRWX:
	 	mov	dword [edi], ebx
	 	add	ebx, 0x1000
	  	add 	edi, 8
	  	loop	.mettreRWX
	  
	 call	pagingLongModeUp
	 call	longModeBitUp
	 call	pagingUp
	 
	 jmp	.isValid
	
	.isNotValid:
		mov	eax,-1
		jmp	.end
	
	.isValid:
		xor	eax,eax
		jmp	.end
	
	.end:
		nop
	
	leave
	ret
	
