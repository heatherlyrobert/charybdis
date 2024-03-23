#include "charybdis.h"



/*====================------------------------------------====================*/
/*===----                       unit testing                           ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST___________o (void) {;}

char
PROG__unit_quiet     (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         x_argc      =    1;
   char       *x_args [1]  = { "charybdis" };
   /*---(urgents)------------------------*/
   rc = PROG_urgents  (x_argc, x_args);
   DEBUG_PROG   yLOG_value    ("urgents"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(startup)------------------------*/
   rc = PROG_startup  (x_argc, x_args);
   DEBUG_PROG   yLOG_value    ("startup"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
PROG__unit_loud      (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         x_argc      =  2;
   char       *x_args [10] = { "charybdis_unit", "@@kitchen" };
   /*---(urgents)------------------------*/
   rc = PROG_urgents  (x_argc, x_args);
   DEBUG_PROG   yLOG_value    ("urgents"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(startup)------------------------*/
   rc = PROG_startup  (x_argc, x_args);
   DEBUG_PROG   yLOG_value    ("startup"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
PROG__unit_end       (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   rc = PROG_shutdown  ();
   DEBUG_PROG   yLOG_value    ("shutdown"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}
