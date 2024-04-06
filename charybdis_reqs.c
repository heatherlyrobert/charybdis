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
      DRAW_main   (g_layout);
      x_caught = 'y';
      break;
   case  SIGUSR1:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR1, switch to pager view");
      g_layout = 'p';
      DRAW_sizing (g_layout);
      DRAW_main   (g_layout);
      x_caught = 'y';
      break;
   case  SIGUSR2:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR2, switch to full view");
      g_layout = 'f';
      DRAW_sizing (g_layout);
      DRAW_main   (g_layout);
      x_caught = 'y';
      break;
   case  SIGALRM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGALRM switch to hidden");
      g_layout = '-';
      DRAW_sizing ('t');
      DRAW_main   (g_layout);
      x_caught = 'y';
      break;
   }
   /*---(unused)-------------------------*/
   /*> switch (a_signal) {                                                            <* 
    *> case  SIGURG:                                                                  <* 
    *>    DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGURG, means ...");                  <* 
    *>    x_caught = 'y';                                                             <* 
    *>    break;                                                                      <* 
    *> case  SIGPOLL:                                                                 <* 
    *>    DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGPOLL, means ...");                 <* 
    *>    x_caught = 'y';                                                             <* 
    *>    break;                                                                      <* 
    *> case  SIGPROF:                                                                 <* 
    *>    DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGPROF, means...");                  <* 
    *>    x_caught = 'y';                                                             <* 
    *>    break;                                                                      <* 
    *> case  SIGVTALRM:                                                               <* 
    *>    DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGVTALRM, means...");                <* 
    *>    x_caught = 'y';                                                             <* 
    *>    break;                                                                      <* 
    *> }                                                                              <*/
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
REQS_handler            (char a_layout, char a_hint [LEN_LABEL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_running   =    0;
   int         x_pid       =    0;
   int         x_rpid      =    0;
   int         l           =    0;
   char        x_good      =  '-';
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   DEBUG_ARGS   yLOG_complex ("args"        , "%c, %s", a_layout, a_hint);
   /*---(check for other instances)------*/
   x_pid  = getpid ();
   DEBUG_ENVI   yLOG_value   ("x_pid"     , x_pid);
   x_running  = yEXEC_duplicate ("charybdis", x_pid, &x_rpid);
   DEBUG_ENVI   yLOG_value   ("x_running" , x_running);
   /*---(single instance)----------------*/
   if (x_running == 0) {
      DEBUG_PROG   yLOG_exit   (__FUNCTION__);
      return 0;
   }
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
   /*---(prepare)------------------------*/
   l = strlen (a_hint);
   /*---(formatting changes)-------------*/
   switch (a_layout) {
   case 'h' :
      printf ("charybdis, switching already running instance to \"hidden\" view\n");
      kill (x_rpid, SIGALRM);
      x_good = 'y';
      break;
   case 't' :
      printf ("charybdis, switching already running instance to \"thin\" view\n");
      kill (x_rpid, SIGHUP);
      x_good = 'y';
      break;
   case 'p' :
      printf ("charybdis, switching already running instance to \"pager\" view\n");
      kill (x_rpid, SIGUSR1);
      x_good = 'y';
      break;
   case 'f' :
      printf ("charybdis, switching already running instance to \"full\" view\n");
      kill (x_rpid, SIGUSR2);
      x_good = 'y';
      break;
   }
   /*---(jump to desktop)----------------*/
   if (l == 1) {
      printf ("charybdis, jumping to desktop %c\n", a_hint [0]);
      rc = yX11_desk_goto (a_hint [0]);
      DEBUG_ARGS   yLOG_value  ("desk_goto"   , rc);
      x_good = 'y';
   }
   /*---(jump to hint)-------------------*/
   else if (l == 2) {
      DEBUG_ARGS   yLOG_info   ("a_hint"      , a_hint);
      rc = STACK_pull  (FILE_CHARYBDIS);
      if (strcmp (a_hint, "zz") == 0) {
         printf ("charybdis, jumping to system window\n");
         rc = STACK_by_system ();
         DEBUG_ARGS   yLOG_value  ("by_system"   , rc);
      } else {
         printf ("charybdis, jumping to specific window\n");
         DEBUG_ARGS   yLOG_value  ("pull"        , rc);
         rc = STACK_by_hint (a_hint, 'g', '-');
         DEBUG_ARGS   yLOG_value  ("by_hint"     , rc);
      }
      if (rc >= 0)  x_good = 'y';
   }
   /*---(create a mark)------------------*/
   /*> else if (l >= 4 && a_hint [1] == '=')  {                                       <* 
    *>    printf ("charybdis, creating a mark\n");                                    <* 
    *>    if (l == 4)  rc = STACK_by_hint (a_hint + 2, 'm', a_hint [0]);              <* 
    *>    else         rc = STACK_by_name (a_hint + 2, 'm', a_hint [0]);              <* 
    *>    DEBUG_ARGS   yLOG_value  ("marking"     , rc);                              <* 
    *>    x_good = 'y';                                                               <* 
    *> }                                                                              <*/
   /*---(jump by title)------------------*/
   else if (l > 2)  {
      printf ("charybdis, jumping to named window\n");
      DEBUG_ARGS   yLOG_info   ("a_hint"      , a_hint);
      rc = STACK_pull  (FILE_CHARYBDIS);
      DEBUG_ARGS   yLOG_value  ("pull"        , rc);
      rc = STACK_by_name (a_hint, 'g', '-');
      DEBUG_ARGS   yLOG_value  ("by_name"     , rc);
      x_good = 'y';
   }
   /*---(unknown end)--------------------*/
   DEBUG_ENVI   yLOG_char    ("x_good"    , x_good);
   if (x_good != 'y') {
      printf ("CONFUSION, charybdis already running and no options included\n");
      DEBUG_ENVI   yLOG_note    ("charybdis is running, nothing requested");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 1;
}


