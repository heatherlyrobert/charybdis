#include "charybdis.h"

/*
 * fluxbox adds a frame to windows and configure acts on the 
 * frames, not the winids.
 *
 *
 */


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
