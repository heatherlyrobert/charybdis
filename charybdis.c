#include "charybdis.h"

/*
 * fluxbox adds a frame to windows and configure acts on the 
 * frames, not the winids.
 *
 *
 */


struct {
   long        winid;
   long        frame;
   short       x, y, w, t;
   char        pretty      [LEN_LABEL];
} s_stack [LEN_RECD];
short    s_nstack   = 0;

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
      s_stack [s_nstack].winid = a_winid;
      s_stack [s_nstack].frame = -1;
      s_stack [s_nstack].x     = -1;
      s_stack [s_nstack].y     = -1;
      s_stack [s_nstack].w     = -1;
      s_stack [s_nstack].t     = -1;
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
   int         i           =    0;
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
   int         i           =    0;
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
   ++s_nstack;
   return 0;
}

char
stack_restack           (long a_winid, long a_after)
{
   return 0;
}

char
stack_populate          (char a_lvl, long a_parent)
{
   char        rc          =    0;
   long        x_root      =    0;
   long        x_parent    =    0;
   long       *x_1sts      = NULL;
   uint        x_n1st      =    0;
   uint        x_1st       =    0;
   long        x_curr      =    0;
   char        x_pre       [LEN_LABEL] = "";
   int         i           =    0;
   char        t           [LEN_LABEL] = "";
   char        l           =    0;
   for (i = 0; i < a_lvl; ++i)  strcat (x_pre, "   ");
   rc = XQueryTree (YX_DISP, a_parent, &x_root, &x_parent, &x_1sts, &x_n1st);
   for (x_1st = 0; x_1st < x_n1st; ++x_1st) {
      x_curr = x_1sts [x_1st];
      /*> printf ("%s%3d  %x\n", x_pre, x_1st, x_curr);                               <*/
      if (a_lvl > 0) {
         ystrl4hex ((double) x_curr, t, 4, 'x', LEN_LABEL);
         /*> printf ("CHECKING  %10x  %-10.10s\n", x_curr, t);                <*/
         l = strlen (t);
         if (strcmp ("00040", t + l - 5) == 0) {
            stack_add ('y', x_curr);
            stack_add ('q', a_parent);
            /*> printf ("FOUND FOUND FOUND\n");                                       <*/
         }
      }
      stack_populate (a_lvl + 1, x_curr);
   }
   XFree (x_1sts);
   return 0;
}

char
main                    (void)
{
   XCreateWindowEvent    *x_create;
   XDestroyWindowEvent   *x_destroy;
   XConfigureEvent       *x_config;
   short                  n        =    0;
   char                   x_wait   =  '-';
   long                   x_root   =    0;
   printf ("starting\n");
   yX11_start ("charybdis", 50, 50, YX_HIDDEN, YX_FIXED, YX_SILENT);

   x_root = DefaultRootWindow (YX_DISP);
   stack_populate (0, x_root);
   stack_list     ();

   return 0;

   XSelectInput(YX_DISP, YX_ROOT, PropertyChangeMask | SubstructureNotifyMask );
   while (1) {
      XNextEvent(YX_DISP, &YX_EVNT);
      /*> printf ("received %d\n", YX_EVNT.type);                                     <*/
      switch (YX_EVNT.type) {
      case CreateNotify      : /* new window */
         x_create  = (XCreateWindowEvent *) &YX_EVNT;
         if (x_create->width > 1) {
            stack_add ('y', x_create->window);
            x_wait = 'y';
         } else if (x_wait == 'y') {
            stack_add ('q', x_create->window);
            x_wait = '-';
         }
         break;
      case DestroyNotify     : /* close window */
         x_wait = '-';
         x_destroy  = (XDestroyWindowEvent *) &YX_EVNT;
         n = stack_by_winid (x_destroy->window);
         if (n >= 0) {
            printf ("destroy (%2d) %-10.10s\n", x_destroy->type, s_stack [n].pretty);
         }
         break;
      case ConfigureNotify    : /* attribute changes */
         x_wait = '-';
         x_config  = (XConfigureEvent *) &YX_EVNT;
         n = stack_by_winid (x_config->window);
         if (n >= 0 && x_config->x > 0) {
            stack_resize (n, x_config->x, x_config->y, x_config->width, x_config->height);
            /*> printf ("config  (%2d) %-10x %-10.10s  %4dx, %4dy, %4dw, %4dh %-10x\n", x_config->type, x_config->event, s_stack [n].pretty, x_config->x, x_config->y, x_config->width, x_config->height, x_config->above);   <*/
         }
         break;
      }
   }
   printf ("done\n");
   yX11_end   ();
   return 0;
}
