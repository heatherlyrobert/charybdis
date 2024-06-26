#include "charybdis.h"



char        g_layout  = '�';
char        g_scope   = '�';
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
   yEXEC_signal (YEXEC_SOFT, YEXEC_YES, YEXEC_NO, REQS_comm, "stdsig");
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
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *a           = NULL;
   char       *b           = NULL;
   int         x_len       =    0;
   char        two_arg     =    0;
   int         x_max       =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
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
      else if (strcmp (a, "--all"         ) == 0)  g_scope     = 'A';
      else if (strcmp (a, "--current"     ) == 0)  g_scope     = 'c';
      else if (strcmp (a, "--nodaemon"    ) == 0)  g_daemon    = '-';
      else if (strncmp (a, "--12345"   , 7) == 0)  ;
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
   /*---(update name)--------------------*/
   rc = yEXEC_maxname (a_argc, a_argv, &x_max);
   /*> printf ("len = %d\n", x_max);                                                  <*/
   ystrlcpy (a_argv [0], P_ONELINE, x_max);
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
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(check for other instances)------*/
   rc = REQS_handler (g_layout, g_scope, g_hint);
   if (g_layout == '�')  g_layout = 't';
   if (g_scope  == '�')  g_scope = 'A';
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit   (__FUNCTION__);
   return rc;
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



