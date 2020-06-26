#Auteur --> aiglematth

# On décrit notre file system fait maison                    #
# On s'inspire d'un systéme de fichier ext (en plus simple)  #

STRUCT bitMap
	Description:
		Permet de savoir si un bloc est alloué ou non, si le bit n ème
		bit est à 1, le n ème bloc est utilisé, sinon, il est utilisable
	Attributs:
		-Taille: nombreBits*tailleBloc = 32Gio On la fixe de façon à pouvoir utiliser 32 Gio
END STRUCT

STRUCT inodeTable
	Description:
		Contient un nombre fixe d'inodes allouables (et donc de fichiers créables)
	Attributs:
		-Taille: tailleInode*nombreInode = tailleInodeTable (on va considérer qu'un fichier fait
		 en moyenne 1Mio, donc 32Gio = 1Mio*32768, on va donc tenter de pouvoir allouer 32768 indodes)
END STRUCT

STRUCT inode
	Description:
		Un inode va représenter toutes les données dont on a besoin pour accéder à un fichier
		hormis son nom, qui sera stocké dans la table des dossiers
	Attributs:
		-Taille: 1 + 12*8 + 3*8 = 121 Octets
		-Metadata: (11tttrwx) 
			-ttt --> 000 (fichier) | 001 (dossier) On garde des bits de rab pour implémenter des liens
			-r   --> readable   si 1, sinon 0
			-w   --> writable   si 1, sinon 0
			-x   --> executable si 1, sinon 0
		-12 Direct data blocks:   Adresse des blocs de données
		-3  Indirect data blocks: Adresse d'un bloc qui contient des adresses vers des blocs de données
END STRUCT

STRUCT AddrBlock
	Description:
		Un bloc contenu dans la zone data qui contient un tableau d'adresses
	Attributs:
		-Taille:	 Taille d'un bloc
		-AddrBlockEntry: Une entrée d'un bloc de data
END STRUCT

STRUCT AddrBlockEntry:
	Description:
		Une entrée d'un bloc d'addresses
	Attributs:
		-Taille: 1*8 + 1*8 = 16 Octets (le type je le met en qword pour que 
		 la taille du bloc / la taille d'une entrée soit entière)
		-Type:   Un qword, si 0, l'addr pointe sur un dataBlock, si 1, l'addr pointe sur un addrBlock
		-Addr:   Une adresse dans la mémoire
END STRUCT

STRUCT DirBlock
	Description:
		Un block directory, la première entrée décrit le directory actuel, le second le dir préceddent
	Attributs:
		-Taille:          Taille d'un bloc
		-DirBlockEntries: Des entrées décrites postérieurement
END STRUCT

STRUCT DirBlockEntry
	Description:
		Une entrée d'un DirBlock
	Attributs:
		-Taille: 248 + 1*8 = 256 Octets
		-Nom du fichier: On va le limiter à 247 chars (le 248eme est 0x00)
		-AddrInode:	 Adresse de l'inode dans l'inodeTable
END STRUCT
