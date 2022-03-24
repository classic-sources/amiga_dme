
/*
 *  ARP interface (ARPLOAD, ARPSAVE)
 */

#include "defs.h"

Prototype void do_arpinsfile (void);
Prototype void do_arpload (void);
Prototype void do_arpsave (void);
Prototype void fixfile (char *, char *);
Prototype void splitpath (char *, char *, char *);


void
do_arpinsfile()
{
    char file[64];
    char dir[64];
    BPTR oldlock = CurrentDir((BPTR)Ep->dirlock);

    splitpath(Ep->Name, file, dir);
    if (arpreq("Insert File", file, dir, NULL)) {
	CurrentDir(oldlock);
	fixfile(file, dir);
	av[0] = (ubyte *)"i";
	av[1] = (ubyte *)file;
	do_edit();
	return;
    }
    CurrentDir(oldlock);
}

void
do_arpload()
{
    char file[64];
    char dir[64];
    BPTR oldlock = CurrentDir((BPTR)Ep->dirlock);

    splitpath(Ep->Name, file, dir);
    if (arpreq("New File", file, dir, NULL)) {
	BPTR newlock;
	if (newlock = Lock(dir, SHARED_LOCK)) {
	    UnLock(CurrentDir(oldlock));
	    Ep->dirlock = (long)newlock;
	    /*
	    fixfile(file,dir);
	    */
	    av[0] = (ubyte *)"n";
	    av[1] = (ubyte *)file;
	    do_edit();
	    return;
	}
    }
    CurrentDir(oldlock);
}

void
do_arpsave()
{
    char file[64];
    char dir[64];
    BPTR oldlock = CurrentDir((BPTR)Ep->dirlock);

    splitpath(Ep->Name, file, dir);
    if (arpreq("Save File", file, dir, NULL)) {
	CurrentDir(oldlock);
	fixfile(file,dir);
	av[1] = (ubyte *)file;
	do_saveas();
    } else {
	CurrentDir(oldlock);
    }
}

void
fixfile(file,dir)
char *file,*dir;
{
    char *ptr;
    short len = strlen(dir);
    char hasdev = 0;

    /*
     *	do we need to add a slash to the directory spec?
     */

    if (len && dir[len-1] != '/' && dir[len-1] != ':') {
	dir[len++] = '/';
	dir[len] = 0;
    }

    /*
     *	Is file spec really a full path spec?
     */

    for (ptr = file; *ptr; ++ptr) {
	if (ptr[0] == ':')
	    hasdev = 1;
    }
    if (!hasdev) {
	movmem(file,file+len,strlen(file)+1);
	movmem(dir,file,len);
    }
}

/*
 *  Search backwards for first ':' or '/' and split path there.
 *  This subroutine may appear to be coded incorrectly to a novice
 *  programmer.  It isn't [now].
 */

void
splitpath(name, file, dir)
char *name;
char *file, *dir;
{
    short i;

    for (i = strlen(name); i >= 0; --i) {       /* was (incorrectly) "i > 0" */
	if (name[i] == ':' || name[i] == '/')
	    break;
    }
    ++i;
    strcpy(file, name + i);
    movmem(name, dir, i);
    dir[i] = 0;
}

