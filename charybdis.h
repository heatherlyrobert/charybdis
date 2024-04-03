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
#define     P_PRONOUNCE "kaa·rihb·dihs"
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
#define     P_VERMINOR  "0.3-, working but a little unpretty ;)"
#define     P_VERNUM    "0.3f"
#define     P_VERTXT    "improved signals and sizing, movement by hints and names"
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
#include    <time.h>              /* clibc  time related functions            */
#include    <signal.h>              /* ANSI-C    signal handling              */

#include    <yLOG.h>
#include    <yURG.h>
#include    <ySTR.h>

#include    <X11/X.h>        /* X11     standard overall file                 */
#include    <X11/Xlib.h>     /* X11     standard C API                        */
#include    <X11/Xatom.h>
#include    <X11/extensions/shape.h>    /* shape extention -- funky windows    */
#include    <GL/gl.h>        /* OPENGL  standard primary header               */
#include    <GL/glx.h>       /* OPENGL  standard X11 integration              */


#include    <yPARSE.h>
#include    <yEXEC.h>        /* CUSTOM : heatherly process execution          */
#include    <yX11.h>
#include    <yFONT.h>

#include   "yDLST_solo.h"


#define     FILE_THEIA      "/run/theia.ttys"
#define     FILE_CHARYBDIS  "/run/charybdis.ttys"


extern      char        unit_answer [LEN_RECD];
extern      char        g_layout;
extern      char        g_daemon;



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
/*---(runtime)--------------*/
char        PROG_dawn               (void);
/*---(wrapup)---------------*/
char        PROG__end               (void);
char        PROG_shutdown           (void);
/*---(done)-----------------*/



/*===[[ charybdis_stack.c ]]==================================================*/
/*--------> ----------------------> ------------------------------------------*/
/*---(program)--------------*/
char        STACK_purge             (void);
char        STACK_init              (void);
/*---(find)-----------------*/
short       STACK_count             (void);
short       STACK_by_winid          (long a_winid);
char        STACK_by_index          (short a_num, char r_hint [LEN_SHORT], char *r_desk, short *r_left, short *r_topp, short *r_wide, short *r_tall, char r_back [LEN_TERSE], char r_pubname [LEN_LABEL], char r_terse [LEN_LABEL], char *r_type);
char        STACK_by_cursor         (char a_move, char r_hint [LEN_SHORT], char *r_desk, short *r_left, short *r_topp, short *r_wide, short *r_tall, char r_back [LEN_TERSE], char r_pubname [LEN_LABEL], char r_terse [LEN_LABEL], char *r_type);
char        STACK_by_hint           (char a_hint [LEN_SHORT], char a_act, char a_mark);
char        STACK_by_name           (char a_name [LEN_LABEL], char a_act, char a_mark);
char        STACK_by_system         (void);
char*       STACK_pretty            (long a_winid);
/*---(maintain)-------------*/
char        STACK_remove            (short n);
char        STACK__insert           (short n);
char        STACK__copy             (short a_from, short a_to);
char        STACK_add               (char a_type, long a_winid);
char        STACK_push_top          (long a_winid, long a_frame);
char        STACK_restack           (long a_winid, long a_after);
char        STACK_resize            (long a_winid, short x, short y, short w, short t);
char        STACK_redesk            (long a_winid, char d);
char        STACK_eterm             (short a_rpid, short a_ppid, long a_window);
char        STACK_theia             (long a_winid, short a_eterm, char a_back [LEN_TERSE], char a_fore [LEN_TERSE]);
char        STACK_context           (long a_winid);
/*---(import)---------------*/
char        STACK__handler          (int n, uchar a_verb [LEN_TERSE], char a_exist, void *a_handler);
char        STACK_pull              (cchar a_file [LEN_PATH]);
/*---(report)---------------*/
char        STACK_list              (void);
char*       STACK_line              (char a_type, short n);
char        STACK_write             (cchar a_name [LEN_PATH]);
/*---(unittest)-------------*/
char        STACK__unit_location    (short n, char d, short x, short y, short w, short t);
char        STACK__unit_context     (short n, char a_back [LEN_TERSE], char a_pubname [LEN_LABEL], char a_terse [LEN_LABEL], char a_type);
char*       STACK__unit             (char *a_question, int n);
/*---(done)-----------------*/



/*===[[ charybdis_get.c ]]====================================================*/
/*--------> ----------------------> ------------------------------------------*/
char        GET_property            (long a_winid, char a_type, long *r_value, char r_string [LEN_FULL]);
char        GET_single              (char a_desk, long a_parent, long a_curr);
char        GET_populate            (char a_lvl, long a_parent);
char        GET_update              (long a_root, int a_loop);
/*---(done)-----------------*/



/*===[[ charybdis_theia.c ]]==================================================*/
/*--------> ----------------------> ------------------------------------------*/
char        THEIA_init              (void);
char        THEIA__handler          (int n, uchar a_verb [LEN_TERSE], char a_exist, void *a_handler);
char        THEIA_pull              (cchar a_file [LEN_PATH]);
char        THEIA_classify          (cchar a_title [LEN_HUND], cchar a_pubname [LEN_LABEL], cchar a_cmdline [LEN_RECD], char r_note [LEN_LABEL]);
/*---(done)-----------------*/



/*===[[ charybdis_draw.c ]]===================================================*/
/*--------> ----------------------> ------------------------------------------*/
char        DRAW_init               (void);
char        DRAW_wrap               (void);
char        DRAW_sizing             (char a_layout);
char        DRAW_windows            (short a_left, short a_topp, short a_wide, short a_tall, short a_gap);
char        DRAW_context            (short a_left, short a_topp, short a_wide, short a_tall, short a_gap);
char        DRAW_desktops           (short a_left, short a_topp, short a_wide, short a_tall, short a_gap);
char        DRAW_pager              (short a_left, short a_topp, short a_wide, short a_tall);
char        DRAW_main               (char a_layout);
char        DRAW_mask               (short a_left, short a_topp, short a_wide, short a_tall, short a_gap);
/*---(done)-----------------*/



/*===[[ charybdis_draw.c ]]===================================================*/
/*--------> ----------------------> ------------------------------------------*/
char        LOOP_init               (float a_base);
char        LOOP_beg                (void);
char        LOOP_end                (void);
/*---(done)-----------------*/




#endif
