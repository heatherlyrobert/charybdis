#include "charybdis.h"


char
THEIA_init              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(yPARSE)-------------------------*/
   rc = yPARSE_config (YPARSE_MANUAL, NULL, YPARSE_ONETIME, YPARSE_FIELD, YPARSE_FILL);
   DEBUG_CONF   yLOG_value   ("yparse"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
THEIA__handler          (int n, uchar a_verb [LEN_TERSE], char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_eterm     [LEN_TERSE] = "";
   char        x_back      [LEN_TERSE] = "";
   char        x_fore      [LEN_TERSE] = "";
   char        x_winid     [LEN_TERSE] = "";
   short       x_epid      =    0;
   double      v           =  0.0;
   long        x_window    =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(dispatch)-----------------------*/
   DEBUG_CONF  yLOG_point   ("a_verb"     , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("a_verb"     , a_verb);
   /*---(read details)-------------------*/
   rc = yPARSE_scanf (a_verb, "TTT"  , x_back, x_fore, x_winid);
   DEBUG_CONF  yLOG_value   ("scanf"      , rc);
   if (rc < 0) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("x_back"     , x_back);
   DEBUG_CONF  yLOG_info    ("x_fore"     , x_fore);
   DEBUG_CONF  yLOG_info    ("x_winid"    , x_winid);
   /*---(preprare)-----------------------*/
   ystrldchg (x_winid, '·', '0', LEN_TERSE);
   ystrl2hex (x_winid, &v, LEN_TERSE);
   x_window = v;
   x_epid   = atoi (a_verb);
   DEBUG_CONF  yLOG_value   ("x_window"   , x_window);
   DEBUG_CONF  yLOG_value   ("x_epid"     , x_epid);
   rc = STACK_theia (x_window, x_epid, x_back, x_fore);
   DEBUG_CONF  yLOG_value   ("theia"      , rc);
   if (rc < 0) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
THEIA_pull              (cchar a_file [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "reading word file...");
   /*---(purge the tables)---------------*/
   rc = yPARSE_reset_in ();
   DEBUG_CONF   yLOG_value   ("purge_in"  , rc);
   /*---(re-configure)-------------------*/
   rc = THEIA_init ();
   DEBUG_CONF   yLOG_value   ("config"    , rc);
   /*---(defense)------------------------*/
   DEBUG_CONF  yLOG_point   ("a_file"     , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_info    ("a_file"     , a_file);
   /*---(read all lines)-----------------*/
   rc = yPARSE_autoread (a_file, NULL, THEIA__handler);
   DEBUG_CONF  yLOG_value   ("read"      , rc);
   /*---(close)--------------------------*/
   rc = yPARSE_close ();
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed closing configuration file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
THEIA_classify        (cchar a_title [LEN_HUND], cchar a_pubname [LEN_LABEL], cchar a_cmdline [LEN_RECD], char r_note [LEN_LABEL])
{
   char        x_type      =  '·';
   char       *p           = NULL;
   char        t           [LEN_HUND]  = "";
   int         l           =    0;
   l = strlen (a_cmdline);
   if (strcmp (a_pubname, "vi") == 0) {
      if (strncmp (a_cmdline, "vi -c call HBUF_on() -c call HTAG_on() " , 39) == 0) {
         if (strncmp (a_cmdline + 39, "master.unit ", 12) != 0) {
            ystrlcpy (t, a_cmdline + 39, LEN_HUND);
            x_type = 'E';
         } else {
            ystrlcpy (t, a_cmdline + 51, LEN_HUND);
            x_type = 'U';
         }
         p = strchr (t, '.');
         if (p != NULL)  p [0] = '\0';
         p = strchr (t, '_');
         if (p != NULL)  p [0] = '\0';
      } else if (strcmp (a_cmdline + l - 5, ".ulog") == 0) {
         ystrlcpy (t, a_cmdline + 30, LEN_HUND);
         p = strchr (t, '.');
         if (p != NULL)  p [0] = '\0';
         p = strchr (t, '_');
         if (p != NULL)  p [0] = '\0';
         x_type = 'L';
      } else if (strncmp (a_cmdline, "vi ", 3) == 0) {
         ystrlcpy (t, a_cmdline + 3, LEN_HUND);
         p = strchr (t, ' ');
         if (p != NULL)  p [0] = '\0';
         x_type = 'V';
      } else {
         ystrlcpy (t, "·········", LEN_HUND);
         x_type = 'v';
      }
   }
   else if (strcmp (a_pubname, "less") == 0) {
      if (strcmp (a_cmdline + l - 5, ".urun") == 0) {
         p = strrchr (a_cmdline, ' ');
         ystrlcpy (t, p + 1, LEN_HUND);
         p = strchr (t, '.');
         if (p != NULL)  p [0] = '\0';
         p = strchr (t, '_');
         if (p != NULL)  p [0] = '\0';
         x_type = 'R';
      } else if (strcmp (a_cmdline, "/usr/bin/less") == 0) {
         ystrlcpy (t, "man"      , LEN_HUND);
         x_type = 'm';
      } else {
         ystrlcpy (t, "·········", LEN_HUND);
         x_type = 'l';
      }
   }
   else if (strcmp (a_pubname, "theia") == 0) {
      ystrlcpy (t, "´·´·´·´·´", LEN_HUND);
      x_type = 'Ï';
   }
   else {
      ystrlcpy (t, a_pubname, LEN_HUND);
      x_type = '-';
   }
   /*---(save-back)----------------*/
   if (x_type == 'E')  ystrlcat (t, " (code)", LEN_LABEL);
   if (x_type == 'U')  ystrlcat (t, " (unit)", LEN_LABEL);
   if (x_type == 'R')  ystrlcat (t, " (urun)", LEN_LABEL);
   if (x_type == 'L')  ystrlcat (t, " (ulog)", LEN_LABEL);
   ystrlcpy (r_note, t, LEN_LABEL);
   /*---(return)-------------------*/
   return x_type;
}
