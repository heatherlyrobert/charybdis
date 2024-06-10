#include "charybdis.h"


static char   s_print  [LEN_RECD] = "";

struct {
   /*---(window)------------*/
   long        r_winid;                     /* application window id          */
   long        r_frame;                     /* fluxbox frame for window       */
   char        r_pretty    [LEN_LABEL];     /* cleaned up window id           */
   /*---(eterm specific)----*/
   short       r_eterm;                     /* owning eterm PID               */
   char        r_back      [LEN_TERSE];     /* background terse name          */
   char        r_fore      [LEN_TERSE];     /* foreground refno               */
   char        r_title     [LEN_HUND];      /* current window title           */
   /*---(context)-----------*/
   short       r_rpid;                      /* bottom level pid running       */
   char        r_lvl;                       /* levels of process below eterm  */
   char        r_pubname   [LEN_LABEL];
   char        r_cmdline   [LEN_RECD];
   char        r_type;                      /* window use based on title      */
   char        r_terse     [LEN_LABEL];
   char        r_hint      [LEN_SHORT];     /* two-letter hint code           */
   char        r_mark;                      /* user-marking                   */
   /*---(geometry)----------*/
   char        r_desk;                      /* which desktop                  */
   short       r_left;                      /* leftmost                       */
   short       r_topp;                      /* topmost                        */
   short       r_wide;                      /* width                          */
   short       r_tall;                      /* height                         */
   char        r_locn;                      /* location identifier            */
   char        r_scrn;                      /* screen identifier (t/b)        */
   char        r_size;                      /* size identifier                */
   /*---(done)--------------*/
} s_stack [LEN_HUND];
short    s_nstack   = 0;
short    s_nhint    = 0;



/*====================------------------------------------====================*/
/*===----                      critical data moves                     ----===*/
/*====================------------------------------------====================*/
static void      o___WORKERS____________o (void) {;}

char
STACK__clear            (short n)
{
   /*---(window)------------*/
   s_stack [n].r_winid    = -1;
   s_stack [n].r_frame    =  0;
   strcpy (s_stack [n].r_pretty, "");
   /*---(eterm)-------------*/
   s_stack [n].r_eterm    = -1;
   strcpy (s_stack [n].r_back  , "·");
   strcpy (s_stack [n].r_fore  , "·");
   strcpy (s_stack [n].r_title , "·");
   /*---(context)-----------*/
   s_stack [n].r_rpid     =  0;
   ystrlcpy (s_stack [n].r_pubname, "·", LEN_LABEL);
   ystrlcpy (s_stack [n].r_cmdline, "·", LEN_RECD);
   s_stack [n].r_type    = '·';
   ystrlcpy (s_stack [n].r_terse  , "·", LEN_LABEL);
   ystrlcpy (s_stack [n].r_hint   , "·", LEN_SHORT);
   s_stack [n].r_mark    = '·';
   /*---(geometry)----------*/
   s_stack [n].r_desk     = -1;
   s_stack [n].r_left     = -1;
   s_stack [n].r_topp     = -1;
   s_stack [n].r_wide     = -1;
   s_stack [n].r_tall     = -1;
   s_stack [n].r_locn     = '·';
   s_stack [n].r_scrn     = '·';
   s_stack [n].r_size     = '·';
   /*---(done)--------------*/
   return 0;
}

char
STACK__duplicate        (short a_dst, short a_src)
{
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_senter  (__FUNCTION__);
   DEBUG_PROG  yLOG_svalue  ("a_dst"     , a_dst);
   DEBUG_PROG  yLOG_svalue  ("a_src"     , a_src);
   /*---(window)------------*/
   s_stack [a_dst].r_winid  = s_stack [a_src].r_winid;
   s_stack [a_dst].r_frame  = s_stack [a_src].r_frame;
   ystrlcpy (s_stack [a_dst].r_pretty , s_stack [a_src].r_pretty , LEN_LABEL);
   /*---(eterm specific)----*/
   s_stack [a_dst].r_eterm  = s_stack [a_src].r_eterm;
   ystrlcpy (s_stack [a_dst].r_back   , s_stack [a_src].r_back   , LEN_TERSE);
   ystrlcpy (s_stack [a_dst].r_fore   , s_stack [a_src].r_fore   , LEN_TERSE);
   ystrlcpy (s_stack [a_dst].r_title  , s_stack [a_src].r_title  , LEN_HUND);
   /*---(context)-----------*/
   s_stack [a_dst].r_rpid   = s_stack [a_src].r_rpid;
   ystrlcpy (s_stack [a_dst].r_pubname, s_stack [a_src].r_pubname, LEN_LABEL);
   ystrlcpy (s_stack [a_dst].r_cmdline, s_stack [a_src].r_cmdline, LEN_RECD);
   s_stack [a_dst].r_type   = s_stack [a_src].r_type;
   ystrlcpy (s_stack [a_dst].r_terse  , s_stack [a_src].r_terse  , LEN_LABEL);
   ystrlcpy (s_stack [a_dst].r_hint   , s_stack [a_src].r_hint   , LEN_SHORT);
   s_stack [a_dst].r_mark   = s_stack [a_src].r_mark;
   /*---(geometry)----------*/
   s_stack [a_dst].r_desk   = s_stack [a_src].r_desk;
   s_stack [a_dst].r_left   = s_stack [a_src].r_left;
   s_stack [a_dst].r_topp   = s_stack [a_src].r_topp;
   s_stack [a_dst].r_wide   = s_stack [a_src].r_wide;
   s_stack [a_dst].r_tall   = s_stack [a_src].r_tall;
   s_stack [a_dst].r_locn   = s_stack [a_src].r_locn;
   s_stack [a_dst].r_scrn   = s_stack [a_src].r_scrn;
   s_stack [a_dst].r_size   = s_stack [a_src].r_size;
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM____________o (void) {;}

char
STACK_purge             (void)
{
   int         i           =    0;
   for (i = 0; i < LEN_HUND; ++i)  STACK__clear (i);
   s_nstack = 0;
   s_nhint  = 0;
   return 0;
}

char
STACK_init              (void)
{
   STACK_purge ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         data search                          ----===*/
/*====================------------------------------------====================*/
static void      o___SEARCH_____________o (void) {;}

short STACK_count       (void) { return s_nstack; }

short
STACK_by_winid          (long a_winid)
{
   int         i           =    0;
   char        x_found     =  '-';
   for (i = 0; i < s_nstack; ++i) {
      if (s_stack [i].r_winid == a_winid) x_found = 'y';
      if (s_stack [i].r_frame == a_winid) x_found = 'y';
      if (x_found != 'y')  continue;
      return i;
   }
   return -1;
}

char
STACK_by_index          (short a_num, char r_hint [LEN_SHORT], char *r_desk, short *r_left, short *r_topp, short *r_wide, short *r_tall, char r_back [LEN_TERSE], char r_pubname [LEN_LABEL], char r_terse [LEN_LABEL], char *r_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (r_hint    != NULL)  strcpy (r_hint   , "");
   if (r_desk    != NULL)  *r_desk    = -1;
   if (r_left    != NULL)  *r_left    = -1;
   if (r_topp    != NULL)  *r_topp    = -1;
   if (r_wide    != NULL)  *r_wide    = -1;
   if (r_tall    != NULL)  *r_tall    = -1;
   if (r_back    != NULL)  strcpy (r_back   , "");
   if (r_pubname != NULL)  strcpy (r_pubname, "");
   if (r_terse   != NULL)  strcpy (r_terse  , "");
   if (r_type    != NULL)  *r_type    = '·';
   /*---(defense)------------------------*/
   DEBUG_YDLST  yLOG_sint    (s_nstack);
   --rce;  if (s_nstack <= 0) {
      DEBUG_YDLST  yLOG_snote   ("nothing in stack");
      DEBUG_YDLST  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   n = a_num;
   /*---(bounce off ends)----------------*/
   if (n < 0) {
      n  =  0;
      rc =  0;
   }
   else if (n >= s_nstack) {
      n  =  s_nstack - 1;
      rc =  0;
   } else {
      rc =  1;
   }
   /*---(save-back)----------------------*/
   if (rc == 1) {
      if (r_hint    != NULL)  ystrlcpy (r_hint   , s_stack [n].r_hint   , LEN_SHORT);
      if (r_desk    != NULL)  *r_desk    = s_stack [n].r_desk;
      if (r_left    != NULL)  *r_left    = s_stack [n].r_left;
      if (r_topp    != NULL)  *r_topp    = s_stack [n].r_topp;
      if (r_wide    != NULL)  *r_wide    = s_stack [n].r_wide;
      if (r_tall    != NULL)  *r_tall    = s_stack [n].r_tall;
      if (r_back    != NULL)  ystrlcpy (r_back   , s_stack [n].r_back   , LEN_TERSE);
      if (r_pubname != NULL)  ystrlcpy (r_pubname, s_stack [n].r_pubname, LEN_LABEL);
      if (r_terse   != NULL)  ystrlcpy (r_terse  , s_stack [n].r_terse  , LEN_LABEL);
      if (r_type    != NULL)  *r_type    = s_stack [n].r_type;
   }
   /*---(normal result)------------------*/
   DEBUG_PROG  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STACK_by_cursor         (char a_move, char r_hint [LEN_SHORT], char *r_desk, short *r_left, short *r_topp, short *r_wide, short *r_tall, char r_back [LEN_TERSE], char r_pubname [LEN_LABEL], char r_terse [LEN_LABEL], char *r_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   static int  n           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (r_hint    != NULL)  strcpy (r_hint   , "");
   if (r_desk    != NULL)  *r_desk    = -1;
   if (r_left    != NULL)  *r_left    = -1;
   if (r_topp    != NULL)  *r_topp    = -1;
   if (r_wide    != NULL)  *r_wide    = -1;
   if (r_tall    != NULL)  *r_tall    = -1;
   if (r_back    != NULL)  strcpy (r_back   , "");
   if (r_pubname != NULL)  strcpy (r_pubname, "");
   if (r_terse   != NULL)  strcpy (r_terse  , "");
   if (r_type    != NULL)  *r_type    = '·';
   /*---(defense)------------------------*/
   DEBUG_YDLST  yLOG_sint    (s_nstack);
   --rce;  if (s_nstack <= 0) {
      DEBUG_YDLST  yLOG_snote   ("nothing in stack");
      DEBUG_YDLST  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(switch)-------------------------*/
   DEBUG_YDLST  yLOG_schar   (a_move);
   DEBUG_YDLST  yLOG_sint    (n);
   --rce;  switch (a_move) {
   case YDLST_HEAD : case YDLST_DHEAD :
      n = 0;
      break;
   case YDLST_PREV : case YDLST_DPREV :
      --n;
      break;
   case YDLST_CURR : case YDLST_DCURR :
      n;
      break;
   case YDLST_NEXT : case YDLST_DNEXT :
      ++n;
      break;
   case YDLST_TAIL : case YDLST_DTAIL :
      n = s_nstack - 1;
      break;
   default         :
      DEBUG_YDLST  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YDLST  yLOG_sint    (n);
   /*---(bounce off ends)----------------*/
   if (n < 0) {
      n  =  0;
      rc =  0;
   }
   else if (n >= s_nstack) {
      n  =  s_nstack - 1;
      rc =  0;
   } else {
      rc =  1;
   }
   /*---(save-back)----------------------*/
   if (rc == 1) {
      if (r_hint    != NULL)  ystrlcpy (r_hint   , s_stack [n].r_hint   , LEN_SHORT);
      if (r_desk    != NULL)  *r_desk    = s_stack [n].r_desk;
      if (r_left    != NULL)  *r_left    = s_stack [n].r_left;
      if (r_topp    != NULL)  *r_topp    = s_stack [n].r_topp;
      if (r_wide    != NULL)  *r_wide    = s_stack [n].r_wide;
      if (r_tall    != NULL)  *r_tall    = s_stack [n].r_tall;
      if (r_back    != NULL)  ystrlcpy (r_back   , s_stack [n].r_back   , LEN_TERSE);
      if (r_pubname != NULL)  ystrlcpy (r_pubname, s_stack [n].r_pubname, LEN_LABEL);
      if (r_terse   != NULL)  ystrlcpy (r_terse  , s_stack [n].r_terse  , LEN_LABEL);
      if (r_type    != NULL)  *r_type    = s_stack [n].r_type;
   }
   /*---(normal result)------------------*/
   DEBUG_PROG  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STACK_by_hint           (char a_hint [LEN_SHORT], char a_act, char a_mark)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   DEBUG_CONF  yLOG_info    ("a_hint"    , a_hint);
   --rce;  if (a_hint == NULL) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   for (i = 0; i < s_nstack; ++i) {
      DEBUG_CONF  yLOG_complex ("check"     , "%3d, %s", i, s_stack [i].r_hint);
      if (strcmp (s_stack [i].r_hint, a_hint) != 0)  continue;
      DEBUG_CONF  yLOG_note    ("FOUND");
      n = i;
      break;
   }
   DEBUG_CONF  yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("r_pretty"  , s_stack [n].r_pretty);
   DEBUG_CONF  yLOG_hex     ("r_winid"   , s_stack [n].r_winid);
   rc = yX11_win_goto (s_stack [n].r_winid);
   DEBUG_CONF  yLOG_value   ("wingoto"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STACK_by_name           (char a_name [LEN_LABEL], char a_act, char a_mark)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   int         i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   DEBUG_CONF  yLOG_info    ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = strlen (a_name);
   for (i = 0; i < s_nstack; ++i) {
      DEBUG_CONF  yLOG_complex ("check"     , "%3d, %s", i, s_stack [i].r_terse);
      if (strncmp (s_stack [i].r_terse, a_name, l) != 0)  continue;
      DEBUG_CONF  yLOG_note    ("FOUND");
      n = i;
      break;
   }
   DEBUG_CONF  yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("r_pretty"  , s_stack [n].r_pretty);
   DEBUG_CONF  yLOG_hex     ("r_winid"   , s_stack [n].r_winid);
   DEBUG_CONF  yLOG_value   ("a_act"     , a_act);
   DEBUG_CONF  yLOG_value   ("a_mark"    , a_mark);
   switch (a_act) {
   case 'g' :
      rc = yX11_win_goto (s_stack [n].r_winid);
      DEBUG_CONF  yLOG_value   ("wingoto"   , rc);
      break;
   case 'm' :
      s_stack [n].r_mark = a_mark;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STACK_by_system         (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   for (i = 0; i < s_nstack; ++i) {
      DEBUG_CONF  yLOG_complex ("check"     , "%3d, %2d, %s", i, s_stack [i].r_desk, s_stack [i].r_pretty);
      if (s_stack [i].r_desk  >= 0)  continue;
      if (strcmp (s_stack [i].r_pretty, "õ··ROOT··") == 0)  continue;
      DEBUG_CONF  yLOG_note    ("FOUND");
      n = i;
      break;
   }
   DEBUG_CONF  yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("r_pretty"  , s_stack [n].r_pretty);
   DEBUG_CONF  yLOG_hex     ("r_winid"   , s_stack [n].r_winid);
   rc = yX11_win_goto (s_stack [n].r_winid);
   DEBUG_CONF  yLOG_value   ("wingoto"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         simple moves                         ----===*/
/*====================------------------------------------====================*/
static void      o___MOVES______________o (void) {;}

char
STACK_remove            (short n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG  yLOG_value   ("n"         , n);
   --rce;  if (n <  0) {
      DEBUG_PROG  yLOG_note    ("n negative");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (n >= s_nstack) {
      DEBUG_PROG  yLOG_note    ("n too large");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(pull entries down)--------------*/
   for (i = 0; i < s_nstack; ++i) {
      if (i <= n)  continue;
      STACK__duplicate (i - 1, i);
   }
   /*---(increment counter)--------------*/
   --s_nstack;
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STACK__insert           (short n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG  yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_PROG  yLOG_note    ("n negative");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (n > s_nstack) {
      DEBUG_PROG  yLOG_note    ("n too large");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(pull entries up)----------------*/
   for (i = s_nstack - 1; i >= 0; --i) {
      if (i < n)  continue;
      STACK__duplicate (i + 1, i);
   }
   /*---(clear new entry)----------------*/
   STACK__clear (n);
   /*---(increment counter)--------------*/
   ++s_nstack;
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STACK__copy             (short a_from, short a_to)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (a_from <  0) {
      DEBUG_PROG  yLOG_note    ("n too large");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_from >= s_nstack) {
      DEBUG_PROG  yLOG_note    ("n too large");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_to   <  0) {
      DEBUG_PROG  yLOG_note    ("a_to negative");
      DEBUG_PROG  yLOG_note    ("n too large");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_to   >= s_nstack) {
      DEBUG_PROG  yLOG_note    ("a_to too large");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy to new location)-----------*/
   DEBUG_PROG  yLOG_note    ("copy to new location");
   STACK__duplicate (a_to, a_from);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       complex moves                          ----===*/
/*====================------------------------------------====================*/
static void      o___COMPLEX____________o (void) {;}

char
STACK_add               (char a_type, long a_winid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG  yLOG_value   ("a_winid"   , a_winid);
   --rce;  if (a_winid ==  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (a_type == 'y') {
      DEBUG_PROG  yLOG_note    ("primary window id add");
      STACK__clear (s_nstack);
      s_stack [s_nstack].r_winid = a_winid;
      ystrl4hex ((double) s_stack [s_nstack].r_winid, t, 4, 'x', LEN_LABEL);
      ystrldchg (t, '0', '·', LEN_LABEL);
      if (t [3] == '·')  t [3] = '0';
      if (a_winid != 0x3a4)  ystrlcpy (s_stack [s_nstack].r_pretty, t, LEN_LABEL);
      else                   ystrlcpy (s_stack [s_nstack].r_pretty, "õ··ROOT··", LEN_LABEL);
      /*> printf ("created %-10.10s\n", s_stack [s_nstack].r_pretty);                   <*/
      ystrlhint (s_nhint, "ll", s_stack [s_nstack].r_hint);
      ++s_nhint;
      ++s_nstack;
      DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   }
   else if (a_type == 'q') {
      DEBUG_PROG  yLOG_note    ("secondary frame id add");
      s_stack [s_nstack - 1].r_frame = a_winid;
      /*> printf ("  r_frame %x\n", a_winid);                                           <*/
   }
   else {
      DEBUG_PROG  yLOG_note    ("add type not legal");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STACK_push_top          (long a_winid, long a_frame)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   rc = STACK__insert (0);
   DEBUG_PROG  yLOG_value   ("insert"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = STACK__clear (0);
   DEBUG_PROG  yLOG_value   ("clear"     , rc);
   --rce;  if (rc < 0) {
      rc = STACK_remove (0);
      DEBUG_PROG  yLOG_value   ("remove*"   , rc);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(push to top)--------------------*/
   DEBUG_PROG  yLOG_note    ("add to top");
   s_stack [0].r_winid = a_winid;
   ystrl4hex ((double) s_stack [0].r_winid, t, 4, 'x', LEN_LABEL);
   ystrldchg (t, '0', '·', LEN_LABEL);
   if (t [3] == '·')  t [3] = '0';
   ystrlcpy (s_stack [0].r_pretty, t, LEN_LABEL);
   s_stack [0].r_frame = a_frame;
   ystrlhint (s_nhint, "ll", s_stack [0].r_hint);
   ++s_nhint;
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STACK_restack           (long a_winid, long a_after)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       x_orig      =    0;
   short       x_tobe      =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(create slot)--------------------*/
   DEBUG_PROG  yLOG_value   ("a_after"   , a_after);
   if (a_after == 0)  x_tobe = 0;
   else               x_tobe = STACK_by_winid (a_after);
   DEBUG_PROG  yLOG_value   ("x_tobe"    , x_tobe);
   if (x_tobe < 0)  x_tobe = 0;
   --rce;  if (x_tobe <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   if (x_tobe >= s_nstack)  x_tobe = s_nstack - 2;
   --rce;  if (x_tobe >  s_nstack) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = STACK__insert (x_tobe);
   DEBUG_PROG  yLOG_value   ("insert"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy entry)---------------------*/
   x_orig = STACK_by_winid (a_winid);
   DEBUG_PROG  yLOG_value   ("x_orig"    , x_orig);
   --rce;  if (x_orig <  0) {
      rc = STACK_remove (x_tobe);
      DEBUG_PROG  yLOG_value   ("remove*"   , rc);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (x_orig >= s_nstack) {
      rc = STACK_remove (x_tobe);
      DEBUG_PROG  yLOG_value   ("remove*"   , rc);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = STACK__copy   (x_orig, x_tobe);
   DEBUG_PROG  yLOG_value   ("copy"      , rc);
   --rce;  if (rc < 0) {
      rc = STACK_remove (x_tobe);
      DEBUG_PROG  yLOG_value   ("remove*"   , rc);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove old slot)----------------*/
   rc = STACK_remove (x_orig);
   DEBUG_PROG  yLOG_value   ("remove"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         data updates                         ----===*/
/*====================------------------------------------====================*/
static void      o___UPDATE_____________o (void) {;}

char
STACK_resize            (long a_winid, short x, short y, short w, short t)
{
   char        rce         =  -10;
   short       n           =    0;
   n = STACK_by_winid (a_winid);
   --rce;  if (n <  0)         return rce;
   --rce;  if (n >= s_nstack)  return rce;
   s_stack [n].r_left  = x;
   s_stack [n].r_topp  = y;
   s_stack [n].r_wide  = w;
   s_stack [n].r_tall  = t;
   yX11_loc_categorize (x, y, w, t, &(s_stack [n].r_locn), &(s_stack [n].r_scrn), &(s_stack [n].r_size));
   return 0;
}

char
STACK_redesk            (long a_winid, char d)
{
   char        rce         =  -10;
   short       n           =    0;
   n = STACK_by_winid (a_winid);
   --rce;  if (n <  0)         return rce;
   --rce;  if (n >= s_nstack)  return rce;
   s_stack [n].r_desk     = d;
   return 0;
}

char
STACK_eterm             (short a_rpid, short a_ppid, long a_window)
{
   short       n           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   DEBUG_PROG  yLOG_value   ("a_rpid"    , a_rpid);
   DEBUG_PROG  yLOG_value   ("a_ppid"    , a_ppid);
   DEBUG_PROG  yLOG_value   ("a_window"  , a_window);
   n = STACK_by_winid (a_window);
   DEBUG_PROG  yLOG_value   ("n"         , n);
   if (n < 0) {
      DEBUG_PROG  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_PROG  yLOG_value   ("r_rpid"    , s_stack [n].r_rpid);
   if (s_stack [n].r_rpid  >  0) {
      DEBUG_PROG  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   s_stack [n].r_rpid  = a_rpid;
   s_stack [n].r_eterm = a_ppid;
   DEBUG_PROG  yLOG_value   ("r_rpid"    , s_stack [n].r_rpid);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
STACK_theia             (long a_winid, short a_eterm, char a_back [LEN_TERSE], char a_fore [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       n           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   DEBUG_PROG  yLOG_value   ("a_winid"   , a_winid);
   n = STACK_by_winid (a_winid);
   DEBUG_PROG  yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_note    ("match found");
   /*---(verify)-------------------------*/
   --rce;  if (s_stack [n].r_eterm <= 0) {
      DEBUG_PROG  yLOG_note    ("adding eterm data");
   } else if (a_eterm == s_stack [n].r_eterm) {
      DEBUG_PROG  yLOG_note    ("already have right eterm");
   } else {
      DEBUG_PROG  yLOG_note    ("differing eterms, serious issue");
   }
   /*---(save-back)----------------------*/
   s_stack [n].r_eterm = a_eterm;
   ystrlcpy (s_stack [n].r_back, a_back, LEN_TERSE);
   ystrlcpy (s_stack [n].r_fore, a_fore, LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
STACK_context           (long a_winid)
{
   char        rc          =    0;
   int         i           =    0;
   int         x_lvl       =    0;
   int         x_use       =    0;
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   DEBUG_PROG  yLOG_hex     ("a_winid"   , a_winid);
   for (i = 0; i < s_nstack; ++i) {
      DEBUG_PROG  yLOG_complex ("review"    , "%3d  %-9x  %-9x", i, s_stack [i].r_pretty, s_stack [i].r_frame);
      if (a_winid == 0 || s_stack [i].r_winid == a_winid) {
         DEBUG_PROG  yLOG_note    ("HANDLING");
         rc = GET_property (s_stack [i].r_winid, 't', NULL, s_stack [i].r_title);
         DEBUG_PROG  yLOG_info    ("title"     , s_stack [i].r_title);
         DEBUG_PROG  yLOG_value   ("eterm"     , s_stack [i].r_eterm);
         if (a_winid != 0 && s_stack [i].r_eterm <= 0) {
            DEBUG_PROG  yLOG_note    ("do not know eterm yet, wait longer");
            DEBUG_PROG  yLOG_exit    (__FUNCTION__);
            return 0;
         }
         rc = yEXEC_find_eterm_use (s_stack [i].r_eterm, &x_lvl, &x_use, s_stack [i].r_pubname, s_stack [i].r_cmdline);
         DEBUG_PROG  yLOG_complex ("use"       , "%d", x_use);
         s_stack [i].r_lvl  = x_lvl;
         s_stack [i].r_rpid = x_use;
         s_stack [i].r_type = THEIA_classify (s_stack [i].r_title, s_stack [i].r_pubname, s_stack [i].r_cmdline, s_stack [i].r_terse);
         if (a_winid != 0) {
            DEBUG_PROG  yLOG_note    ("did the one required, done");
            DEBUG_PROG  yLOG_exit    (__FUNCTION__);
            return 1;
         }
      }
   }
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 1;
}


/*====================------------------------------------====================*/
/*===----                       data reporting                         ----===*/
/*====================------------------------------------====================*/
static void      o___REPORTING__________o (void) {;}

char*
STACK_pretty            (long a_winid)
{
   short       n           =    0;
   n = STACK_by_winid (a_winid);
   if (n < 0)          return "n/a";
   if (n >= s_nstack)  return "n/a";
   return s_stack [n].r_pretty;
}

char*
STACK_line              (char a_type, short n)
{
   if (n < 0) sprintf (s_print, "stk ht  winid----  frame-  -d left topp wide tall  G S L  ht  eterm  bg---- fg  lv -run- pubname----- ---terse------------ t ht m");
   else       sprintf (s_print, "%3d %-2.2s  %-9.9s  %-6.6x  %2d %4d %4d %4d %4d  %c %c %c  %-2.2s  %5d  %-6.6s %-2.2s  %2d %5d %-12.12s %-20.20s %c %-2.2s %c", n, s_stack [n].r_hint, s_stack [n].r_pretty, s_stack [n].r_frame, s_stack [n].r_desk, s_stack [n].r_left, s_stack [n].r_topp, s_stack [n].r_wide, s_stack [n].r_tall, s_stack [n].r_locn, s_stack [n].r_scrn, s_stack [n].r_size, s_stack [n].r_hint, s_stack [n].r_eterm, s_stack [n].r_back, s_stack [n].r_fore, s_stack [n].r_lvl, s_stack [n].r_rpid, s_stack [n].r_pubname, s_stack [n].r_terse, s_stack [n].r_type, s_stack [n].r_hint, s_stack [n].r_mark);
   return s_print;
}

char
STACK_list              (void)
{
   int         i           =    0;
   char        x_dwin      [8] = { 0, 0, 0, 0, 0, 0, 0, 0};
   char        x_heart     [LEN_HUND]  = "";
   int         v           =    0;
   int         e           =    0;
   int         u           =    0;
   int         l           =    0;
   /*---(heartbeat)----------------------*/
   yEXEC_heartbeat (getpid (), 0, NULL, NULL, x_heart);
   /*---(file header)--------------------*/
   printf ("#!/usr/local/bin/charybdis --list\n");
   printf ("## %s %s\n", P_NAMESAKE, P_HERITAGE);
   printf ("#@ source       åcharybdisæ\n");
   printf ("#@ subject      åeterm window inventory listæ\n");
   printf ("#@ heartbeat    å%sæ\n", x_heart);
   printf ("#@ parse      20åÏ--··Ï-··Ï--------··Ï-----··Ï-·Ï---·Ï---·Ï---·Ï---··Ï·Ï·Ï··Ï-··Ï----··Ï-----·Ï-··Ï-·Ï----·Ï-----------·Ï-------------------·Ï·Ï- Ïæ\n");
   printf ("#@ titles     20åstk··ht··winid······frame···d··left·topp·wide·tall··G·S·L··ht··eterm··bg·····fg··lv·rpid··pubname······terse················T·ht·Mæ\n");
   for (i = 0; i < s_nstack; ++i) {
      if (i % 15 == 0)  printf ("\n##  ht  winid----  frame-  -d left topp wide tall  G S L  ht  eterm  bg---- fg  lv rpid- ---pubname-- ---terse------------ T ht M\n");
      if (i %  5 == 0)  printf ("\n");
      printf ("%3d %-2.2s  %-9.9s  %-6.6x  %2d %4d %4d %4d %4d  %c %c %c  %-2.2s  %5d  %-6.6s %-2.2s  %2d %5d %-12.12s %-20.20s %c %-2.2s %c\n", i, s_stack [i].r_hint, s_stack [i].r_pretty, s_stack [i].r_frame, s_stack [i].r_desk, s_stack [i].r_left, s_stack [i].r_topp, s_stack [i].r_wide, s_stack [i].r_tall, s_stack [i].r_locn, s_stack [i].r_scrn, s_stack [i].r_size, s_stack [i].r_hint, s_stack [i].r_eterm, s_stack [i].r_back, s_stack [i].r_fore, s_stack [i].r_lvl, s_stack [i].r_rpid, s_stack [i].r_pubname, s_stack [i].r_terse, s_stack [i].r_type, s_stack [i].r_hint, s_stack [i].r_mark);
      if (s_stack [i].r_desk >= 0)  ++(x_dwin [s_stack [i].r_desk]);
      switch (s_stack [i].r_type) {
      case 'V' :  ++v;  break;
      case 'E' :  ++e;  break;
      case 'U' :  ++u;  break;
      case 'L' :  ++l;  break;
      }
   }
   printf ("\n## count by desktop  ");
   for (i = 0; i < 8; ++i) {
      if (x_dwin [i] != 0)   printf ("%1d=%2d  ", i, x_dwin [i]);
      else                   printf ("%1d=··  " , i);
   }
   printf ("\n## count by type     E=%2d, V=%2d, U=%2d, L=%2d\n", e, v, u, l);
   /*---(file footer)--------------------*/
   printf ("\n## end-of-report.  %d lines.  done, finito, completare, whimper [Ï´···\n", s_nstack);
   return 0;
}

char
STACK_write           (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   int         i           =    0;
   char        x_heart     [LEN_HUND]  = "";
   char        t           [LEN_LABEL] = "";
   char        s           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   /*---(heartbeat)----------------------*/
   yEXEC_heartbeat (getpid (), 0, NULL, NULL, x_heart);
   /*---(open file)----------------------*/
   f = fopen (a_name, "wt");
   DEBUG_FILE   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(file header)--------------------*/
   fprintf (f, "#!/usr/local/bin/charybdis\n");
   fprintf (f, "##   %s %s\n", P_NAMESAKE, P_HERITAGE);
   fprintf (f, "##   last update on å%sæ\n", x_heart);
   fprintf (f, "\n");
   /*---(read terminal)------------------*/
   for (i = 0; i < s_nstack; ++i) {
      if (i % 15 == 0)  fprintf (f, "\n###  ht  --winid--  -fram-  -d  ---x  ---y  ---w  ---t  -  -  -  eterm  bg----  fg  -run-  ---use------  t  ---terse------------ \n");
      if (i %  5 == 0)  fprintf (f, "\n");
      fprintf (f, "%3d  %-2.2s  %-9.9s  %-6.6x  %2d  %4d  %4d  %4d  %4d  %c  %c  %c  %5d  %-6.6s  %-2.2s  %5d  %-12.12s  %c  %-20.20s \n", i, s_stack [i].r_hint, s_stack [i].r_pretty, s_stack [i].r_frame, s_stack [i].r_desk, s_stack [i].r_left, s_stack [i].r_topp, s_stack [i].r_wide, s_stack [i].r_tall, s_stack [i].r_locn, s_stack [i].r_scrn, s_stack [i].r_size, s_stack [i].r_eterm, s_stack [i].r_back, s_stack [i].r_fore, s_stack [i].r_rpid, s_stack [i].r_pubname, s_stack [i].r_type, s_stack [i].r_terse);
   }
   /*---(file footer)--------------------*/
   fprintf (f, "\n");
   fprintf (f, "## end-of-file.  %d lines.  done, finito, completare, whimper [Ï´···\n", i);
   /*---(close file)---------------------*/
   fflush (f);
   rc = fclose (f);
   --rce;  if (rc != 0) {
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       importing file                         ----===*/
/*====================------------------------------------====================*/
static void      o___IMPORT_____________o (void) {;}

char
STACK__handler          (int n, uchar a_verb [LEN_TERSE], char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_hint      [LEN_TERSE] = "";
   char        x_winid     [LEN_TERSE] = "";
   long        x_window    =    0;
   char        x_terse     [LEN_LABEL] = "";
   char        x_desk      [LEN_SHORT] = "";
   double      v           =  0.0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(dispatch)-----------------------*/
   DEBUG_CONF  yLOG_point   ("a_verb"     , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("a_verb"     , a_verb);
   /*---(read details)-------------------*/
   rc = yPARSE_scanf (a_verb, "TT-S-------------L"  , x_hint, x_winid, x_desk, x_terse);
   DEBUG_CONF  yLOG_value   ("scanf"      , rc);
   if (rc < 0) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("x_winid"    , x_winid);
   ystrlcpy (s_stack [s_nstack].r_pretty, x_winid, LEN_TERSE);
   ystrldchg (x_winid, '·', '0', LEN_TERSE);
   ystrl2hex (x_winid, &v, LEN_TERSE);
   DEBUG_CONF  yLOG_info    ("x_winid"    , x_winid);
   DEBUG_CONF  yLOG_hex     ("v"          , v);
   s_stack [s_nstack].r_winid = v;
   ystrlcpy (s_stack [s_nstack].r_hint , x_hint, LEN_SHORT);
   DEBUG_CONF  yLOG_info    ("x_hint"     , x_hint);
   DEBUG_CONF  yLOG_info    ("x_desk"     , x_desk);
   s_stack [s_nstack].r_desk  = atoi (x_desk);
   ystrlcpy (s_stack [s_nstack].r_terse, x_terse, LEN_LABEL);
   DEBUG_CONF  yLOG_info    ("x_terse"    , x_terse);
   ++s_nstack;
   DEBUG_CONF  yLOG_value   ("s_nstack"   , s_nstack);
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STACK_pull              (cchar a_file [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*> yURG_msg ('>', "reading word file...");                                        <*/
   /*---(purge the tables)---------------*/
   rc = yPARSE_reset_in ();
   DEBUG_CONF   yLOG_value   ("purge_in"  , rc);
   /*---(defense)------------------------*/
   DEBUG_CONF  yLOG_point   ("a_file"     , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("a_file"     , a_file);
   /*---(read all lines)-----------------*/
   rc = yPARSE_autoread (a_file, NULL, STACK__handler);
   DEBUG_CONF  yLOG_value   ("read"      , rc);
   DEBUG_CONF  yLOG_value   ("s_nstack"  , s_nstack);
   /*---(close)--------------------------*/
   rc = yPARSE_close ();
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed closing configuration file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST___________o (void) {;}

char        unit_answer [LEN_RECD];

char
STACK__unit_location    (short n, char d, short x, short y, short w, short t)
{
   s_stack [n].r_desk  = d;
   s_stack [n].r_left  = x;
   s_stack [n].r_topp  = y;
   s_stack [n].r_wide  = w;
   s_stack [n].r_tall  = t;
   yX11_loc_categorize (x, y, w, t, &(s_stack [n].r_locn), &(s_stack [n].r_scrn), &(s_stack [n].r_size));
   return 0;
}

char
STACK__unit_context     (short n, char a_back [LEN_TERSE], char a_pubname [LEN_LABEL], char a_terse [LEN_LABEL], char a_type)
{
   ystrlcpy (s_stack [n].r_back   , a_back   , LEN_TERSE);
   ystrlcpy (s_stack [n].r_pubname, a_pubname, LEN_LABEL);
   ystrlcpy (s_stack [n].r_terse  , a_terse  , LEN_LABEL);
   s_stack [n].r_type  = a_type;
   return 0;
}

char*
STACK__unit             (char *a_question, int n)
{
   /*---(locals)-----------+-----------+-*/
   int         c           =    0;
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "STACK            : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "STACK count      : %d", s_nstack);
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      if (n >= s_nstack) {
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : ··  ·······  ······   ·d    ·x    ·y    ·w    ·t  · · ·  ·", n);
      } else if (s_stack [n].r_winid < 0) {
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : ··  ·······  ······   ·d    ·x    ·y    ·w    ·t  · · ·  ·", n);
      } else {
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : %-2.2s  %7x  %6x  %2dd %4dx %4dy %4dw %4dt  %c %c %c  %s", n, s_stack [n].r_hint, s_stack [n].r_winid, s_stack [n].r_frame, s_stack [n].r_desk, s_stack [n].r_left, s_stack [n].r_topp, s_stack [n].r_wide, s_stack [n].r_tall, s_stack [n].r_locn, s_stack [n].r_scrn, s_stack [n].r_size, s_stack [n].r_pretty);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}
