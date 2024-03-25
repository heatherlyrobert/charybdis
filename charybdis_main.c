#include "charybdis.h"



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
   char                   x_wait   =  '-';
   long                   x_root   =    0;
   long                   v        =    0;
   long                   x_curr   =    0;
   short                  x_wwide, x_wtall;
   short                  x_left, x_topp, x_wide, x_tall;
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
   /*---(window size)--------------------*/
   x_wtall = 90 * 8 + 40;
   x_wwide = 265;
   /*---(window size)--------------------*/
   x_left  = x_wwide - 90;
   x_topp  = 20;
   x_wide  = 90;
   x_tall  = x_wtall - 40;
   /*---(create)-------------------------*/
   yX11_start ("charybdis", x_wwide, x_wtall, YX_HIDDEN, YX_FIXED, YX_SILENT);
   yX11_move  (1097,  4);
   DRAW_init  ();
   get_property (YX_ROOT, 'D', &v, NULL);
   printf ("%d\n", v);
   get_property (YX_ROOT, 'A', &v, NULL);
   printf ("%-10x\n", v);
   /*---(gather)-------------------------*/
   x_root = DefaultRootWindow (YX_DISP);
   printf ("%-10x\n", x_root);
   stack_add      ('y', x_root);
   printf ("added\n");
   stack_populate (0, x_root);
   printf ("starting windows\n");
   yEXEC_data_windows (stack_eterm);
   printf ("done with windows\n");
   THEIA_pull     (FILE_THEIA);
   stack_context  ();
   stack_list     ();
   /*---(process)------------------------*/
   DRAW_main  (x_left, x_topp, x_wide, x_tall);
   XSelectInput(YX_DISP, YX_ROOT, PropertyChangeMask | SubstructureNotifyMask );
   while (1) {
      XNextEvent(YX_DISP, &YX_EVNT);
      /*> printf ("received %d\n", YX_EVNT.type);                                     <*/
      switch (YX_EVNT.type) {
      case CreateNotify      : /* new window */
         x_create  = (XCreateWindowEvent *) &YX_EVNT;
         if (x_create->width > 1) {
            stack_add ('y', x_create->window);
            printf ("\ncreate  (%2d) %-10.10s\n", x_create->type, stack_pretty (x_create->window));
            x_wait = 'y';
         } else if (x_wait == 'y') {
            stack_add ('q', x_create->window);
            x_wait = '-';
            /*> n = stack_by_winid (x_create->window);                                <*/
            printf ("\ncreate+ (%2d) %-10.10s\n", x_create->type, stack_pretty (x_create->window));
         }
         break;
      case DestroyNotify     : /* close window */
         x_wait = '-';
         x_destroy  = (XDestroyWindowEvent *) &YX_EVNT;
         n = stack_by_winid (x_destroy->window);
         if (n >= 0) {
            printf ("\ndestroy (%2d) %-10.10s\n", x_destroy->type, stack_pretty (x_destroy->window));
            stack_remove (n);
            stack_list     ();
         }
         break;
      case ConfigureNotify    : /* attribute changes */
         x_wait = '-';
         x_config  = (XConfigureEvent *) &YX_EVNT;
         n = stack_by_winid (x_config->window);
         if (x_config->x > 0) {
            printf ("\nconfig  (%2d) %-10x %-10x %-10.10s  %4dx, %4dy, %4dw, %4dh %-10x\n", x_config->type, x_config->event, x_config->window, stack_pretty (x_config->window), x_config->x, x_config->y, x_config->width, x_config->height, x_config->above);
            /*> printf ("\nrestack (%2d)\n", x_config->type);                         <*/
            stack_resize  (x_config->window, x_config->x, x_config->y, x_config->width, x_config->height);
            stack_restack (x_config->window, x_config->above);
            /*> get_property (YX_ROOT, 'A', &v);                                      <* 
             *> printf ("update active window to %-10x\n", v);                        <* 
             *> stack_restack (v, -1);                                                <*/
            stack_list     ();
         }
         break;
      case PropertyNotify     : /* window manager changes */
         x_wait = '-';
         x_property = (XPropertyEvent *) &YX_EVNT;
         printf ("\nproperty(%2d) %d\n", x_property->type, x_property->atom);
         switch (x_property->atom) {
         case 298 :
            get_property (YX_ROOT, 'D', &v, NULL);
            printf ("switched to desktop %d\n", v);
            get_property (YX_ROOT, 'A', &x_curr, NULL);
            printf ("update active window to %-10x\n", x_curr);
            stack_restack (x_curr, -1);
            get_property (x_curr, 'd', &v, NULL);
            stack_redesk (x_curr, v);
            stack_list     ();
            break;

         }
         break;
      }
      DRAW_main  (x_left, x_topp, x_wide, x_tall);
   }
   /*---(destroy)------------------------*/
   yX11_end   ();
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   yPROG__shutdown ();
   /*---(complete)-----------------------*/
   return 0;
}
