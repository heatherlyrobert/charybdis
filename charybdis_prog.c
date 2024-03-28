#include "charybdis.h"



char        g_layout  = 'p';


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
      break;
   case  SIGUSR1:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR1 MEANS ...");
      g_layout = 'p';
      DRAW_sizing (g_layout);
      break;
   case  SIGUSR2:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGUSR2 MEANS ...");
      g_layout = 'f';
      DRAW_sizing (g_layout);
      break;
   case  SIGALRM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGALRM MEANS ...");
      break;
   case  SIGTERM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGTERM means terminate daemon");
      break;
   case  SIGSEGV:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGSEGV means daemon blew up");
      break;
   case  SIGABRT:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGABRT means daemon blew up");
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
   int         i           =    0;
   char        rc          =    0;
   int         x_len       =    0;
   int         x_sid       =    0;
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
   int         i           = 0;             /* loop iterator -- arguments     */
   char       *a           = NULL;          /* current argument               */
   int         x_len       = 0;             /* argument length                */
   char        two_arg     = 0;
   /*---(process args)-------------------*/
   DEBUG_ARGS   yLOG_value  ("a_argc"      , a_argc);
   --rce;  for (i = 1; i < a_argc; ++i) {
      a   = a_argv[i];
      if (i < a_argc - 1) two_arg = 1; else two_arg = 0;
      x_len = strlen(a);
      DEBUG_ARGS   yLOG_bullet (i           , a);
      /*---(skip debugging)--------------*/
      if      (a[0] == '@')                     continue;
      /*---(interactive)-----------------*/
      if      (strcmp (a, "--version"     ) == 0) {
         printf ("hearth (%s/%s) %s\n", P_VERNUM, __DATE__, P_VERTXT);
         exit (0);
      }
      else if (strcmp (a, "--usage"       ) == 0) { PROG_usage ();  return -1; }
      /*---(overall)---------------------*/
      else if (strcmp (a, "--thin"        ) == 0)  g_layout    = 't';
      else if (strcmp (a, "--pager"       ) == 0)  g_layout    = 'p';
      else if (strcmp (a, "--full"        ) == 0)  g_layout    = 'f';
      else if (strcmp (a, "--list"        ) == 0)  g_layout    = '-';
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
   char        rc          = 0;
   /*> int         i           = 0;                                                   <*/
   /*> tSTAT       s;                                                                 <*/
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(signaling)----------------------*/
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

char
PROG_single        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   int         x_uid       =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(check for other)----------------*/
   x_running  = yEXEC_find ("charybdis", NULL);
   x_running += yEXEC_find ("charybdis_debug", NULL);
   DEBUG_ENVI   yLOG_value   ("x_running" , x_running);
   --rce;  if (x_running > 1) {
      printf ("charybdis already running in pager mode\n");
      DEBUG_ENVI   yLOG_note    ("charybdis already running in pager mode");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)------------------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
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



