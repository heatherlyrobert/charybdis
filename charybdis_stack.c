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
   char        r_pubname   [LEN_LABEL];
   char        r_cmdline   [LEN_RECD];
   char        r_type;                      /* window use based on title      */
   char        r_terse     [LEN_LABEL];
   char        r_hint      [LEN_SHORT];     /* two-letter hint code           */
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



/*====================------------------------------------====================*/
/*===----                      critical data moves                     ----===*/
/*====================------------------------------------====================*/
static void      o___WORKERS____________o (void) {;}

char
stack__clear            (short n)
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
   /*---(geometry)----------*/
   s_stack [n].r_desk     = -1;
   s_stack [n].r_left     = -1;
   s_stack [n].r_topp     = -1;
   s_stack [n].r_wide     = -1;
   s_stack [n].r_tall     = -1;
   /*---(done)--------------*/
   return 0;
}

char
stack__duplicate        (short a_dst, short a_src)
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
stack_purge             (void)
{
   int         i           =    0;
   for (i = 0; i < LEN_HUND; ++i)  stack__clear (i);
   s_nstack = 0;
   return 0;
}

char
stack_init              (void)
{
   stack_purge ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         data search                          ----===*/
/*====================------------------------------------====================*/
static void      o___SEARCH_____________o (void) {;}

short
stack_by_winid          (long a_winid)
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



/*====================------------------------------------====================*/
/*===----                         simple moves                         ----===*/
/*====================------------------------------------====================*/
static void      o___MOVES______________o (void) {;}

char
stack_remove            (short n)
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
      stack__duplicate (i - 1, i);
   }
   /*---(increment counter)--------------*/
   --s_nstack;
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
stack__insert           (short n)
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
      stack__duplicate (i + 1, i);
   }
   /*---(clear new entry)----------------*/
   stack__clear (n);
   /*---(increment counter)--------------*/
   ++s_nstack;
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
stack__copy             (short a_from, short a_to)
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
   stack__duplicate (a_to, a_from);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       complex moves                          ----===*/
/*====================------------------------------------====================*/
static void      o___COMPLEX____________o (void) {;}

char
stack_add               (char a_type, long a_winid)
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
      stack__clear (s_nstack);
      s_stack [s_nstack].r_winid = a_winid;
      ystrl4hex ((double) s_stack [s_nstack].r_winid, t, 4, 'x', LEN_LABEL);
      ystrldchg (t, '0', '·', LEN_LABEL);
      if (t [3] == '·')  t [3] = '0';
      if (a_winid != 0x3a4)  ystrlcpy (s_stack [s_nstack].r_pretty, t, LEN_LABEL);
      else                   ystrlcpy (s_stack [s_nstack].r_pretty, "õ··ROOT··", LEN_LABEL);
      /*> printf ("created %-10.10s\n", s_stack [s_nstack].r_pretty);                   <*/
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
stack_push_top          (long a_winid, long a_frame)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   rc = stack__insert (0);
   DEBUG_PROG  yLOG_value   ("insert"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = stack__clear (0);
   DEBUG_PROG  yLOG_value   ("clear"     , rc);
   --rce;  if (rc < 0) {
      rc = stack_remove (0);
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
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
stack_restack           (long a_winid, long a_after)
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
   else               x_tobe = stack_by_winid (a_after);
   DEBUG_PROG  yLOG_value   ("x_tobe"    , x_tobe);
   if (x_tobe < 0)  x_tobe = 0;
   --rce;  if (x_tobe <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (x_tobe >  s_nstack) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = stack__insert (x_tobe);
   DEBUG_PROG  yLOG_value   ("insert"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy entry)---------------------*/
   x_orig = stack_by_winid (a_winid);
   DEBUG_PROG  yLOG_value   ("x_orig"    , x_orig);
   --rce;  if (x_orig <  0) {
      rc = stack_remove (x_tobe);
      DEBUG_PROG  yLOG_value   ("remove*"   , rc);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (x_orig >= s_nstack) {
      rc = stack_remove (x_tobe);
      DEBUG_PROG  yLOG_value   ("remove*"   , rc);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = stack__copy   (x_orig, x_tobe);
   DEBUG_PROG  yLOG_value   ("copy"      , rc);
   --rce;  if (rc < 0) {
      rc = stack_remove (x_tobe);
      DEBUG_PROG  yLOG_value   ("remove*"   , rc);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove old slot)----------------*/
   rc = stack_remove (x_orig);
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
stack_resize            (long a_winid, short x, short y, short w, short t)
{
   char        rce         =  -10;
   short       n           =    0;
   n = stack_by_winid (a_winid);
   --rce;  if (n <  0)         return rce;
   --rce;  if (n >= s_nstack)  return rce;
   s_stack [n].r_left  = x;
   s_stack [n].r_topp  = y;
   s_stack [n].r_wide  = w;
   s_stack [n].r_tall  = t;
   /*> printf ("resize  %-10.10s  %4dx, %4dy, %4dw, %4dh\n", s_stack [n].r_pretty, s_stack [n].r_left, s_stack [n].r_topp, s_stack [n].r_wide, s_stack [n].r_tall);   <*/
   return 0;
}

char
stack_redesk            (long a_winid, char d)
{
   char        rce         =  -10;
   short       n           =    0;
   n = stack_by_winid (a_winid);
   --rce;  if (n <  0)         return rce;
   --rce;  if (n >= s_nstack)  return rce;
   s_stack [n].r_desk     = d;
   /*> printf ("resize  %-10.10s  %4dx, %4dy, %4dw, %4dh\n", s_stack [n].r_pretty, s_stack [n].r_left, s_stack [n].r_topp, s_stack [n].r_wide, s_stack [n].r_tall);   <*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       data reporting                         ----===*/
/*====================------------------------------------====================*/
static void      o___REPORTING__________o (void) {;}

char*
stack_pretty            (long a_winid)
{
   short       n           =    0;
   n = stack_by_winid (a_winid);
   if (n < 0)          return "n/a";
   if (n >= s_nstack)  return "n/a";
   return s_stack [n].r_pretty;
}

char*
stack_line              (char a_type, short n)
{
   sprintf (s_print, "%-6d  %-6.6s  %-2.2s  %-9.9s  %-9.9x  %-2.2s  %1d  %-100.100s  %c  %-20.20s  %4d  %4d  %c  %c  %4d  %4d  %c  %6d  %-10.10s  %s \n",
         s_stack [n].r_eterm, s_stack [n].r_back , s_stack [n].r_fore , s_stack [n].r_pretty , s_stack [n].r_frame ,
         n               , s_stack [n].r_desk , s_stack [n].r_title, s_stack [n].r_type , s_stack [n].r_terse ,
         s_stack [n].r_left , s_stack [n].r_topp , s_stack [n].r_scrn , s_stack [n].r_locn ,
         s_stack [n].r_wide , s_stack [n].r_tall , s_stack [n].r_size ,
         s_stack [n].r_rpid , s_stack [n].r_pubname, s_stack [n].r_cmdline);
   return 0;
}

char
stack_list              (void)
{
   int         i           =    0;
   for (i = 0; i < s_nstack; ++i) {
      printf ("%3d  %-10.10s  %-10.10x  %2dd, %4dx %4dy %4dw %4dt\n", i, s_stack [i].r_pretty, s_stack [i].r_frame, s_stack [i].r_desk, s_stack [i].r_left, s_stack [i].r_topp, s_stack [i].r_wide, s_stack [i].r_tall);
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST___________o (void) {;}

char        unit_answer [LEN_RECD];

char
stack__unit_location    (short n, short x, short y, short w, short t)
{
   s_stack [n].r_left  = x;
   s_stack [n].r_topp  = y;
   s_stack [n].r_wide  = w;
   s_stack [n].r_tall  = t;
   return 0;
}

char*
stack__unit             (char *a_question, int n)
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
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : ··········  ··········     ·x    ·y    ·w    ·t  ·", n);
      } else if (s_stack [n].r_winid < 0) {
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : ··········  ··········     ·x    ·y    ·w    ·t  ·", n);
      } else {
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : %10x  %10x  %4dx %4dy %4dw %4dt  %s", n, s_stack [n].r_winid, s_stack [n].r_frame, s_stack [n].r_left, s_stack [n].r_topp, s_stack [n].r_wide, s_stack [n].r_tall, s_stack [n].r_pretty);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}
