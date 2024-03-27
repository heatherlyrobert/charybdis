#include "charybdis.h"

#define     NSEC        1000000000
typedef     struct timespec   tTSPEC;

typedef     long long  llong;

static   llong l_beg   = 0;
static   llong l_end   = 0;
static   llong l_prev  = 0;

static   llong l_secs  = 0;
static   llong l_nsec  = 0;

static   llong l_exp   = 0;
static   llong l_act   = 0;
static   llong l_used  = 0;
static   llong l_sleep = 0;

char
LOOP_init               (float a_base)
{
   l_secs = l_nsec = 0.0;
   if (a_base >= 1.0)  l_secs  = trunc (a_base);
   l_nsec  = (a_base - l_secs) * NSEC;
   return 0;
}

char
LOOP_beg                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tTSPEC      x_dur;
   /*---(reset timing);------------------*/
   l_beg   = 0;
   l_end   = 0;
   /*---(get beginning time)-------------*/
   clock_gettime  (CLOCK_MONOTONIC_RAW, &x_dur);
   l_beg   = x_dur.tv_sec * NSEC;
   l_beg  += x_dur.tv_nsec;
   /*---(complete)-----------------------*/
   return 0;
}

char
LOOP_end                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tTSPEC      x_dur;
   /*---(get beginning time)-------------*/
   clock_gettime  (CLOCK_MONOTONIC_RAW, &x_dur);
   l_end   = x_dur.tv_sec * NSEC;
   l_end  += x_dur.tv_nsec;
   /*---(calc diffs)---------------------*/
   l_exp   = (l_secs * NSEC) + l_nsec;
   l_act   = l_end - l_prev;
   l_used  = l_end - l_beg;
   l_sleep = l_exp - l_used;
   /*---(calc run diffs)-----------------*/
   l_prev  = l_end;
   /*---(sleep)--------------------------*/
   x_dur.tv_sec  = l_sleep / NSEC;
   x_dur.tv_nsec = l_sleep % NSEC;
   nanosleep      (&x_dur, NULL);
   /*---(complete)-----------------------*/
   return 0;
}

/*
 * fluxbox adds a frame to windows and configure acts on the 
 * frames, not the winids.
 *
 *
 */

/*> if ( XGetWindowProperty(FbTk::App::instance()->display(), win,                <* 
 *>                         atom, 0, 1, False, XA_CARDINAL,                       <* 
 *>                         &ret_type, &fmt, &nitems,                             <* 
 *>                         &bytes_after, (unsigned char**)&data) != Success) {   <* 
 *>     throw PropertyException(getAtomName(atom));                               <* 
 *> }                                                                             <* 
 *> unsigned int val = (unsigned int)( *data );                                   <* 
 *> XFree(data);                                                                  <* 
 *> return val;                                                                   <*/

/*> bool Ewmh::propertyNotify(Pager &pager, XPropertyEvent &event) try {                                     <* 
 *>     if (event.window != DefaultRootWindow(FbTk::App::instance()->display())) {                           <* 
 *>         return false;                                                                                    <* 
 *>     }                                                                                                    <* 
 *>     if (event.atom == m_data->current_desktop) {                                                         <* 
 *>         pager.setCurrentWorkspace(getIntProperty(event.window, event.atom));                             <* 
 *>     } else if (event.atom == m_data->number_of_desktops) {                                               <* 
 *>         pager.updateWorkspaceCount(getIntProperty(event.window, event.atom));                            <* 
 *>     } else if (event.atom == m_data->active_window) {                                                    <* 
 *>         pager.setFocusedWindow(getWinProperty(event.window, event.atom));                                <* 
 *>     } else if (event.atom == m_data->clientlist) {                                                       <* 
 *>         vector<Window> windows;                                                                          <* 
 *>         getWinArrayProperty(DefaultRootWindow(FbTk::App::instance()->display()), event.atom, windows);   <* 
 *>         std::vector< pair<Window, unsigned int > > wins_workspaces;                                      <* 
 *>         for ( unsigned int win = 0; win < windows.size(); ++win ) {                                      <* 
 *>             wins_workspaces.                                                                             <* 
 *>                 push_back( std::make_pair(windows[win],                                                  <* 
 *>                                           getIntProperty(windows[win],                                   <* 
 *>                                                          m_data->wm_desktop)));                          <* 
 *>         }                                                                                                <* 
 *>         pager.addWindows(wins_workspaces);                                                               <* 
 *>     } else {                                                                                             <* 
 *>         // did not handle it here                                                                        <* 
 *>         return false;                                                                                    <* 
 *>     }                                                                                                    <*/

/*> } else if (event.type == PropertyNotify) {                                    <* 
 *>     propertyEvent(event.xproperty);                                           <*/

/*> printf ("%d\n", get_property (YX_ROOT, XA_CARDINAL, "_NET_CURRENT_DESKTOP", NULL));   <*/

/*> static gchar *get_window_title (Display *disp, Window win) {/+{{{+/               <* 
 *>     gchar *title_utf8;                                                            <* 
 *>     gchar *wm_name;                                                               <* 
 *>     gchar *net_wm_name;                                                           <* 
 *>                                                                                   <* 
 *>     wm_name = get_property(disp, win, XA_STRING, "WM_NAME", NULL);                <* 
 *>     net_wm_name = get_property(disp, win,                                         <* 
 *>             XInternAtom(disp, "UTF8_STRING", False), "_NET_WM_NAME", NULL);       <* 
 *>                                                                                   <* 
 *>     if (net_wm_name) {                                                            <* 
 *>         title_utf8 = g_strdup(net_wm_name);                                       <* 
 *>     }                                                                             <* 
 *>     else {                                                                        <* 
 *>         if (wm_name) {                                                            <* 
 *>             title_utf8 = g_locale_to_utf8(wm_name, -1, NULL, NULL, NULL);         <* 
 *>         }                                                                         <* 
 *>         else {                                                                    <* 
 *>             title_utf8 = NULL;                                                    <* 
 *>         }                                                                         <* 
 *>     }                                                                             <* 
 *>                                                                                   <* 
 *>     g_free(wm_name);                                                              <* 
 *>     g_free(net_wm_name);                                                          <* 
 *>                                                                                   <* 
 *>     return title_utf8;                                                            <* 
 *> }/+}}}+/                                                                          <*/

char
get_property            (long a_winid, char a_type, long *r_value, char r_string [LEN_FULL])
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


/*> if (! (cur_desktop = (unsigned long *)get_property(disp, root,                <* 
 *>         XA_CARDINAL, "_NET_CURRENT_DESKTOP", NULL))) {                        <*/

/*> char                                                                              <* 
 *> current desktop        (Display *disp, Window win, char *msg, /+ {{{ +/           <* 
 *>     unsigned long data0, unsigned long data1,                                     <* 
 *>     unsigned long data2, unsigned long data3,                                     <* 
 *>     unsigned long data4) {                                                        <* 
 *>   XEvent event;                                                                   <* 
 *>   long mask = SubstructureRedirectMask | SubstructureNotifyMask;                  <* 
 *>                                                                                   <* 
 *>   event.xclient.type = ClientMessage;                                             <* 
 *>   event.xclient.serial = 0;                                                       <* 
 *>   event.xclient.send_event = True;                                                <* 
 *>   event.xclient.message_type = XInternAtom(disp, msg, False);                     <* 
 *>   event.xclient.window = win;                                                     <* 
 *>   event.xclient.format = 32;                                                      <* 
 *>   event.xclient.data.l[0] = data0;                                                <* 
 *>   event.xclient.data.l[1] = data1;                                                <* 
 *>   event.xclient.data.l[2] = data2;                                                <* 
 *>   event.xclient.data.l[3] = data3;                                                <* 
 *>   event.xclient.data.l[4] = data4;                                                <* 
 *>                                                                                   <* 
 *>   if (XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event)) {           <* 
 *>     return EXIT_SUCCESS;                                                          <* 
 *>   }                                                                               <* 
 *>   else {                                                                          <* 
 *>     fprintf(stderr, "Cannot send %s event.\n", msg);                              <* 
 *>     return EXIT_FAILURE;                                                          <* 
 *>   }                                                                               <* 
 *> }/+}}}+/                                                                          <*/

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
   char        x_temp      [LEN_LABEL] = "";
   char        l           =    0;
   long        v           =    0;
   int         x, y, j;
   uint        w, t;
   long        x_long;
   for (i = 0; i < a_lvl; ++i)  strcat (x_pre, "   ");
   rc = XQueryTree (YX_DISP, a_parent, &x_root, &x_parent, &x_1sts, &x_n1st);
   for (x_1st = 0; x_1st < x_n1st; ++x_1st) {
      x_curr = x_1sts [x_1st];
      /*> printf ("%s%3d  %x\n", x_pre, x_1st, x_curr);                               <*/
      if (a_lvl > 0) {
         ystrl4hex ((double) x_curr, x_temp, 4, 'x', LEN_LABEL);
         /*> printf ("CHECKING  %10x  %-10.10s\n", x_curr, x_temp);                <*/
         l = strlen (x_temp);
         if (strcmp ("00040", x_temp + l - 5) == 0) {
            stack_push_top (x_curr, a_parent);
            get_property (x_curr, 'd', &v, NULL);
            stack_redesk (a_parent, v);
            XGetGeometry (YX_DISP, a_parent, &x_long, &j, &j, &w, &t, &j, &j);
            XTranslateCoordinates (YX_DISP, a_parent, x_long, j, j, &x, &y, &x_long);
            stack_resize (x_curr, x, y, w, t);
         }
      }
      stack_populate (a_lvl + 1, x_curr);
   }
   XFree (x_1sts);
   return 0;
}

char
get_context             (long a_winid, short a_eterm)
{
   char        rc          =    0;
   char        x_title     [LEN_FULL]  = "";
   int         x_lvl       =    0;
   int         x_use       =    0;
   char        x_pubname   [LEN_LABEL] = "";
   char        x_cmdline   [LEN_RECD]  = "";
   rc = get_property (a_winid, 't', NULL, x_title);
   /*> printf ("%-10x  %-45.45s\n", x_curr, x_title);                        <*/
   rc = yEXEC_find_eterm_use (a_eterm, &x_lvl, &x_use, x_pubname, x_cmdline);
   printf ("   %d\n", x_lvl);
   printf ("   %d\n", x_use);
   printf ("   %-45.45s\n", x_pubname);
   printf ("   %-45.45s\n", x_cmdline);
   return 0;
}
