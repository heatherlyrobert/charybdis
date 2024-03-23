/*============================----beg-of-source---============================*/
#ifndef charybdis
#define charybdis yes


/*===[[ HEADER BEG ]]=========================================================*/
/*                      ┤иииииииии1иииииииии2иииииииии3иииииииии4иииииииии5иииииииии6иииииииии7*/
/*--------- 12345678901 ┤123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_NAME      "charybdis"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_FOCUS     ""                 
#define     P_NICHE     ""                    
#define     P_SUBJECT   "fluxbox desktop pager"
#define     P_PURPOSE   "provide real-time window location and stacking data"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_NAMESAKE  "charybdis-adialeiptos (unceasing)"
#define     P_PRONOUNCE ""
#define     P_HERITAGE  "sea-swallowing, ship-killing whirlpool in the straits of messina"
#define     P_BRIEFLY   "ship-killing whirlpool"
#define     P_IMAGERY   "gigantic sucking vortex of water surrounded by a ring of sharp teeth"
#define     P_REASON    "as the messenger of the mayan spirit"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_HOMEDIR   "/home/system/charybdis.fluxbox_desktop_pager"
#define     P_BASENAME  "charybdis"
#define     P_FULLPATH  "/usr/local/bin/charybdis"
#define     P_SUFFIX    "иии"
#define     P_CONTENT   "иии"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 11.3.0"
#define     P_CODESIZE  "small       ()"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_DEPSTDC   "stdio,string,signal"
#define     P_DEPGRAPH  "иии"
#define     P_DEPCORE   "yURG,yLOG,ySTR"
#define     P_DEPVIKEYS ""
#define     P_DEPOTHER  ""
#define     P_DEPSOLO   "иии"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2024, mar"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_VERMAJOR  "0.--, prototyping"
#define     P_VERMINOR  "0.2-, start turning into real program"
#define     P_VERNUM    "0.2b"
#define     P_VERTXT    "simple add/purge unit tested on stack"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*иииииииии иииииииииии ┤иииииииииииииииииииииииииииии┤ииииииииииииииииииииииииииииииииииииииии*/
/*--------- 12345678901 ┤123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*                      ┤иииииииии1иииииииии2иииииииии3иииииииии4иииииииии5иииииииии6иииииииии7*/
/*===[[ HEADER END ]]=========================================================*/



#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    <yLOG.h>
#include    <yURG.h>
#include    <ySTR.h>

#include    <X11/X.h>        /* X11     standard overall file                 */
#include    <X11/Xlib.h>     /* X11     standard C API                        */


#include    <yX11.h>



extern      char        unit_answer [LEN_RECD];



/*===[[ charybdis_prog.c ]]===================================================*/
/*--------> ----------------------> ------------------------------------------*/
/*---(support)--------------*/
char        PROG_usage              (void);
char*       PROG_version            (void);
/*---(preinit)--------------*/
char        PROG__header            (void);
char        PROG_urgents            (int a_argc, char *a_argv []);
/*---(startup)--------------*/
char        PROG__init              (void);
char        PROG__args              (int a_argc, char *a_argv []);
char        PROG__begin             (void);
char        PROG_startup            (int a_argc, char *a_argv []);
/*---(wrapup)---------------*/
char        PROG__end               (void);
char        PROG_shutdown           (void);
/*---(done)-----------------*/



/*===[[ charybdis_stack.c ]]==================================================*/
/*--------> ----------------------> ------------------------------------------*/
/*---(program)--------------*/
char        stack_purge             (void);
char        stack_init              (void);
/*---(find)-----------------*/
short       stack_by_winid          (long a_winid);
/*---(maintain)-------------*/
char        stack_add               (char a_type, long a_winid);
char        stack_resize            (short n, short x, short y, short w, short t);
char        stack_remove            (int n);
char        stack_insert            (int n);
char        stack_restack           (long a_winid, long a_after);
/*---(report)---------------*/
char        stack_list              (void);
/*---(unittest)-------------*/
char*       stack__unit             (char *a_question, int n);
/*---(done)-----------------*/



#endif
