;Auteur --> aiglematth

;Mettre la struct qui pointe vers la gdt dans ptrGdt !
	
cli

lgdt	[ptrGdt]

mov	eax,cr0
or	eax,1
mov	cr0,eax

