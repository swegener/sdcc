/* lklibr.c */

/*
 * (C) Copyright 1989-1995
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 *
 * With contributions for the
 * object libraries from
 * Ken Hornstein
 * kenh@cmf.nrl.navy.mil
 *
 */

/*
 * Extensions: P. Felber
 */

#define EQ(A,B) !strcmp((A),(B))
#define MAXLINE 254 /*when using fgets*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aslink.h"

#ifdef	OTHERSYSTEM
#ifdef SDK
#ifdef UNIX
    #define LKDIRSEP '/'
    #define LKDIRSEPSTR "/"
#else /* UNIX */
	#define LKDIRSEP '\\'
	#define LKDIRSEPSTR "\\"
#endif /* UNIX */
#else /* SDK */
	#define LKDIRSEP '\\'
	#define LKDIRSEPSTR "\\"
#endif /* SDK */
#endif

#ifdef __CYGWIN__
void ToCygWin(char * filspc)
{
    char posix_path[PATH_MAX];
    void cygwin_conv_to_full_posix_path(char * win_path, char * posix_path);
    cygwin_conv_to_full_posix_path(filspc, posix_path);
    strcpy(filspc, posix_path);
}
#endif

/*)Module	lklibr.c
 *
 *	The module lklibr.c contains the functions which
 *	(1) specify the path(s) to library files [.LIB]
 *	(2) specify the library file(s) [.LIB] to search
 *	(3) search the library files for specific symbols
 *	    and link the module containing this symbol
 *
 *	lklibr.c contains the following functions:
 *		VOID	addpath()
 *		VOID	addlib()
 *		VOID	addfile()
 *		VOID	search()
 *		VOID	fndsym()
 *		VOID	library()
 *		VOID	loadfile()
 *
 */

#ifdef INDEXLIB
typedef struct slibrarysymbol mlibrarysymbol;
typedef struct slibrarysymbol *pmlibrarysymbol;

struct slibrarysymbol {
	char name[NCPS];
	pmlibrarysymbol next;
};

typedef struct slibraryfile mlibraryfile;
typedef struct slibraryfile *pmlibraryfile;

struct slibraryfile {
	int loaded;
	char *libspc;
	char *str;
	char relfil[FILSPC];
	char filename[FILSPC];
    long offset; //if > 0, the embedded file offset in the library file libspc
	pmlibrarysymbol symbols;
	pmlibraryfile next;
};

int buildlibraryindex();
#endif /* INDEXLIB */

/*)Function	VOID	addpath()
 *
 *	The function addpath() creates a linked structure containing
 *	the paths to various object module library files.
 *
 *	local variables:
 *		lbpath	*lbph		pointer to new path structure
 *		lbpath	*lbp		temporary pointer
 *
 *	global variables:
 *		lbpath	*lbphead	The pointer to the first
 *				 	path structure
 *
 *	 functions called:
 *		char	getnb()		lklex.c
 *		VOID *	new()		lksym.c
 *		int	strlen()	c_library
 *		char *	strcpy()	c_library
 *		VOID	unget()		lklex.c
 *
 *	side effects:
 *		An lbpath structure may be created.
 */

VOID
addpath()
{
	struct lbpath *lbph, *lbp;

	lbph = (struct lbpath *) new (sizeof(struct lbpath));
	if (lbphead == NULL) {
		lbphead = lbph;
	} else {
		lbp = lbphead;
		while (lbp->next)
			lbp = lbp->next;
		lbp->next = lbph;
	}
	unget(getnb());
	lbph->path = (char *) new (strlen(ip)+1);
	strcpy(lbph->path, ip);
}

/*)Function	VOID	addlib()
 *
 *	The function addlib() tests for the existance of a
 *	library path structure to determine the method of
 *	adding this library file to the library search structure.
 *
 *	This function calls the function addfile() to actually
 *	add the library file to the search list.
 *
 *	local variables:
 *		lbpath	*lbph		pointer to path structure
 *
 *	global variables:
 *		lbpath	*lbphead	The pointer to the first
 *				 	path structure
 *      ip a pointer to the library name
 *
 *	 functions called:
 *		VOID	addfile()	lklibr.c
 *		char	getnb()		lklex.c
 *		VOID	unget()		lklex.c
 *
 *	side effects:
 *		The function addfile() may add the file to
 *		the library search list.
 */

VOID
addlib()
{
	struct lbpath *lbph;
    int foundcount=0;

	unget(getnb());

	if (lbphead == NULL)
    {
		foundcount=addfile(NULL, ip);
	}
    else
    {
	    for (lbph=lbphead; lbph; lbph=lbph->next)
        {
		    foundcount+=addfile(lbph->path, ip);
	    }
    }
    if(foundcount == 0)
    {
        printf("\n?ASlink-Warning-Couldn't find library '%s'", ip);
    }
}

/*)Function	int	addfile(path,libfil)
 *
 *		char	*path		library path specification
 *		char	*libfil		library file specification
 *
 *	The function addfile() searches for the library file
 *	by concatenating the path and libfil specifications.
 *	if the library is found, an lbname structure is created
 *	and linked to any previously defined structures.  This
 *	linked list is used by the function fndsym() to attempt
 *	to find any undefined symbols.
 *
 *	The function does not give report an error on invalid
 *	path / file specifications or if the file is not found.
 *
 *	local variables:
 *		lbname	*lbnh		pointer to new name structure
 *		lbname	*lbn		temporary pointer
 *
 *	global variables:
 *		lbname	*lbnhead	The pointer to the first
 *				 	path structure
 *
 *	 functions called:
 *		char	getnb()		lklex.c
 *		VOID *	new()		lksym.c
 *		int	strlen()	c_library
 *		char *	strcpy()	c_library
 *		VOID	unget()		lklex.c
 *
 *	side effects:
 *		An lbname structure may be created.
 *
 *  return:
 *      1: the library was found
 *      0: the library was not found
 */

int addfile(char * path, char * libfil)
{
	FILE *fp;
	char *str;
	struct lbname *lbnh, *lbn;
    int libfilinc=0;

	if (path != NULL)
    {
		str = (char *) new (strlen(path) + strlen(libfil) + 6);
		strcpy(str, path);

        if (str[strlen(str)-1] != LKDIRSEP)
        {
			strcat(str, LKDIRSEPSTR);
		}
	}
    else
    {
		str = (char *) new (strlen(libfil) + 5);
	}

	if (libfil[0] == LKDIRSEP)
    {
        libfil++;
        libfilinc=1;
    }
	
    strcat(str, libfil);

	if(strchr(libfil, FSEPX) == NULL)
    {
		sprintf(&str[strlen(str)], "%clib", FSEPX);
	}

    fp=fopen(str, "r");
    if(fp == NULL)
    {
        /*Ok, that didn't work.  Try with the 'libfil' name only*/
        if(libfilinc) libfil--;
        fp=fopen(libfil, "r");
        if(fp != NULL) 
        {
            /*Bingo!  'libfil' is the absolute path of the library*/
            strcpy(str, libfil);
            path=NULL;/*This way 'libfil' and 'path' will be rebuilt from 'str'*/
        }
    }

    if(path==NULL)
    {
        /*'path' can not be null since it is needed to find the '.o' files associated with
        the library.  So, get 'path' from 'str' and then chop it off and recreate 'libfil'.
        That way putting 'path' and 'libfil' together will result into the original filepath
        as contained in 'str'.*/
        int j;
        path = (char *) new (strlen(str));
        strcpy(path, str);
        for(j=strlen(path)-1; j>=0; j--)
        {
            if((path[j]=='\\')||(path[j]=='/'))
            {
                strcpy(libfil, &path[j+1]);
                path[j+1]=0;
                break;
            }
        }
        if(j<=0) path[0]=0;
    }

	if (fp != NULL)
    {
		fclose(fp);
		lbnh = (struct lbname *) new (sizeof(struct lbname));
		if (lbnhead == NULL)
        {
			lbnhead = lbnh;
		}
        else
        {
			lbn = lbnhead;
			while (lbn->next)
				lbn = lbn->next;
			lbn->next = lbnh;
		}

		lbnh->path = path;
		lbnh->libfil = (char *) new (strlen(libfil) + 1);
		strcpy(lbnh->libfil, libfil);
		lbnh->libspc = str;
        return 1;
	}
    else
    {
		free(str);
        return 0;
	}
}

/*)Function	VOID	search()
 *
 *	The function search() looks through all the symbol tables
 *	at the end of pass 1.  If any undefined symbols are found
 *	then the function fndsym() is called. Function fndsym()
 *	searches any specified library files to automagically
 *	import the object modules containing the needed symbol.
 *
 *	After a symbol is found and imported by the function
 *	fndsym() the symbol tables are again searched.  The
 *	symbol tables are search until no more symbols can be
 *	resolved within the library files.  This ensures that
 *	back references from one library module to another are
 *	also resolved.
 *
 *	local variables:
 *		int	i		temporary counter
 *		sym	*sp		pointer to a symbol structure
 *		int	symfnd		found a symbol flag
 *
 *	global variables:
 *		sym	*symhash[]	array of pointers to symbol tables
 *
 *	 functions called:
 *		int	fndsym()	lklibr.c
 *
 *	side effects:
 *		If a symbol is found then the library object module
 *		containing the symbol will be imported and linked.
 */

VOID
search()
{
	register struct sym *sp;
	register int i, symfnd;

	/*
	 * Look for undefined symbols.  Keep
	 * searching until no more symbols are resolved.
	 */
	symfnd = 1;
	while (symfnd) {
		symfnd = 0;
		/*
		 * Look through all the symbols
		 */
		for (i=0; i<NHASH; ++i) {
			sp = symhash[i];
			while (sp) {
				/* If we find an undefined symbol
				 * (one where S_DEF is not set), then
				 * try looking for it.  If we find it
				 * in any of the libraries then
				 * increment symfnd.  This will force
				 * another pass of symbol searching and
				 * make sure that back references work.
				 */
				if ((sp->s_type & S_DEF) == 0) {
					if (fndsym(sp->s_id)) {
						symfnd++;
					}
				}
				sp = sp->s_sp;
			}
		}
	}
}

/*Load a .rel file embedded in a sdcclib file*/
void LoadRel(FILE * libfp, char * ModName)
{
	char str[NINPUT+2];
	int state=0;

	while (fgets(str, NINPUT, libfp) != NULL)
	{
		str[NINPUT+1] = '\0';
		chop_crlf(str);
		switch(state)
		{
			case 0:
				if(EQ(str, "<FILE>"))
				{
					fgets(str, NINPUT, libfp);
					str[NINPUT+1] = '\0';
					chop_crlf(str);
					if(EQ(str, ModName)) state=1;
					else
					{
						printf("Bad offset in library file str=%s, Modname=%s\n", str, ModName);
						lkexit(1);
					}
				}
			break;
			case 1:
				if(EQ(str, "<REL>")) state=2;
			break;
			case 2:
				if(EQ(str, "</REL>")) return;
				ip = str;
				link();
			break;
		}
	}
}

/*)Function	VOID	fndsym(name)
 *
 *		char	*name		symbol name to find
 *
 *	The function fndsym() searches through all combinations of the
 *	library path specifications (input by the -k option) and the
 *	library file specifications (input by the -l option) that
 *	lead to an existing file.
 *
 *	The file specicifation may be formed in one of two ways:
 *
 *	(1)	If the library file contained an absolute
 *		path/file specification then this becomes filspc.
 *		(i.e. C:\...)
 *
 *	(2)	If the library file contains a relative path/file
 *		specification then the concatenation of the path
 *		and this file specification becomes filspc.
 *		(i.e. \...)
 *
 *	The structure lbfile is created for the first library
 *	object file which contains the definition for the
 *	specified undefined symbol.
 *
 *	If the library file [.LIB] contains file specifications for
 *	non existant files, no errors are returned.
 *
 *	local variables:
 *		char	buf[]		[.REL] file input line
 *		char	c		[.REL] file input character
 *		FILE	*fp		file handle for object file
 *		lbfile	*lbf		temporary pointer
 *		lbfile	*lbfh		pointer to lbfile structure
 *		FILE	*libfp		file handle for library file
 *		lbname	*lbnh		pointer to lbname structure
 *		char	*path		file specification path
 *		char	relfil[]	[.REL] file specification
 *		char	*str		combined path and file specification
 *		char	symname[]	[.REL] file symbol string
 *
 *	global variables:
 *		lbname	*lbnhead	The pointer to the first
 *				 	name structure
 *		lbfile	*lbfhead	The pointer to the first
 *				 	file structure
 *
 *	 functions called:
 *		int	fclose()	c_library
 *		int	fgets()		c_library
 *		FILE	*fopen()	c_library
 *		VOID	free()		c_library
 *		char	getnb()		lklex.c
 *		VOID	lkexit()	lkmain.c
 *		VOID	loadfile()	lklibr.c
 *		VOID *	new()		lksym.c
 *		char *	sprintf()	c_library
 *		int	sscanf()	c_library
 *		char *	strcat()	c_library
 *		char *	strchr()	c_library
 *		char *	strcpy()	c_library
 *		int	strlen()	c_library
 *		int	strncmp()	c_library
 *		VOID	unget()		lklex.c
 *
 *	side effects:
 *		If the symbol is found then a new lbfile structure
 *		is created and added to the linked list of lbfile
 *		structures.  The file containing the found symbol
 *		is linked.
 */

#ifdef INDEXLIB

/* First entry in the library object symbol cache */
mlibraryfile libr;

int fndsym( char *name )
{
	struct lbfile *lbfh, *lbf;
	pmlibraryfile ThisLibr;
	pmlibrarysymbol ThisSym = NULL;

	/* Build the index if this is the first call to fndsym */
	if (libr.next==NULL)
		buildlibraryindex();
	
	/* Iterate through all library object files */
	ThisLibr = libr.next;
	while (ThisLibr) {

		/* Iterate through all symbols in an object file */
		ThisSym = ThisLibr->symbols->next;

		while (ThisSym) {
            //printf("ThisSym->name=%s\n", ThisSym->name);
			if (!strcmp(ThisSym->name, name)) {
				if (!ThisLibr->loaded) {
					/* Object file is not loaded - add it to the list */
					lbfh = (struct lbfile *) new (sizeof(struct lbfile));
					if (lbfhead == NULL) {
						lbfhead = lbfh;
					} else {
						lbf = lbfhead;
						while (lbf->next)
						lbf = lbf->next;
						lbf->next = lbfh;
					}
					lbfh->libspc = ThisLibr->libspc;
					lbfh->filspc = ThisLibr->str;
					lbfh->relfil = (char *) new (strlen(ThisLibr->relfil) + 1);
					strcpy(lbfh->relfil,ThisLibr->relfil);
                    lbfh->offset = ThisLibr->offset;
                    if(lbfh->offset>0)
                    { /*For an embedded object file in a library*/
                        void loadfile_SdccLib(char * libspc, char * module, long offset);
                        loadfile_SdccLib(lbfh->libspc, lbfh->relfil, lbfh->offset);
                    }
                    else
                    { /*For a stand alone object file*/
					    loadfile(lbfh->filspc);
                    }
					ThisLibr->loaded=1;
				}
				return (1);	/* Found the symbol, return */
			}
			ThisSym=ThisSym->next;  /* Next sym in library */
		}
		ThisLibr=ThisLibr->next; /* Next library in list */
	}
	return 0;	/* Failure - symbol not found in any library */
}

pmlibraryfile buildlibraryindex_SdccLib(char * PathLib, FILE * libfp, char * DirLib, pmlibraryfile This)
{
	char ModName[NCPS]="";
	char FLine[MAXLINE+1];
	int state=0;
	long IndexOffset=0, FileOffset;
    pmlibrarysymbol ThisSym = NULL;

	while(!feof(libfp))
    {
        FLine[0]=0;
        fgets(FLine, MAXLINE, libfp);
        chop_crlf(FLine);

        switch(state)
        {
            case 0:
                if(EQ(FLine, "<INDEX>"))
                {
					/*The next line has the size of the index*/
                    FLine[0]=0;
                    fgets(FLine, MAXLINE, libfp);
                    chop_crlf(FLine);
					IndexOffset=atol(FLine);
					state=1;
                }
            break;
            case 1:
                if(EQ(FLine, "<MODULE>"))
				{
					/*The next line has the name of the module and the offset
					of the corresponding embedded file in the library*/
                    FLine[0]=0;
                    fgets(FLine, MAXLINE, libfp);
                    chop_crlf(FLine);
					sscanf(FLine, "%s %ld", ModName, &FileOffset);
					state=2;

                    /*create a new libraryfile object for this module*/
					This->next = (pmlibraryfile)new( sizeof( mlibraryfile ));
					if (This->next == NULL)
                    {
						printf("panic: can't allocate memory.\n");
						exit(-1);
					}

					This=This->next;
					This->next = NULL;
					This->loaded=-1; /*Kind of useless, but...*/
                    This->offset=FileOffset+IndexOffset;
					This->libspc=PathLib;
					strcpy(This->relfil, ModName);

                    #ifdef SDK
                    sprintf(This->filename, "%s%s%co", DirLib, ModName, FSEPX);
                    #else /* SDK */
                    sprintf(This->filename, "%s%s%crel", DirLib, ModName, FSEPX);
                    #endif /* SDK */

                    This->str=This->filename;

					ThisSym = This->symbols = (pmlibrarysymbol)malloc( sizeof(mlibrarysymbol));
					ThisSym->next = NULL;
				}
                else if(EQ(FLine, "</INDEX>"))
				{
					return This; /*Finish, get out of here*/
				}
            break;
            case 2:
                if(EQ(FLine, "</MODULE>"))
				{
					This->loaded=0;
					/*Create the index for the next module*/
                    state=1;
				}
                else
				{
					/*Add the symbols*/
					ThisSym->next = (pmlibrarysymbol)malloc(sizeof(mlibrarysymbol));
					ThisSym=ThisSym->next;
					if (ThisSym == NULL)
                    {
						printf("panic: can't allocate memory.\n");
						exit(-2);
					}
					ThisSym->next=NULL;
					strcpy(ThisSym->name, FLine);
                }
            break;
			
			default:
				return This; /*State machine should never reach this point, but just in case...*/
			break;
        }
    }

	return This; /*State machine should never reach this point, but just in case...*/
}


/* buildlibraryindex - build an in-memory cache of the symbols contained in
 *			the libraries
 */
int buildlibraryindex(void)
{
	FILE *libfp, *fp;
	struct lbname *lbnh;
	char relfil[NINPUT+2], *str, *path;
	char buf[NINPUT+2], c;
	char symname[NINPUT+2];
	pmlibraryfile This;
	pmlibrarysymbol ThisSym;

	This=&libr;

	/* Iterate through all library files */
/*1*/	for (lbnh=lbnhead; lbnh; lbnh=lbnh->next) {
		libfp = fopen( lbnh->libspc, "r" );
		if (libfp) {
			path=lbnh->path;

			/*
			 * Read in a line from the library file.
			 * This is the relative file specification
			 * for a .REL file in this library.
			 */

/*2*/			while (fgets(relfil, NINPUT, libfp) != NULL) {
				relfil[NINPUT+1] = '\0';
				chop_crlf(relfil);
				if (path != NULL) {
					str = (char *)new(strlen(path)+strlen(relfil)+6);
					strcpy(str,path);
#ifdef	OTHERSYSTEM
#ifdef SDK
#ifdef UNIX
					if (str[strlen(str)-1] != '/') {
						strcat(str,"/");
#else /* UNIX */
					if (str[strlen(str)-1] != '\\') {
						strcat(str,"\\");
#endif /* UNIX */
#else /* SDK */
					if (str[strlen(str)-1] != '\\') {
						strcat(str,"\\");
#endif /* SDK */
					}
#endif
				} else {
					str = (char *)new(strlen(relfil) + 5);
				}

                if(strcmp(relfil, "<SDCCLIB>")==0)
			    {
                    /*Get the built in index of a library*/
				    This=buildlibraryindex_SdccLib(lbnh->libspc, libfp, str, This);
				    free(str);
				    fclose(libfp);
				    break; /*get the index for next library*/
			    }
                
                /*From here down, build the index for the original library
                format*/
#ifdef SDK
#ifdef UNIX
				if (relfil[0] == '/') {
#else /* UNIX */
				if (relfil[0] == '\\') {
#endif /* UNIX */
#else /* SDK */
				if (relfil[0] == '\\') {
#endif /* SDK */
					strcat(str,relfil+1);
				} else {
					strcat(str,relfil);
				}
				if(strchr(str,FSEPX) == NULL) {
#ifdef SDK
					sprintf(&str[strlen(str)], "%co", FSEPX);
#else /* SDK */
					sprintf(&str[strlen(str)], "%crel", FSEPX);
#endif /* SDK */
				}
/*3*/				if ((fp = fopen(str, "r")) != NULL) {

					/* Opened OK - create a new libraryfile object for it */
					This->next = (pmlibraryfile)new( sizeof( mlibraryfile ));
					if (This->next == NULL) {
						printf("panic: can't allocate memory.\n");
						exit(-1);
					}

					This=This->next;
					This->next = NULL;
					This->loaded=-1;
                    This->offset=-1; /*There should be a rel file*/

					strcpy( This->filename, str );

					ThisSym = This->symbols = (pmlibrarysymbol)malloc( sizeof(mlibrarysymbol));
					ThisSym->next = NULL;

					/*
					 * Read in the object file.  Look for lines that
					 * begin with "S" and end with "D".  These are
					 * symbol table definitions.  If we find one, see
					 * if it is our symbol.  Make sure we only read in
					 * our object file and don't go into the next one.
					 */
			
/*4*/					while (fgets(buf, NINPUT, fp) != NULL) {

					buf[NINPUT+1] = '\0';
					buf[strlen(buf) - 1] = '\0';

					/*
					 * Skip everything that's not a symbol record.
					 */
					if (buf[0] != 'S')
						continue;

					/*
					 * When a 'T line' is found terminate file scan.
					 * All 'S line's preceed 'T line's in .REL files.
					 */
					if (buf[0] == 'T')
						break;

					sscanf(buf, "S %s %c", symname, &c);

					/* If it's an actual symbol, record it */
/*5*/					if (c == 'D') {
						ThisSym->next = (pmlibrarysymbol)malloc(sizeof(mlibrarysymbol));
						ThisSym=ThisSym->next;
						if (ThisSym == NULL) {
							printf("panic: can't allocate memory.\n");
							exit(-2);
						}
						This->loaded=0;
						ThisSym->next=NULL;
						This->str = str;
						strcpy(This->relfil,relfil);
						strcpy(ThisSym->name, symname);
						This->libspc = lbnh->libspc;
					}
				} /* Closes while - read object file */
				fclose(fp);
			} /* Closes if object file opened OK */
		} /* Ends while - processing all in libr */
		fclose(libfp);
	} /* Ends good open of libr file */
	}
	return 0;
}
#else /* INDEXLIB */


/*Check for a symbol in a SDCC library.  If found, add the embedded .rel.
The library must be created with the SDCC librarian 'sdcclib' since the
linking process depends on the correct file offsets embedded in the library
file.*/

int SdccLib(char * PathLib, FILE * libfp, char * DirLib, char * SymName)
{
	struct lbfile *lbfh, *lbf;
	char ModName[NCPS]="";
	char FLine[MAXLINE+1];
	int state=0;
	long IndexOffset=0, FileOffset;

	while(!feof(libfp))
    {
        FLine[0]=0;
        fgets(FLine, MAXLINE, libfp);
        chop_crlf(FLine);

        switch(state)
        {
            case 0:
                if(EQ(FLine, "<INDEX>"))
                {
					/*The next line has the size of the index*/
                    FLine[0]=0;
                    fgets(FLine, MAXLINE, libfp);
                    chop_crlf(FLine);
					IndexOffset=atol(FLine);
					state=1;
                }
            break;
            case 1:
                if(EQ(FLine, "<MODULE>"))
				{
					/*The next line has the name of the module and the offset
					of the corresponding embedded file in the library*/
                    FLine[0]=0;
                    fgets(FLine, MAXLINE, libfp);
                    chop_crlf(FLine);
					sscanf(FLine, "%s %ld", ModName, &FileOffset);
					state=2;
				}
                else if(EQ(FLine, "</INDEX>"))
				{
					/*Reached the end of the index.  The symbol is not in this library.*/
					return 0;
				}
            break;
            case 2:
                if(EQ(FLine, "</MODULE>"))
				{
					/*The symbol is not in this module, try the next one*/
                    state=1;
				}
                else
				{
					/*Check if this is the symbol we are looking for.*/
					if (strncmp(SymName, FLine, NCPS)==0)
					{
						/*The symbol is in this module.*/

						/*As in the original library format, it is assumed that the .rel
						files reside in the same directory as the lib files.*/
						strcat(DirLib, ModName);
                        #ifdef SDK
			            sprintf(&DirLib[strlen(DirLib)], "%co", FSEPX);
                        #else /* SDK */
			            sprintf(&DirLib[strlen(DirLib)], "%crel", FSEPX);
                        #endif /* SDK */

						/*If this module has been loaded already don't load it again.*/
						lbf = lbfhead;
						while (lbf)
						{
							if(EQ(DirLib, lbf->filspc)) return 1;/*Already loaded*/
							lbf=lbf->next;
						}
						
						/*Add the embedded file to the list of files to be loaded in
						the second pass.  That is performed latter by the function
						library() below.*/
						lbfh = (struct lbfile *) new (sizeof(struct lbfile));
						if (lbfhead == NULL)
						{
							lbfhead = lbfh;
						}
						else
						{
							lbf = lbfhead;
							while (lbf->next)
							lbf = lbf->next;
							lbf->next = lbfh;
						}

						lbfh->libspc = PathLib;
						lbfh->filspc = DirLib;
						lbfh->relfil = (char *) new (strlen(ModName) + 1);
						strcpy(lbfh->relfil, ModName);
						/*Library embedded file, so lbfh->offset must be >=0*/
						lbfh->offset = IndexOffset+FileOffset;
						
						/*Jump to where the .rel begins and load it.*/
						fseek(libfp, lbfh->offset, SEEK_SET);
						LoadRel(libfp, ModName);

						return 1; /*Found the symbol, so success!*/
					}
				}
            break;
			
			default:
				return 0; /*It should never reach this point, but just in case...*/
			break;
        }
    }

	return 0; /*The symbol is not in this library*/
}

int
fndsym(name)
char *name;
{
	FILE *libfp, *fp;
	struct lbname *lbnh;
	struct lbfile *lbfh, *lbf;
	char relfil[NINPUT+2];
	char buf[NINPUT+2];
	char symname[NINPUT];
	char *path,*str;
	char c;
	int result;

	/*
	 * Search through every library in the linked list "lbnhead".
	 */

/*1*/	for (lbnh=lbnhead; lbnh; lbnh=lbnh->next) {
		if ((libfp = fopen(lbnh->libspc, "r")) == NULL) {
			fprintf(stderr, "Cannot open library file %s\n",
				lbnh->libspc);
			lkexit(1);
		}
		path = lbnh->path;

		/*
		 * Read in a line from the library file.
		 * This is the relative file specification
		 * for a .REL file in this library.
		 */

/*2*/		while (fgets(relfil, NINPUT, libfp) != NULL) {
		    relfil[NINPUT+1] = '\0';
		    chop_crlf(relfil);
		    if (path != NULL) {
			str = (char *) new (strlen(path)+strlen(relfil)+6);
			strcpy(str,path);
#ifdef	OTHERSYSTEM
#ifdef SDK
#ifdef UNIX
			if (str[strlen(str)-1] != '/') {
				strcat(str,"/");
#else /* UNIX */
			if (str[strlen(str)-1] != '\\') {
				strcat(str,"\\");
#endif /* UNIX */
#else /* SDK */
			if (str[strlen(str)-1] != '\\') {
				strcat(str,"\\");
#endif /* SDK */
			}
#endif
		    } else {
			str = (char *) new (strlen(relfil) + 5);
		    }

            /*See if this is a library with embedded files*/
			if(strcmp(relfil, "<SDCCLIB>")==0)
			{
				result=SdccLib(lbnh->libspc, libfp, str, name);
				fclose(libfp);
				if(result) return(1); /*Found the symbol*/
				free(str);
				/*The symbol is not in the current library,
				check the next library in the list*/
				break; 
			}

			/*From here down is the support for libraries in the original format*/
#ifdef SDK
#ifdef UNIX
		    if (relfil[0] == '/') {
#else /* UNIX */
		    if (relfil[0] == '\\') {
#endif /* UNIX */
#else /* SDK */
		    if (relfil[0] == '\\') {
#endif /* SDK */
			strcat(str,relfil+1);
		    } else {
			strcat(str,relfil);
		    }
		    if(strchr(relfil,FSEPX) == NULL) {
#ifdef SDK
			sprintf(&str[strlen(str)], "%co", FSEPX);
#else /* SDK */
			sprintf(&str[strlen(str)], "%crel", FSEPX);
#endif /* SDK */
		    }
/*3*/		    if ((fp = fopen(str, "r")) != NULL) {

			/*
			 * Read in the object file.  Look for lines that
			 * begin with "S" and end with "D".  These are
			 * symbol table definitions.  If we find one, see
			 * if it is our symbol.  Make sure we only read in
			 * our object file and don't go into the next one.
			 */
			
/*4*/			while (fgets(buf, NINPUT, fp) != NULL) {

			buf[NINPUT+1] = '\0';
			buf[strlen(buf) - 1] = '\0';

			/*
			 * Skip everything that's not a symbol record.
			 */
			if (buf[0] != 'S')
				continue;

			/*
			 * When a 'T line' is found terminate file scan.
			 * All 'S line's preceed 'T line's in .REL files.
			 */
			if (buf[0] == 'T')
				break;

			sscanf(buf, "S %s %c", symname, &c);

			/*
			 * If we find a symbol definition for the
			 * symbol we're looking for, load in the
			 * file and add it to lbfhead so it gets
			 * loaded on pass number 2.
			 */
/*5*/			if (strncmp(symname, name, NCPS) == 0 && c == 'D') {

			lbfh = (struct lbfile *) new (sizeof(struct lbfile));
			if (lbfhead == NULL) {
				lbfhead = lbfh;
			} else {
				lbf = lbfhead;
				while (lbf->next)
					lbf = lbf->next;
				lbf->next = lbfh;
			}
			lbfh->libspc = lbnh->libspc;
			lbfh->filspc = str;
			lbfh->relfil = (char *) new (strlen(relfil) + 1);
			lbfh->offset = -1; /*Stand alone rel file*/
			strcpy(lbfh->relfil,relfil);
			fclose(fp);
			fclose(libfp);
			loadfile(str);
			return (1);

/*5*/			}

/*4*/			}
		    fclose(fp);
/*3*/		    }

		    free(str);
/*2*/		}
		fclose(libfp);
/*1*/	}
	return(0);
}
#endif /* INDEXLIB */

void loadfile_SdccLib(char * libspc, char * module, long offset)
{
	FILE *fp;

#ifdef __CYGWIN__
    ToCygWin(libspc);
#endif

	if ((fp = fopen(libspc,"r")) != NULL)
	{
		fseek(fp, offset, SEEK_SET);
		LoadRel(fp, module);
		fclose(fp);
	}
}

/*)Function	VOID	library()
 *
 *	The function library() links all the library object files
 *	contained in the lbfile structures.
 *
 *	local variables:
 *		lbfile	*lbfh		pointer to lbfile structure
 *
 *	global variables:
 *		lbfile	*lbfhead	pointer to first lbfile structure
 *
 *	 functions called:
 *		VOID	loadfile	lklibr.c
 *
 *	side effects:
 *		Links all files contained in the lbfile structures.
 */

VOID
library()
{
	struct lbfile *lbfh;

	for (lbfh=lbfhead; lbfh; lbfh=lbfh->next)
	{
		if(lbfh->offset<0)
		{
			/*Stand alone rel file (original lib format)*/
			loadfile(lbfh->filspc);
		}
		else
		{
			/*rel file embedded in lib (new lib format)*/
			loadfile_SdccLib(lbfh->libspc, lbfh->relfil, lbfh->offset);
		}
	}
}

/*)Function	VOID	loadfile(filspc)
 *
 *		char	*filspc		library object file specification
 *
 *	The function loadfile() links the library object module.
 *
 *	local variables:
 *		FILE	*fp		file handle
 *		int	i		input line length
 *		char	str[]		file input line
 *
 *	global variables:
 *		char	*ip		pointer to linker input string
 *
 *	 functions called:
 *		int	fclose()	c_library
 *		int	fgets()		c_library
 *		FILE *	fopen()		c_library
 *		VOID	link()		lkmain.c
 *		int	strlen()	c_library
 *
 *	side effects:
 *		If file exists it is linked.
 */

VOID
loadfile(filspc)
char *filspc;
{
	FILE *fp;
	char str[NINPUT+2];
	int i;

#ifdef __CYGWIN__
    ToCygWin(filspc);
#endif

	if ((fp = fopen(filspc,"r")) != NULL) {
		while (fgets(str, NINPUT, fp) != NULL) {
			str[NINPUT+1] = '\0';
			i = strlen(str) - 1;
			if (str[i] == '\n')
				str[i] = '\0';
			ip = str;
			link();
		}
		fclose(fp);
	}
    else
    {
        printf("Couldn't find file %s\n", filspc);
        exit(-4);
    }
}
