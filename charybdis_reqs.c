#include "charybdis.h"


void             /* [------] receive signals ---------------------------------*/
REQS_comm          (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc)
{
   char        x_caught =  '-';
   /*---(format changes)-----------------*/
   switch (a_signal) {
   case  SIGHUP:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGHUP, switch to thin view");
      g_layout = 't';
      DRAW_sizing (g_layout);
      g_draw = x_caught = 'y';
      break;
   case  SIGUSR1:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR1, switch to pager view");
      g_layout = 'p';
      DRAW_sizing (g_layout);
      g_draw = x_caught = 'y';
      break;
   case  SIGUSR2:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR2, switch to full view");
      g_layout = 'f';
      DRAW_sizing (g_layout);
      g_draw = x_caught = 'y';
      break;
   case  SIGALRM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGALRM switch to hidden");
      g_layout = '-';
      DRAW_sizing ('t');
      g_draw = x_caught = 'y';
      break;
   }
   /*---(unused)-------------------------*/
   switch (a_signal) {
   case  SIGPOLL:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGPOLL, means ...");
      g_scope  = 'A';
      g_draw = x_caught = 'y';
      break;
   case  SIGPROF:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGPROF, means...");
      g_scope  = 'c';
      g_draw = x_caught = 'y';
      break;
      /*> case  SIGURG:                                                                  <* 
       *>    DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGURG, means ...");                  <* 
       *>    x_caught = 'y';                                                             <* 
       *>    break;                                                                      <*/
      /*> case  SIGVTALRM:                                                               <* 
       *>    DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGVTALRM, means...");                <* 
       *>    x_caught = 'y';                                                             <* 
       *>    break;                                                                      <*/
   }
   /*---(killing)------------------------*/
   switch (a_signal) {
   case  SIGTERM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGTERM means terminate gracefully");
      x_caught = 'y';
      break;
   case  SIGSEGV:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGSEGV means blew up");
      x_caught = 'y';
      break;
   case  SIGABRT:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGABRT means blew up");
      x_caught = 'y';
      break;
   }
   /*---(unknown/uncaught)---------------*/
   if (x_caught != 'y') {
      DEBUG_PROG  yLOG_info     ("SIGNAL", "unknown signal recieved");
   }
   /*---(complete)-----------------------*/
   return;
}


char
REQS_handler            (char a_layout, char a_scope, char a_hint [LEN_LABEL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_running   =    0;
   int         x_pid       =    0;
   int         x_rpid      =    0;
   int         l           =    0;
   char        x_handled   =  '-';
   double      x_winid     =    0;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   DEBUG_ARGS   yLOG_complex ("args"        , "%c, %s", a_layout, a_hint);
   /*---(check for other instances)------*/
   x_pid  = getpid ();
   DEBUG_ENVI   yLOG_value   ("x_pid"     , x_pid);
   x_running  = yEXEC_duplicate ("charybdis", x_pid, &x_rpid);
   DEBUG_ENVI   yLOG_value   ("x_running" , x_running);
   /*---(murder)-------------------------*/
   --rce;  if (a_layout == 'K') {
      if (x_running > 0) {
         printf ("charybdis, killing main instance\n");
         kill (x_rpid, SIGKILL);
         DEBUG_PROG   yLOG_exit   (__FUNCTION__);
         return 1;
      }
      printf ("can not kill charybdis, no instance is running\n");
      DEBUG_ENVI   yLOG_note    ("can not kill, no charybdis is running");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(single instance)----------------*/
   if (x_running == 0) {
      DEBUG_PROG   yLOG_exit   (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   l = strlen (a_hint);
   /*---(formatting changes)-------------*/
   switch (a_layout) {
   case 'h' :
      printf ("charybdis, switching already running instance to \"hidden\" view\n");
      kill (x_rpid, SIGALRM);
      rc = 1;
      x_handled = 'y';
      break;
   case 't' :
      printf ("charybdis, switching already running instance to \"thin\" view\n");
      kill (x_rpid, SIGHUP);
      rc = 1;
      x_handled = 'y';
      break;
   case 'p' :
      printf ("charybdis, switching already running instance to \"pager\" view\n");
      kill (x_rpid, SIGUSR1);
      rc = 1;
      x_handled = 'y';
      break;
   case 'f' :
      printf ("charybdis, switching already running instance to \"full\" view\n");
      kill (x_rpid, SIGUSR2);
      rc = 1;
      x_handled = 'y';
      break;
   }
   /*---(scope changes)------------------*/
   switch (a_scope) {
   case 'A' :
      printf ("charybdis, switching already running instance to \"all desktops\" view\n");
      kill (x_rpid, SIGPOLL);
      rc = 1;
      x_handled = 'y';
      break;
   case 'c' :
      printf ("charybdis, switching already running instance to \"current desktop\" view\n");
      kill (x_rpid, SIGPROF);
      rc = 1;
      x_handled = 'y';
      break;
   }
   /*---(jump to desktop)----------------*/
   if (l <= 0)  {
      DEBUG_ENVI   yLOG_char    ("x_handled" , x_handled);
      if (x_handled != 'y') {
         printf ("CONFUSION, charybdis already running and no options included\n");
         DEBUG_ENVI   yLOG_note    ("charybdis is running, nothing requested");
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_PROG   yLOG_exit   (__FUNCTION__);
      return 1;
   }
   /*---(jump to desktop)----------------*/
   rc = STACK_pull  (FILE_CHARYBDIS);
   DEBUG_ARGS   yLOG_value  ("pull"        , rc);
   if (rc < 0) {
      printf ("charybdis, could not pull in data file \"%s\"\n", FILE_CHARYBDIS);
      rc = -1;
      x_handled = 'y';
   }
   /*---(jump to desktop)----------------*/
   else if (l == 1) {
      rc = yX11_desk_goto (a_hint [0]);
      DEBUG_ARGS   yLOG_value  ("desk_goto"   , rc);
      if (rc >= 0) {
         printf ("charybdis, jumping to desktop '%c'\n", a_hint [0]);
         rc = 1;
      } else {
         printf ("charybdis, could not find desktop '%c'\n", a_hint [0]);
      }
      x_handled = 'y';
   }
   /*---(jump to hint)-------------------*/
   else if (l == 2) {
      if (strcmp (a_hint, "zz") == 0) {
         rc = STACK_by_system ();
         DEBUG_ARGS   yLOG_value  ("by_system"   , rc);
         if (rc >= 0) {
            printf ("charybdis, jumping to system hint \"%s\"\n", a_hint);
            rc = 1;
         } else {
            printf ("charybdis, coud not find system hint \"%s\"\n", a_hint);
         }
      } else if (atoi (a_hint) > 0) {
         sprintf (t, "0x0%s00040", a_hint);
         rc = ystrl2hex (t, &x_winid, LEN_LABEL);
         DEBUG_ARGS   yLOG_value  ("to hex"      , rc);
         if (rc >= 0) {
            rc = yX11_win_goto (x_winid);
            DEBUG_ARGS   yLOG_value  ("by_winid"    , rc);
            if (rc >= 0) {
               printf ("charybdis, jumped to window id \"%s\"\n", t);
               rc = 1;
            } else {
               printf ("charybdis, no window found id \"%s\"\n", t);
            }
         } else {
             printf ("charybdis, window id \"%s\" could not be found\n", t);
         }
         x_handled = 'y';

      } else {
         rc = STACK_by_hint (a_hint, 'g', '-');
         DEBUG_ARGS   yLOG_value  ("by_hint"     , rc);
         if (rc >= 0) {
            printf ("charybdis, jumping to window hint \"%s\"\n", a_hint);
            rc = 1;
         } else {
            printf ("charybdis, coud not find window hint \"%s\"\n", a_hint);
         }
      }
      x_handled = 'y';
   }
   /*---(jump by title)------------------*/
   else if (l > 2)  {
      rc = ystrl2hex (a_hint, &x_winid, LEN_LABEL);
      if (rc < 0) {
         rc = STACK_by_name (a_hint, 'g', '-');
         DEBUG_ARGS   yLOG_value  ("by_name"     , rc);
         if (rc >= 0) {
            printf ("charybdis, jumped to window prefixed \"%s\"\n", a_hint);
            rc = 1;
         } else {
            printf ("charybdis, no window found prefixed \"%s\"\n", a_hint);
         }
         x_handled = 'y';
      } else {
         rc = yX11_win_goto (x_winid);
         DEBUG_ARGS   yLOG_value  ("by_winid"    , rc);
         if (rc >= 0) {
            printf ("charybdis, jumped to window id \"%s\"\n", a_hint);
            rc = 1;
         } else {
            printf ("charybdis, no window found id \"%s\"\n", a_hint);
         }
         x_handled = 'y';
      }
   }
   /*---(unknown end)--------------------*/
   DEBUG_ENVI   yLOG_char    ("x_handled" , x_handled);
   if (x_handled != 'y') {
      printf ("CONFUSION, charybdis already running and no options included\n");
      DEBUG_ENVI   yLOG_note    ("charybdis is running, nothing requested");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}


