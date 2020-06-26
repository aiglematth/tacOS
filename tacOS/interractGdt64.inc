;Auteur --> aiglematth

lgdt	[GDT64.Pointer]

;Un bon contexte
cli                           
mov	ax, GDT64.Data            
mov 	ds, ax                    
mov 	es, ax                    
mov 	fs, ax                    
mov 	gs, ax                   
mov 	ss, ax

jmp	GDT64.Code:longModeMem
