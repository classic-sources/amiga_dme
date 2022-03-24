
/*
 * DEFS.H
 *
 *	(C)Copyright 1987 by Matthew Dillon, All Rights Reserved
 *
 *	ARexx support added 03/19/88  -  Kim DeVaughn
 *
 */

#define AREXX 1

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <devices/inputevent.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <intuition/intuition.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  note: if you want Local routines to silently be global, simply
 *  '#define Local' to nothing.
 */

#define Prototype extern
#define Local	  static

#ifdef LATTICE
typedef long size_t;
#endif

#define MAXTOGGLE   256
#define QMOVE	    (0x6B|0x80)

#define COLT(n)  (XTbase + (n) * Xsize)
#define ROWT(n)  (YTbase + (n) * Ysize)
#define COL(n)   (Xbase  + (n) * Xsize)
#define ROW(n)   (Ybase  + (n) * Ysize)

/*
 *  code compatibility... try to phase these usages out.
 */

#define ARGS(args)  args

typedef unsigned char	    ubyte;
typedef unsigned short	    uword;
typedef unsigned long	    ulong;

typedef struct MinNode	    MNODE;
typedef struct Node	    NODE;
typedef struct MinList	    MLIST;
typedef struct List	    LIST;
typedef struct MsgPort	    PORT;
typedef struct Window	    WIN;
typedef struct Message	    MSG;
typedef struct TextAttr     TA;
typedef struct TextFont     FONT;
typedef struct RastPort     RP;
typedef struct IntuiMessage	IMESS;
typedef struct IntuitionBase	IBASE;
typedef struct Menu	    MENU;
typedef struct MenuItem     ITEM;
typedef struct IntuiText    ITEXT;

typedef struct _ED {
    MNODE Node;
    WIN *Win;
    FONT *Font; 	    /*	da font, da font!		    */
    long Topline, Topcolumn;
    long Line, Column;
    long Lines, Maxlines;
    ubyte **List;
    ubyte Name[64];
    ubyte Wtitle[130];
    char Modified;
    long  dirlock;	    /* directory lock			    */
    char iconmode;	    /*	window in icon mode		    */

    /*
     *	CONFIG INFORMATION
     */

    char  BeginConfig;
    ubyte Tabstop;
    ubyte Margin;
    char Insertmode;
    char IgnoreCase;
    char Wordwrap;
    short WWCol;	    /*	word wrap column		    */
    short Winx; 	    /*	save state of non-icon window	    */
    short Winy;
    short Winwidth;
    short Winheight;
    short IWinx, IWiny;     /*	save state of icon window	    */
    int   FGPen;
    int   BGPen;
    int   HGPen;
    int   TPen;
    char  EndConfig;
} ED;

extern long  BSline, BEline;
extern short BSchar, BEchar;
extern ED    *BEp;


#ifndef NULL
#define NULL 0
#endif
#ifdef E
#undef E
#endif

extern MLIST DBase;
extern MLIST PBase;
extern ED    *Ep;

#include "prog-protos.h"

