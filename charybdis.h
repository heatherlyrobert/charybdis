/*============================----beg-of-source---============================*/
#ifndef charybdis
#define charybdis yes


/*===[[ HEADER BEG ]]=========================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_NAME      "charybdis"
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     ""                 
#define     P_NICHE     ""                    
#define     P_SUBJECT   "fluxbox desktop pager"
#define     P_PURPOSE   "provide real-time window location and stacking data"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "charybdis-adialeiptos (unceasing)"
#define     P_PRONOUNCE ""
#define     P_HERITAGE  "sea-swallowing, ship-killing whirlpool in the straits of messina"
#define     P_BRIEFLY   "ship-killing whirlpool"
#define     P_IMAGERY   "gigantic sucking vortex of water surrounded by a ring of sharp teeth"
#define     P_REASON    "as the messenger of the mayan spirit"
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/charybdis.fluxbox_desktop_pager"
#define     P_BASENAME  "charybdis"
#define     P_FULLPATH  "/usr/local/bin/charybdis"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 11.3.0"
#define     P_CODESIZE  "small       ()"
/*········· ··········· ´·····························´········································*/
#define     P_DEPSTDC   "stdio,string,stdlib"
#define     P_DEPGRAPH  "X11"
#define     P_DEPCORE   "yURG,yLOG,ySTR"
#define     P_DEPVIKEYS ""
#define     P_DEPOTHER  "yX11,yEXEC"
#define     P_DEPSOLO   "···"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2024, mar"
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "0.--, prototyping"
#define     P_VERMINOR  "0.2-, start turning into real program"
#define     P_VERNUM    "0.2e"
#define     P_VERTXT    "pulls/joins theia data and integrates some theia functions"
/*········· ··········· ´·····························´········································*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*===[[ HEADER END ]]=========================================================*/



#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    <yLOG.h>
#include    <yURG.h>
#include    <ySTR.h>

#include    <X11/X.h>        /* X11     standard overall file                 */
#include    <X11/Xlib.h>     /* X11     standard C API                        */
#include    <X11/Xatom.h>


#include    <yPARSE.h>
#include    <yEXEC.h>        /* CUSTOM : heatherly process execution          */
#include    <yX11.h>

#define     FILE_THEIA    "/run/theia.ttys"


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
char*       stack_pretty            (long a_winid);
/*---(maintain)-------------*/
char        stack_add               (char a_type, long a_winid);
char        stack_remove            (short n);
char        stack__insert           (short n);
char        stack__copy             (short a_from, short a_to);
char        stack_push_top          (long a_winid, long a_frame);
char        stack_restack           (long a_winid, long a_after);
char        stack_resize            (long a_winid, short x, short y, short w, short t);
char        stack_redesk            (long a_winid, char d);
char        stack_eterm             (short a_rpid, short a_ppid, long a_window);
char        stack_theia             (long a_winid, short a_eterm, char a_back [LEN_TERSE], char a_fore [LEN_TERSE]);
char        stack_context           (void);
/*---(report)---------------*/
char        stack_list              (void);
char*       stack_line              (char a_type, short n);
/*---(unittest)-------------*/
char        stack__unit_location    (short n, char d, short x, short y, short w, short t);
char*       stack__unit             (char *a_question, int n);
/*---(done)-----------------*/



char        get_property            (long a_winid, char a_type, long *r_value, char r_string [LEN_FULL]);
char        get_context             (long a_winid, short a_eterm);

char        THEIA_init              (void);
char        THEIA__handler          (int n, uchar a_verb [LEN_TERSE], char a_exist, void *a_handler);
char        THEIA_pull              (cchar a_file [LEN_PATH]);
char        THEIA_classify          (cchar a_title [LEN_HUND], cchar a_pubname [LEN_LABEL], cchar a_cmdline [LEN_RECD], char r_note [LEN_LABEL]);


#endif
