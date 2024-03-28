#include "charybdis.h"

/*
 * fluxbox adds a frame to windows and configure acts on the 
 * frames, not the winids.
 *
 *
 */

char
GET_property            (long a_winid, char a_type, long *r_value, char r_string [LEN_FULL])
{
   char        rce         =  -10;
   char        rc          =    0;
   char        x_exp_prop  [LEN_TITLE] = "";
   Atom        x_act_prop;
   Atom        x_exp_type;
   Atom        x_act_type;
   int x_format;
   unsigned long c;
   unsigned long x_after;
   unsigned long a;
   unsigned char *x_return;
   char *t;
   /*---(header)-------------------------*/
   DEBUG_DATA  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_value  != NULL)  *r_value = -1;
   if (r_string != NULL)  strcpy (r_string, "");
   /*---(prepare)------------------------*/
   DEBUG_DATA  yLOG_char    ("a_type"    , a_type);
   --rce;  switch (a_type) {
   case 'D' :  /* current desktop */
      x_exp_type = XA_CARDINAL;
      strcpy (x_exp_prop, "_NET_CURRENT_DESKTOP");
      break;
   case 'd' :  /* window desktop */
      x_exp_type = XA_CARDINAL;
      strcpy (x_exp_prop, "_NET_WM_DESKTOP");
      break;
   case 'A' :  /* active window */
      x_exp_type = XA_WINDOW;
      strcpy (x_exp_prop, "_NET_ACTIVE_WINDOW");
      break;
   case 't' :  /* window title */
      x_exp_type = XA_STRING;
      strcpy (x_exp_prop, "WM_NAME");
      break;
   default  :
      DEBUG_DATA  yLOG_note    ("type unknown");
      DEBUG_DATA  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(convert property)---------------*/
   DEBUG_DATA  yLOG_info    ("x_exp_prop"    , x_exp_prop);
   x_act_prop = XInternAtom (YX_DISP, x_exp_prop, False);
   DEBUG_DATA  yLOG_value   ("x_act_prop"    , x_act_prop);
   /*---(request dat)--------------------*/
   rc = XGetWindowProperty (YX_DISP, a_winid, x_act_prop, 0, LEN_RECD / 4, False,
         x_exp_type, &x_act_type, &x_format, &c, &x_after, &x_return);
   DEBUG_DATA  yLOG_value   ("return"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA  yLOG_note    ("could not retrieve property");
      DEBUG_DATA  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check return type)--------------*/
   DEBUG_DATA  yLOG_value   ("x_exp_type", x_exp_type);
   DEBUG_DATA  yLOG_value   ("x_act_type", x_act_type);
   --rce;  if (x_act_type != x_exp_type) {
      XFree (x_return);
      DEBUG_DATA  yLOG_note    ("type mismatch");
      DEBUG_DATA  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA  yLOG_value   ("x_format"  , x_format);
   DEBUG_DATA  yLOG_value   ("x_after"   , x_after);
   DEBUG_DATA  yLOG_point   ("x_return"  , x_return);
   /*---(translate)----------------------*/
   DEBUG_DATA  yLOG_value   ("c"         , c);
   a = (x_format / (32 / sizeof (long))) * c;
   DEBUG_DATA  yLOG_value   ("a"         , a);
   t = malloc (a + 1);
   memcpy(t, x_return, a);
   t [a] = '\0';
   XFree (x_return);
   /*---(save-back)----------------------*/
   switch (a_type) {
   case 'D'  : case 'd'  :
      if (r_value  != NULL)  *r_value = t [0];
      break;
   case 'A'  :
      if (r_value  != NULL)  *r_value = *((Window *) t);
      break;
   case 't' :
      if (r_string != NULL)  ystrlcpy (r_string, t, LEN_FULL);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
GET_single              (char a_desk, long a_parent, long a_curr)
{
   long        v           =    0;
   int         x, y, j;
   uint        w, t;
   long        x_long;
   STACK_redesk   (a_curr, a_desk);
   XGetGeometry   (YX_DISP, a_curr, &x_long, &x, &y, &w, &t, &j, &j);
   STACK_resize   (a_curr, x, y, w, t);
   /*> printf ("%-9x  %-9x  %4dx  %4dy  %4dw  %4dt\n", a_parent, a_curr, x, y, w, t);   <*/
   /*> STACK_list ();                                                                 <*/
   return 0;
}

char
GET_populate            (char a_lvl, long a_parent)
{
   char        rc          =    0;
   long        x_root      =    0;
   long        x_parent    =    0;
   long       *x_1sts      = NULL;
   uint        x_n1st      =    0;
   uint        x_1st       =    0;
   long        x_curr      =    0;
   int         i           =    0;
   char        x_temp      [LEN_LABEL] = "";
   char        l           =    0;
   long        v           =    0;
   int         x, y, j;
   uint        w, t;
   long        x_long;
   short       n           =    0;
   rc = XQueryTree (YX_DISP, a_parent, &x_root, &x_parent, &x_1sts, &x_n1st);
   for (x_1st = 0; x_1st < x_n1st; ++x_1st) {
      x_curr = x_1sts [x_1st];
      if (a_lvl > 0) {
         ystrl4hex ((double) x_curr, x_temp, 4, 'x', LEN_LABEL);
         /*> printf ("CHECKING  %10x  %-10.10s\n", x_curr, x_temp);                <*/
         l = strlen (x_temp);
         if (strcmp ("00040", x_temp + l - 5) == 0) {
            n = STACK_by_winid (x_curr);
            if (n < 0) {
               STACK_push_top (x_curr, a_parent);
               GET_property   (x_curr, 'd', &v, NULL);
               STACK_redesk   (a_parent, v);
               /*> XGetGeometry   (YX_DISP, a_parent, &x_long, &j, &j, &w, &t, &j, &j);        <* 
                *> XTranslateCoordinates (YX_DISP, a_parent, x_long, j, j, &x, &y, &x_long);   <*/
               XGetGeometry   (YX_DISP, a_parent, &x_long, &x, &y, &w, &t, &j, &j);
               STACK_resize   (x_curr, x, y, w, t);
               /*> printf ("%-9x  %4dx  %4dy  %4dw  %4dt\n", x_curr, x, y, w, t);     <*/
            }
         }
      }
      GET_populate (a_lvl + 1, x_curr);
   }
   XFree (x_1sts);
   return 0;
}

char
GET_update              (long a_root, int a_loop)
{
   /*> if (a_loop % 40 == 0) {                                                        <* 
    *>    GET_populate   (0, a_root);                                                 <* 
    *>    yEXEC_data_windows (STACK_eterm);                                           <* 
    *> }                                                                              <*/
   /*> THEIA_pull     (FILE_THEIA);                                                   <* 
    *> STACK_context  ();                                                             <*/
   return 0;
}


