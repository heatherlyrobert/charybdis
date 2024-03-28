#include "charybdis.h"

/*
 *  MASSIVE TRICKS FOR FLUXBOX
 *     fluxbox keeps its own stacking order private !  (X11 is not quite right)
 *     so, always end a loop with asking fluxbox what's on top (and force it ;)
 *
 *
 */


char
main               (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =   0;           /* generic return code            */
   short       n           =   0;
   XCreateWindowEvent    *x_create;
   XDestroyWindowEvent   *x_destroy;
   XConfigureEvent       *x_config;
   XPropertyEvent        *x_property;
   XCirculateEvent       *x_circulate;
   char                   x_wait   =  '-';
   long                   x_root   =    0;
   long                   v        =    0;
   long                   x_curr   =    0;
   short                  x_wwide, x_wtall;
   short                  x_left, x_topp, x_wide, x_tall;
   int         cs          =    0;
   int         c           =    0;
   char                   x_desk;
   long                   x_cdesk;
   long                   x_prev   =   -1; 
   int         x_loop      =    0;
   int         x_delay     =   -1;
   /*---(preprare)-----------------------*/
   rc = PROG_urgents (a_argc, a_argv );
   DEBUG_PROG   yLOG_value   ("urgents"   , rc);
   --rce;  if (rc <  0) {
      PROG_shutdown  ();
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = PROG_startup (a_argc, a_argv );
   DEBUG_PROG   yLOG_value   ("startup"   , rc);
   --rce;  if (rc <  0) {
      PROG_shutdown  ();
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(gather)-------------------------*/
   yX11_start_unit ();
   x_root = DefaultRootWindow (YX_DISP);
   STACK_add      ('y', x_root);
   GET_populate   (0, x_root);
   yEXEC_data_windows (STACK_eterm);
   THEIA_pull     (FILE_THEIA);
   STACK_context  (0);
   STACK_write    (FILE_CHARYBDIS);
   /*> STACK_list     ();                                                             <*/
   if (g_layout == '-')  return 0;
   /*---(window size)--------------------*/
   /*> x_wtall = 90 * 8 + 36;                                                         <* 
    *> x_wwide = 240;                                                                 <*/
   /*---(window size)--------------------*/
   /*> x_wide  = 80;                                                                  <* 
    *> x_left  = x_wwide - x_wide;                                                    <* 
    *> x_topp  = 20;                                                                  <* 
    *> x_tall  = x_wtall - 40;                                                        <*/
   /*---(create)-------------------------*/
   /*> yX11_start ("charybdis", x_wwide, x_wtall, YX_HIDDEN, YX_FIXED, YX_SILENT);    <* 
    *> yX11_move  (1097,  4);                                                         <*/
   DRAW_sizing (g_layout);
   DRAW_init  ();
   GET_property (YX_ROOT, 'D', &v, NULL);
   /*> printf ("%d\n", v);                                                            <*/
   GET_property (YX_ROOT, 'A', &v, NULL);
   /*> printf ("%-10x\n", v);                                                         <*/
   /*---(process)------------------------*/
   /*> DRAW_main  (0, x_wtall, x_wwide, x_wtall);                                     <*/
   DRAW_main  (g_layout);
   LOOP_init (0.1);
   XSelectInput(YX_DISP, YX_ROOT, PropertyChangeMask | SubstructureNotifyMask );
   while (1) {
      LOOP_beg ();
      DEBUG_PROG   yLOG_value   ("LOOP"      , x_loop);
      while (XPending(YX_DISP)) {
         ++c;
         XNextEvent(YX_DISP, &YX_EVNT);
         GET_property (YX_ROOT, 'D', &x_cdesk, NULL);
         /*> printf ("\n%4d : received (%d) on %d\n", c, YX_EVNT.type, x_cdesk);      <*/
         switch (YX_EVNT.type) {
         case CreateNotify      : /* new window */
            x_create  = (XCreateWindowEvent *) &YX_EVNT;
            if (x_create->width > 1) {
               /*> STACK_add ('y', x_create->window);                                 <*/
               x_prev = x_create->window;
               /*> printf ("       create   (%2d) %-10.10s\n", x_create->type, STACK_pretty (x_create->window));   <*/
               x_wait = 'y';
            } else if (x_wait == 'y') {
               STACK_push_top (x_prev, x_create->window);
               GET_single     (x_cdesk, x_prev, x_create->window);
               /*> STACK_context  (x_prev);                                           <*/
               ++c;
               x_wait  = '-';
               x_delay =  10;
               /*> x_prev = -1;                                                       <*/
               /*> printf ("       create+  (%2d) %-10.10s\n", x_create->type, STACK_pretty (x_create->window));   <*/
            }
            break;
         case DestroyNotify     : /* close window */
            x_wait = '-';
            x_destroy  = (XDestroyWindowEvent *) &YX_EVNT;
            /*> printf ("       destroy  (%2d) %-10.10s\n", x_destroy->type, STACK_pretty (x_destroy->window));   <*/
            n = STACK_by_winid (x_destroy->window);
            if (n >= 0)  STACK_remove (n);
            /*> STACK_list ();                                                        <*/
            break;
         case ConfigureNotify    : /* attribute changes */
            x_wait = '-';
            x_config  = (XConfigureEvent *) &YX_EVNT;
            n = STACK_by_winid (x_config->window);
            if (n >= 0)  STACK_by_index (n, NULL, &x_desk, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            /*> printf ("       config   (%2d) %-10x %-10x %2dn %-10.10s  %2dd, %4dx, %4dy, %4dw, %4dh %-10x\n", x_config->type, x_config->event, x_config->window, n, STACK_pretty (x_config->window), x_desk, x_config->x, x_config->y, x_config->width, x_config->height, x_config->above);   <*/
            if (n >= 0 && x_desk == x_cdesk && x_config->width > 0) {
               STACK_resize  (x_config->window, x_config->x, x_config->y, x_config->width, x_config->height);
               STACK_restack (x_config->window, x_config->above);
            }
            break;
         case PropertyNotify     : /* window manager changes */
            x_wait = '-';
            x_property = (XPropertyEvent *) &YX_EVNT;
            switch (x_property->atom) {
            case 298 :
               GET_property (YX_ROOT, 'D', &v, NULL);
               /*> printf ("       switched to desktop %d\n", v);                     <*/
               GET_property (YX_ROOT, 'A', &x_curr, NULL);
               GET_property  (x_curr, 'd', &v, NULL);
               STACK_redesk  (x_curr, v);
               break;
            case 340 :
               /*> printf ("       nothing to do (%-10x)\n",  x_property->window);    <*/
               break;
            default  :
               /*> printf ("       do not undertand (%2d) %d\n", x_property->type, x_property->atom);   <*/
               break;
            }
            break;
         }
         GET_property (YX_ROOT, 'A', &x_curr, NULL);
         STACK_restack (x_curr, -1);
      }
      /*> if (x_loop > 0 && x_loop % 10 == 0)  GET_update (x_root, x_loop);           <*/
      THEIA_pull     (FILE_THEIA);
      if (x_loop > 0 && x_loop % 10 == 0) {
         DEBUG_PROG  yLOG_note    ("update current");
         STACK_context  (x_curr);
         ++c;
      }
      if (x_delay == 0) {
         DEBUG_PROG  yLOG_note    ("update newest");
         rc = STACK_context  (x_prev);
         DEBUG_PROG  yLOG_value   ("update"    , rc);
         if (rc == 0)  x_delay == 5;
         else          ++c;
      }
      if (x_delay >= 0)  --x_delay;
      /*> if (x_loop > 0 && x_loop % 10 == 0) { STACK_context  (0); ++c; }            <*/
      /*> if (c > cs)  DRAW_main  (0, x_wtall, x_wwide, x_wtall);                     <*/
      if (c > cs)  DRAW_main  (g_layout);
      STACK_write    (FILE_CHARYBDIS);
      cs = c;
      /*> STACK_list     ();                                                          <*/
      ++x_loop;
      LOOP_end ();
   }
   /*---(destroy)------------------------*/
   DRAW_wrap  ();
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   yPROG__shutdown ();
   /*---(complete)-----------------------*/
   return 0;
}
