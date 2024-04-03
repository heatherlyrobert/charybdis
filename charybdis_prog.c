#include "charybdis.h"



char        g_layout  = '�';
char        g_daemon  = 'y';
char        g_hint    [LEN_LABEL] = "";


/*====================------------------------------------====================*/
/*===----                        program wide                          ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o (void) {;}

char
PROG_usage              (void)
{
   printf("\n");
   exit(0);
}

char      verstring    [500];

char*
PROG_version            (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   ystrlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   ystrlcpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   ystrlcpy (t, "[cbang      ]", 15);
#else
   ystrlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, LEN_HUND, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

void             /* [------] receive signals ---------------------------------*/
PROG_comm          (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc)
{
   /*---(catch)--------------------------*/
   switch (a_signal) {
   case  SIGHUP:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGHUP MEANS REFRESH");
      g_layout = 't';
      DRAW_sizing (g_layout);
      DRAW_main   (g_layout);
      break;
   case  SIGUSR1:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR1 MEANS ...");
      g_layout = 'p';
      DRAW_sizing (g_layout);
      DRAW_main   (g_layout);
      break;
   case  SIGUSR2:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR2 MEANS ...");
      g_layout = 'f';
      DRAW_sizing (g_layout);
      DRAW_main   (g_layout);
      break;
   case  SIGALRM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGALRM MEANS ...");
      g_layout = '-';
      DRAW_sizing ('t');
      DRAW_main   (g_layout);
      break;
   case  SIGTERM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGTERM means terminate");
      break;
   case  SIGSEGV:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGSEGV means blew up");
      break;
   case  SIGABRT:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGABRT means blew up");
      break;
   default      :
      DEBUG_PROG  yLOG_info     ("SIGNAL", "unknown signal recieved");
      break;
   }
   /*---(complete)-----------------------*/
   return;
}



/*====================------------------------------------====================*/
/*===----                       pre-initialization                     ----===*/
/*====================------------------------------------====================*/
static void      o___PREINIT________o (void) {;}

char
PROG__header            (void)
{
   /*---(header)----------------------*/
   DEBUG_PROG   yLOG_enter (__FUNCTION__);
   /*---(versioning)------------------*/
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_info     ("version" , PROG_version ());
   DEBUG_PROG   yLOG_note     ("custom core");
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version      ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version      ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version     ());
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
PROG_urgents            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(set mute)-----------------------*/
   yURG_all_mute ();
   /*---(start logger)-------------------*/
   rc = yURG_logger  (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process urgents)----------------*/
   rc = yURG_urgs    (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(show header)--------------------*/
   rc = PROG__header ();
   DEBUG_PROG   yLOG_value    ("header"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        program startup                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP____________o (void) {;}

char
PROG__init              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(logger)-------------------------*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(-------)------------------------*/
   STACK_init  ();
   THEIA_init  ();
   /*---(signals)-------------------------------*/
   yEXEC_signal (YEXEC_SOFT, YEXEC_YES, YEXEC_NO, PROG_comm, "stdsig");
   DEBUG_ENVI   yLOG_value   ("signals"   , rc);
   --rce;  if (rc < 0) {
      /*> printf ("hestia sigals could not be set properly\n");                       <*/
      DEBUG_ENVI   yLOG_note    ("hestia signals cound not be set properly");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return 0;
}

#define    TWOARG      if (two_arg == 1)

char
PROG__args              (int a_argc, char *a_argv [])
{
   char        rce         =  -10;
   /*---(begin)------------+-----------+-*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(locals)-------------------------*/
   int         i           =    0;
   char       *a           = NULL;
   char       *b           = NULL;
   int         x_len       =    0;
   char        two_arg     =    0;
   /*---(process args)-------------------*/
   DEBUG_ARGS   yLOG_value  ("a_argc"      , a_argc);
   --rce;  for (i = 1; i < a_argc; ++i) {
      a   = a_argv[i];
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      x_len = strlen (a);
      DEBUG_ARGS   yLOG_bullet (i           , a);
      /*---(skip debugging)--------------*/
      if      (a[0] == '@')                     continue;
      /*---(interactive)-----------------*/
      if      (strcmp (a, "--version"     ) == 0) {
         printf ("charybdis (%s/%s) %s\n", P_VERNUM, __DATE__, P_VERTXT);
         exit (0);
      }
      else if (strcmp (a, "--usage"       ) == 0) { PROG_usage ();  return -1; }
      /*---(overall)---------------------*/
      else if (strcmp (a, "--hidden"      ) == 0)  g_layout    = 'h';
      else if (strcmp (a, "--thin"        ) == 0)  g_layout    = 't';
      else if (strcmp (a, "--pager"       ) == 0)  g_layout    = 'p';
      else if (strcmp (a, "--full"        ) == 0)  g_layout    = 'f';
      else if (strcmp (a, "--kill"        ) == 0)  g_layout    = 'K';
      else if (strcmp (a, "--list"        ) == 0)  g_layout    = '-';
      else if (strcmp (a, "--nodaemon"    ) == 0)  g_daemon    = '-';
      /*---(hints)-----------------------*/
      else if (a [0] != '-') {
         ystrlcpy (g_hint, a, LEN_LABEL);
         DEBUG_ARGS   yLOG_info   ("g_hint"      , g_hint);
      }
      /*---(trouble)---------------------*/
      else {
         /*> ystrlcpy (my.tsae, a, LEN_RECD);                                         <*/
         /*> printf ("command line option <%s> not understood, FATAL\n", a);          <* 
          *> DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);                           <* 
          *> return rce;                                                              <*/
      }
   }  /*---(done)------------------------*/
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return 0;
}

char
PROG__begin             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_running   =    0;
   int         x_pid       =    0;
   int         x_rpid      =    0;
   char        n           =    0;
   long        x_root      =    0;
   int         l           =    0;
   char        x_good      =  '-';
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(check for other instances)------*/
   x_pid  = getpid ();
   DEBUG_ENVI   yLOG_value   ("x_pid"     , x_pid);
   x_running  = yEXEC_duplicate ("charybdis", x_pid, &x_rpid);
   DEBUG_ENVI   yLOG_value   ("x_running" , x_running);
   --rce;  if (x_running > 0) {
      DEBUG_ARGS   yLOG_char   ("g_layout"    , g_layout);
      switch (g_layout) {
      case 'K' :
         printf ("charybdis, killing main instance\n");
         kill (x_rpid, SIGKILL);
         x_good = 'y';
         break;
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
      if (strcmp (g_hint, "") != 0) {
         l = strlen (g_hint);
         if (l == 1) {
            printf ("charybdis, jumping to desktop %c\n", g_hint [0]);
            rc = yX11_desk_goto (g_hint [0]);
            DEBUG_ARGS   yLOG_value  ("desk_goto"   , rc);
            x_good = 'y';
         } else if (l == 2) {
            DEBUG_ARGS   yLOG_info   ("g_hint"      , g_hint);
            rc = STACK_pull  (FILE_CHARYBDIS);
            if (strcmp (g_hint, "zz") == 0) {
               printf ("charybdis, jumping to system window\n");
               rc = STACK_by_system ();
               DEBUG_ARGS   yLOG_value  ("by_system"   , rc);
            } else {
               printf ("charybdis, jumping to specific window\n");
               DEBUG_ARGS   yLOG_value  ("pull"        , rc);
               rc = STACK_by_hint (g_hint, 'g', '-');
               DEBUG_ARGS   yLOG_value  ("by_hint"     , rc);
            }
            if (rc >= 0)  x_good = 'y';
         } else if (l >= 4 && g_hint [1] == '=')  {
            printf ("charybdis, creating a mark\n");
            if (l == 4)  rc = STACK_by_hint (g_hint + 2, 'm', g_hint [0]);
            else         rc = STACK_by_name (g_hint + 2, 'm', g_hint [0]);
            DEBUG_ARGS   yLOG_value  ("marking"     , rc);
            x_good = 'y';
         } else if (l > 2)  {
            printf ("charybdis, jumping to named window\n");
            DEBUG_ARGS   yLOG_info   ("g_hint"      , g_hint);
            rc = STACK_pull  (FILE_CHARYBDIS);
            DEBUG_ARGS   yLOG_value  ("pull"        , rc);
            rc = STACK_by_name (g_hint, 'g', '-');
            DEBUG_ARGS   yLOG_value  ("by_name"     , rc);
            x_good = 'y';
         }
      }
      DEBUG_ENVI   yLOG_char    ("x_good"    , x_good);
      if (x_good != 'y') {
         printf ("CONFUSION, charybdis already running and no options included\n");
         DEBUG_ENVI   yLOG_note    ("charybdis is running, nothing requested");
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   if (g_layout == 'K') {
      printf ("FATAL, can not kill charybdis, no instance is running\n");
      DEBUG_ENVI   yLOG_note    ("can not kill, no charybdis is running");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return   0;
}

char
PROG_startup            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   yURG_stage_check (YURG_BEG);
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = PROG__init   ();
   DEBUG_PROG   yLOG_value    ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(arguments)----------------------*/
   rc = PROG__args   (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("args"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(begin)--------------------------*/
   rc = PROG__begin  ();
   DEBUG_PROG   yLOG_value    ("begin"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   yURG_stage_check (YURG_MID);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                     runtime support                          ----===*/
/*====================------------------------------------====================*/
static void      o___RUNTIME____________o (void) {;}

char
PROG_dawn          (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         x_logger    =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   x_logger = yLOGS_lognum ();
   DEBUG_PROG   yLOG_value    ("x_logger"  , x_logger);
   DEBUG_PROG   yLOG_char     ("g_daemon"  , g_daemon);
   if (g_daemon == 'y') {
      rc = yEXEC_daemon (x_logger, NULL);
      DEBUG_PROG   yLOG_value    ("daemon"    , rc);
      /*> rc = daemon (1, 0);                                                         <*/
      if (rc != 0) exit (0);
   }
   /*---(complete)------------------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     program wrapup                           ----===*/
/*====================------------------------------------====================*/
static void      o___WRAPUP_____________o (void) {;}

char
PROG__end               (void)
{
   /*---(begin)------------+-----------+-*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(call)---------------------------*/
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return 0;
}

char
PROG_shutdown           (void)
{
   /*---(header)-------------------------*/
   yURG_stage_check (YURG_END);
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   PROG__end ();
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   DEBUG_PROG   yLOGS_end    ();
   return 0;
}



