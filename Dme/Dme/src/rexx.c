
/*
 *  REXX.C
 *
 *	(c) Copyright 1987 by Kim DeVaughn, All Rights Reserved
 *
 *  ARexx interface code, etc.
 *
 */

#include "defs.h"

Prototype void openrexx (void);
Prototype void closerexx (void);
Prototype void do_rx (void);
Prototype void do_rx1 (void);
Prototype void do_rx2 (void);
Prototype void do_rxImplied (char *, char *);
Prototype int do_rexx (char *);


#if AREXX

#include "rexx/storage.h"
#include "rexx/rxslib.h"
#include "rexx/rexxio.h"
#include "rexx/errors.h"
#include "rexx.h"


int foundcmd;	    /* control for implicit ARexx macro invocation   */
int cmderr;	    /* global command error flag for do_rexx()'s use */

__stkargs APTR CreateRexxMsg ARGS((PORT *, char *, char *));
__stkargs void DeleteRexxMsg ARGS((struct RexxMsg *));
__stkargs APTR CreateArgstring ARGS((char *, int));
__stkargs void DeleteArgstring ARGS((struct RexxArg *));
__stkargs int IsRexxMsg ARGS((struct RexxMsg *));
__stkargs void InitPort ARGS((PORT *, char *));
__stkargs void FreePort ARGS((PORT *));

struct RxsLib *RexxSysBase;

/*
 * initialization for ARexx ... just open rexsyslib.library
 */

void
openrexx()
{
    RexxSysBase = (struct RxsLib *)OpenLibrary("rexxsyslib.library", (ULONG)RXSVERS);
    return;
}



/*
 * cleanup any open ARexx stuff ...  just close rexsyslib.library for now
 */

void
closerexx()
{
    if (RexxSysBase)
	CloseLibrary((struct Library *)RexxSysBase);
}



/*
 *  explicit invocation interface between do_command() and do_rexx
 *  for ARexx macros having NO arguments (i.e., for the "rx" command)
 */

void
do_rx()
{
    do_rexx(av[1]);
}



/*
 *  explicit invocation interface between do_command() and do_rexx
 *  for ARexx macros having ONE argument (i.e., for the "rx1" command)
 */

void
do_rx1()
{
    char macbuf[256];

    strcpy(macbuf, av[1]);
    strcat(macbuf, " ");
    strcat(macbuf, av[2]);
    do_rexx(macbuf);
}



/*
 *  explicit invocation interface between do_command() and do_rexx
 *  for ARexx macros having TWO arguments (i.e., for the "rx2" command)
 */

void
do_rx2()
{
    char macbuf[256];

    strcpy(macbuf, av[1]);
    strcat(macbuf, " ");
    strcat(macbuf, av[2]);
    strcat(macbuf, " ");
    strcat(macbuf, av[3]);
    do_rexx(macbuf);
}



/*
 *  implicit invocation interface between do_command() and do_rexx
 *  for ARexx macros implicitly called; arbitrary number of arguments
 */

void
do_rxImplied(cmd, args)
char *cmd;
char *args;
{
    char macbuf[256];

    strcpy(macbuf, cmd);
    strcat(macbuf, " ");
    strcat(macbuf, args);
    do_rexx(macbuf);
}

/*
 *  issue a command to ARexx ...
 */

int
do_rexx(macstr)
char *macstr;
{
    struct RexxArg *macarg;

    struct MsgPort  RexxPort;
    struct MsgPort *ARexxPort;

    struct RexxMsg *macptr;
    struct RexxMsg *cmdptr;

    char host[16];
    char hexbuf[12];	    /* should only need 9 bytes */
    char errmsg[80];	    /* don't build a larger error message */

    int  ret;
    int  err;


    if (RexxSysBase == 0) {
	title("Unknown Command   -   No Macros:  ARexx Not Installed ");   /* no rexxsyslib */
	return(0);
    }

    clrmem(&RexxPort, sizeof(struct MsgPort));
    strcpy(host, "DME");
    sprintf(hexbuf, "%08x", &RexxPort);
    strcat(host, hexbuf);
    InitPort(&RexxPort, host);      /* need to error check this */
    AddPort(&RexxPort);
    /* return here if InitPort failed */


    if (macarg = (struct RexxArg *)CreateArgstring(macstr, strlen(macstr))) {
	if (macptr = (struct RexxMsg *)CreateRexxMsg(&RexxPort, "dme", host)) {
	    ACTION(macptr) = RXCOMM;
	    ARG0(macptr)   = (STRPTR)macarg;

	    Forbid();
	    if (ARexxPort = (struct MsgPort *)FindPort("REXX")) {
		PutMsg(ARexxPort, (MSG *)macptr);
		Permit();
		title("Calling ARexx Macro ... ");

		for (;;) {
		    WaitPort(&RexxPort);
		    cmdptr = (struct RexxMsg *)GetMsg(&RexxPort);

		    if (IsRexxMsg(cmdptr)) {

			foundcmd = 0;
			cmderr = CMD_INITIAL;
			ret = do_command(ARG0(cmdptr));
			err = cmderr;
			if (foundcmd) {
			    ret = (ret == 1) ? 0 : 5;   /* cmd error:  RC = 5  */
			} else {
			    ret = do_rexx(ARG0(cmdptr));    /* another macro? */
			}

			RESULT1(cmdptr) = ret;
			RESULT2(cmdptr) = 0;
			ReplyMsg((MSG *)cmdptr);
		    }
		    do_command("null");     /* a kludge to set "foundcmd" */
		    if (macptr == cmdptr) break;
		}


		if (ret = RESULT1(cmdptr)) {
		    if (RESULT2(cmdptr)) {
			if (RESULT2(cmdptr) == 1) {
			    title("Unknown Command ");
			} else {
			    sprintf(errmsg, "ARexx Macro Error:  Code = %d  Severity = %d ", RESULT2(cmdptr), ret);
			    title(errmsg);
			}
		    } else {
			sprintf(errmsg, "User Specified Macro Error:  RC = %d ", ret);
			title(errmsg);
		    }
		} else {
		    if (err <= TITLE_THRESHHOLD) {
			title("OK ");
		    }
		}
		ret = ret + err;
	    } else {
		Permit();
		title("Unknown Command   -   No Macros:  ARexx Not Active ");   /* no REXX port */

		ret = -1;
	    }
	    DeleteRexxMsg(macptr);
	} else {
	    title("CreateRexxMsg() Failed ");   /* may be overkill, and not need to ckeck this */
	    ret = -1;
	}
	DeleteArgstring(macarg);
    } else {
	title("CreateArgstring() Failed ");     /* may be overkill, and not need to check this */
	ret = -1;
    }

    RemPort(&RexxPort);
    FreePort(&RexxPort);
    return(ret);
}

#endif

