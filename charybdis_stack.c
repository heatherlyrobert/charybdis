#include "charybdis.h"



struct {
   long        winid;
   long        frame;
   short       x, y, w, t;
   char        pretty      [LEN_LABEL];
} s_stack [LEN_RECD];
short    s_nstack   = 0;

char
stack__clear            (short n)
{
   s_stack [n].winid = -1;
   s_stack [n].frame =  0;
   s_stack [n].x     = -1;
   s_stack [n].y     = -1;
   s_stack [n].w     = -1;
   s_stack [n].t     = -1;
   strcpy (s_stack [n].pretty, "");
   return 0;
}

char
stack_purge             (void)
{
   int         i           =    0;
   for (i = 0; i < LEN_RECD; ++i)  stack__clear (i);
   s_nstack = 0;
   return 0;
}

char
stack_init              (void)
{
   stack_purge ();
   return 0;
}

char
stack_list              (void)
{
   int         i           =    0;
   for (i = 0; i < s_nstack; ++i) {
      printf ("%3d  %-10.10s  %x\n", i, s_stack [i].pretty, s_stack [i].frame);
   }
   return 0;
}


short
stack_by_winid          (long a_winid)
{
   int         i           =    0;
   char        x_found     =  '-';
   for (i = 0; i < s_nstack; ++i) {
      if (s_stack [i].winid == a_winid) x_found = 'y';
      if (s_stack [i].frame == a_winid) x_found = 'y';
      if (x_found != 'y')  continue;
      return i;
   }
   return -1;
}

char
stack_add               (char a_type, long a_winid)
{
   char        t           [LEN_LABEL] = "";
   if (a_type == 'y') {
      stack__clear (s_nstack);
      s_stack [s_nstack].winid = a_winid;
      ystrl4hex ((double) s_stack [s_nstack].winid, t, 4, 'x', LEN_LABEL);
      ystrldchg (t, '0', '·', LEN_LABEL);
      if (t [3] == '·')  t [3] = '0';
      ystrlcpy (s_stack [s_nstack].pretty, t, LEN_LABEL);
      /*> printf ("created %-10.10s\n", s_stack [s_nstack].pretty);                   <*/
      ++s_nstack;
   }
   if (a_type == 'q') {
      s_stack [s_nstack - 1].frame = a_winid;
      /*> printf ("  frame %x\n", a_winid);                                           <*/
   }
   return 0;
}

char
stack_resize            (short n, short x, short y, short w, short t)
{
   s_stack [n].x     = x;
   s_stack [n].y     = y;
   s_stack [n].w     = w;
   s_stack [n].t     = t;
   printf ("resize  %-10.10s  %4dx, %4dy, %4dw, %4dh\n", s_stack [n].pretty, s_stack [n].x, s_stack [n].y, s_stack [n].w, s_stack [n].t);
   return 0;
}

char
stack_remove            (int n)
{
   char        rce         =  -10;
   int         i           =    0;
   --rce;  if (n < 0)          return rce;
   --rce;  if (n >= s_nstack)  return rce;
   for (i = 0; i < s_nstack; ++i) {
      if (i <= n)  continue;
      s_stack [i - 1].winid  = s_stack [i].winid;
      s_stack [i - 1].frame  = s_stack [i].frame;
      s_stack [i - 1].x      = s_stack [i].x;
      s_stack [i - 1].y      = s_stack [i].y;
      s_stack [i - 1].w      = s_stack [i].w;
      s_stack [i - 1].t      = s_stack [i].t;
      ystrlcpy (s_stack [i - 1].pretty, s_stack [i].pretty, LEN_LABEL);
   }
   --s_nstack;
   return 0;
}

char
stack_insert            (int n)
{
   char        rce         =  -10;
   int         i           =    0;
   --rce;  if (n < 0)         return rce;
   --rce;  if (n > s_nstack)  return rce;
   for (i = s_nstack - 1; i >= 0; --i) {
      if (i < n)  continue;
      s_stack [i + 1].winid  = s_stack [i].winid;
      s_stack [i + 1].frame  = s_stack [i].frame;
      s_stack [i + 1].x      = s_stack [i].x;
      s_stack [i + 1].y      = s_stack [i].y;
      s_stack [i + 1].w      = s_stack [i].w;
      s_stack [i + 1].t      = s_stack [i].t;
      ystrlcpy (s_stack [i + 1].pretty, s_stack [i].pretty, LEN_LABEL);
   }
   stack__clear (n);
   ++s_nstack;
   return 0;
}

char
stack_copy              (int a_from, int a_to)
{
   char        rce         =  -10;
   --rce;  if (a_from <  0)         return rce;
   --rce;  if (a_from >= s_nstack)  return rce;
   --rce;  if (a_to   <  0)         return rce;
   --rce;  if (a_to   >= s_nstack)  return rce;
   s_stack [a_to].winid  = s_stack [a_from].winid;
   s_stack [a_to].frame  = s_stack [a_from].frame;
   s_stack [a_to].x      = s_stack [a_from].x;
   s_stack [a_to].y      = s_stack [a_from].y;
   s_stack [a_to].w      = s_stack [a_from].w;
   s_stack [a_to].t      = s_stack [a_from].t;
   ystrlcpy (s_stack [a_to].pretty, s_stack [a_from].pretty, LEN_LABEL);
   return 0;
}

char
stack_restack           (long a_winid, long a_after)
{
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char        unit_answer [LEN_RECD];

char
stack__unit_location    (short n, short x, short y, short w, short t)
{
   s_stack [n].x      = x;
   s_stack [n].y      = y;
   s_stack [n].w      = w;
   s_stack [n].t      = t;
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
      } else if (s_stack [n].winid < 0) {
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : ··········  ··········     ·x    ·y    ·w    ·t  ·", n);
      } else {
         snprintf (unit_answer, LEN_HUND, "STACK entry (%2d) : %10x  %10x  %4dx %4dy %4dw %4dt  %s", n, s_stack [n].winid, s_stack [n].frame, s_stack [n].x, s_stack [n].y, s_stack [n].w, s_stack [n].t, s_stack [n].pretty);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}
