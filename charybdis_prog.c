#include "charybdis.h"



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
   int         i           =    0;
   char        rc          =    0;
   int         x_len       =    0;
   int         x_sid       =    0;
   /*---(logger)-------------------------*/
   DEBUG_PROG   yLOG_enter  (__FUNCTION__);
   /*---(-------)------------------------*/
   stack_purge ();
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
      /*> else if (strcmp (a, "--vcheck"      ) == 0)  my.audit    = 'y';             <* 
       *> else if (strcmp (a, "--base_only"   ) == 0)  my.scope    = '-';             <* 
       *> else if (strcmp (a, "--vars_also"   ) == 0)  my.scope    = 'v';             <* 
       *> else if (strcmp (a, "--extended"    ) == 0)  my.scope    = '+';             <* 
       *> else if (strcmp (a, "--heads"       ) == 0)  my.heads    = 'y';             <*/
      /*---(variants)--------------------*/
      /*> else if (strcmp (a, "--vshort"      ) == 0)  my.variants = 's';             <* *> else if (strcmp (a, "--vfull"       ) == 0)  my.variants = 'f';             <*/
      /*---(ornamentation)---------------*/
      /*> else if (strcmp (a, "--base"        ) == 0)  my.ornament = YASCII_BASE;      <* 
       *> else if (strcmp (a, "--hints"       ) == 0)  my.ornament = YASCII_HINTS;     <* 
       *> else if (strcmp (a, "--tsae"        ) == 0)  my.ornament = YASCII_TSAE;      <* 
       *> else if (strcmp (a, "--full"        ) == 0)  my.ornament = YASCII_FULL;      <* 
       *> else if (strcmp (a, "--english"     ) == 0)  my.ornament = YASCII_ENGLISH;   <* 
       *> else if (strcmp (a, "--icing"       ) == 0)  my.ornament = YASCII_ICING;     <* 
       *> else if (strcmp (a, "--diagram"     ) == 0)  my.ornament = YASCII_DIAGRAM;   <*/
      /*---(gapping)---------------------*/
      /*> else if (strcmp (a, "--nogap"       ) == 0)  my.gapping  = YASCII_NOGAP;     <* 
       *> else if (strcmp (a, "--normgap"     ) == 0)  my.gapping  = YASCII_NORMGAP;   <* 
       *> else if (strcmp (a, "--evengap"     ) == 0)  my.gapping  = YASCII_EVENGAP;   <*/
      /*---(drawing)---------------------*/
      /*> else if (strcmp (a, "--flower"      ) == 0)  my.report   = MAYAN_FLOWER;    <* 
       *> else if (strcmp (a, "--page"        ) == 0)  my.report   = MAYAN_PAGE;      <* 
       *> else if (strcmp (a, "--pages"       ) == 0)  my.report   = MAYAN_PAGES;     <* 
       *> else if (strcmp (a, "--group"       ) == 0)  my.report   = MAYAN_GROUP;     <* 
       *> else if (strcmp (a, "--list"        ) == 0)  my.report   = MAYAN_LIST;      <* 
       *> else if (strcmp (a, "--quotes"      ) == 0)  my.report   = MAYAN_QUOTE;     <* 
       *> else if (strcmp (a, "--detail"      ) == 0)  my.report   = MAYAN_DETAIL;    <* 
       *> else if (strcmp (a, "--text"        ) == 0)  my.report   = MAYAN_TRANS;     <* 
       *> else if (strcmp (a, "--freq"        ) == 0)  my.report   = MAYAN_FREQ;      <*/
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



