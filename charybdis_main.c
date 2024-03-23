#include "charybdis.h"



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
         /*> if (n >= 0) {                                                                  <* 
          *>    printf ("destroy (%2d) %-10.10s\n", x_destroy->type, s_stack [n].pretty);   <* 
          *> }                                                                              <*/
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
