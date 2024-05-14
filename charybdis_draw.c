#include "charybdis.h"



static char  *s_face = "shrike";
static int    s_font = 0;

static char   s_exist = '-';

static short  s_wwide = 0;
static short  s_wtall = 0;

static char   s_cdesk = 0;

static short  s_left  = 0;
static short  s_topp  = 0;
static short  s_wide  = 0;
static short  s_tall  = 0;
static short  s_gap   = 4;

static char   s_twin      [LEN_TERSE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static char   s_dwin      [LEN_TERSE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o (void) {;}

char
DRAW_init               (void)
{
   /*---(header)----------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(textures)-----------------------*/
   DEBUG_GRAF   yLOG_note    ("textures");
   glEnable        (GL_TEXTURE_2D);    /* NEW */
   /*---(blending)-----------------------*/
   DEBUG_GRAF   yLOG_note    ("blending");
   glShadeModel    (GL_SMOOTH);
   glEnable        (GL_DEPTH_TEST);
   glEnable        (GL_ALPHA_TEST);
   glAlphaFunc     (GL_GEQUAL, 0.0125);
   glEnable        (GL_BLEND);
   glBlendFunc     (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDepthFunc     (GL_LEQUAL);
   /*---(anti-aliasing)------------------*/
   DEBUG_GRAF   yLOG_note    ("anti-aliasing");
   glHint          (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /*---(special polygon antialiasing)----------*/
   DEBUG_GRAF   yLOG_note    ("polygon");
   glEnable        (GL_POLYGON_SMOOTH);
   glPolygonMode   (GL_FRONT_AND_BACK, GL_FILL);
   glHint          (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   /*---(simple defaulting)--------------*/
   DEBUG_GRAF   yLOG_note    ("sizes");
   glLineWidth     (0.50f);
   /*---(process immediately)------------*/
   DEBUG_GRAF   yLOG_note    ("flush");
   glFlush         ();
   /*---(load font)----------------------*/
   s_font = yFONT_load (s_face);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DRAW_wrap               (void)
{
   yFONT_free (s_font);
   yX11_end   ();
   return 0;
}

char
DRAW_sizing             (char a_layout)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(vertical)-----------------------*/
   s_wtall = 90 * 8 + 36;
   s_tall  = s_wtall - 40;
   s_topp  = 20;
   s_gap   = 4;
   DEBUG_GRAF   yLOG_complex ("height"    , "%4dw, %4dt, %4dt, %4dg", s_wtall, s_tall, s_topp, s_gap);
   /*---(horizontal)---------------------*/
   DEBUG_GRAF   yLOG_char   ("a_layout"  , a_layout);
   switch (a_layout) {
   case 'h' : s_wwide = 80;   break;
   case 't' : s_wwide = 80;   break;
   case 'p' : s_wwide = 80 + (3 * s_gap) + 180; break;
   case 'f' : s_wwide = 80 + (3 * s_gap) + 880; break;
   }
   s_wide  = 80;
   s_left  = s_wwide - s_wide;
   DEBUG_GRAF   yLOG_complex ("width"     , "%4dw, %4ds, %4dl", s_wwide, s_wide, s_left);
   /*---(resize)-------------------------*/
   DEBUG_GRAF   yLOG_char   ("s_exist"   , s_exist);
   if (s_exist == '-') {
      rc = yX11_start (P_ONELINE, s_wwide, s_wtall, YX_HIDDEN, YX_FIXED, YX_SILENT);
      DEBUG_GRAF   yLOG_value  ("start"     , rc);
   } else {
      rc = yX11_resize (s_wwide, s_wtall);
      DEBUG_GRAF   yLOG_value  ("resize"    , rc);
   }
   s_exist = 'y';
   rc = yX11_move  (1366 - s_wwide - 4,    6);
   DEBUG_GRAF   yLOG_value  ("move"      , rc);
   DEBUG_GRAF   yLOG_char   ("s_exist"   , s_exist);
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}




/*====================------------------------------------====================*/
/*===----                          per desktop                         ----===*/
/*====================------------------------------------====================*/
static void  o___DESKTOP_________o (void) {;}

char
DRAW__desktop_mask      (char a_layout, char a_curr, char a_desk, short a_left, short a_topp, short a_wide, short a_tall, short a_gap, char a_lines, GC a_gc, Pixmap a_bounds)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =    0;
   int         x_line      =   12;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_senter (__FUNCTION__);
   if (a_lines > 0 || a_desk == a_curr) {
      XFillRectangle (YX_DISP, a_bounds, a_gc, a_left, a_topp, a_wide, a_tall);
   }
   if (a_lines > 0) {
      switch (g_layout) {
      case 'p' : case 'f' :
         n = a_lines + 1;
         if (n > 7)  n = 7;
         XFillRectangle (YX_DISP, a_bounds, a_gc, 0, a_topp, a_left - 4, (n * x_line) + 6);
         break;
      }
   }
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_sexit  (__FUNCTION__);
   return 0;
}

char
DRAW__desktop_back      (char a_layout, char a_curr, char a_desk, short a_left, short a_topp, short a_wide, short a_tall, short a_gap, char a_lines)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =    0;
   float       a           =  0.5;
   float       b           =  0.5;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_senter (__FUNCTION__);
   /*---(prepare)---------------------*/
   DEBUG_GRAF   yLOG_sint   (a_desk);
   DEBUG_GRAF   yLOG_sint   (a_lines);
   if (a_lines == 0) {
      DEBUG_GRAF   yLOG_sint   (a_lines);
      glColor4f (0.20, 0.20, 0.20, 1.00);
      glBegin         (GL_POLYGON); {
         glVertex3f  (a_left         , a_topp         ,  400);
         glVertex3f  (a_left + a_wide, a_topp         ,  400);
         glVertex3f  (a_left + a_wide, a_topp - a_tall,  400);
         glVertex3f  (a_left         , a_topp - a_tall,  400);
      } glEnd   ();
   }
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_sexit  (__FUNCTION__);
   return 0;
}

char
DRAW__desktop_frame     (char a_layout, char a_curr, char a_desk, short a_left, short a_topp, short a_wide, short a_tall, short a_gap, char a_lines)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =    0;
   int         x_line      =   12;
   float       a           =  0.5;
   float       b           =  0.5;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_senter (__FUNCTION__);
   /*---(prepare)---------------------*/
   if (a_lines > 0) {
      a = 0.35;
      b = 0.65;
   }
   n = a_lines + 1;  /* add in header line space */
   if (n > 7)  n = 7;
   /*---(color)-----------------------*/
   if (a_desk == a_curr)   glColor4f (1.00, 1.00, 0.00, 1.00);
   else                    glColor4f (0.00, 0.00, 0.00, 1.00);
   /*---(frame)-----------------------*/
   glLineWidth    (0.25);
   glBegin        (GL_LINE_STRIP); {
      glVertex3f  (a_left         , a_topp         ,  500  );
      glVertex3f  (a_left + a_wide, a_topp         ,  500  );
      glVertex3f  (a_left + a_wide, a_topp - a_tall,  500  );
      glVertex3f  (a_left         , a_topp - a_tall,  500  );
      glVertex3f  (a_left         , a_topp         ,  500  );
   } glEnd   ();
   /*---(divider)---------------------*/
   glLineWidth    (0.10);
   glEnable       (GL_LINE_STIPPLE);
   glLineStipple  (1, 0x3333);
   glBegin        (GL_LINES); {
      glVertex3f  (a_left                , a_topp - ((a_tall + a_gap) / 2.0),  500);
      glVertex3f  (a_left + a_wide * a   , a_topp - ((a_tall + a_gap) / 2.0),  500);
      glVertex3f  (a_left + a_wide * b   , a_topp - ((a_tall + a_gap) / 2.0),  500);
      glVertex3f  (a_left + a_wide       , a_topp - ((a_tall + a_gap) / 2.0),  500);
   } glEnd();
   glDisable      (GL_LINE_STIPPLE);
   /*---(context area)----------------*/
   if (a_lines > 0 && strchr ("pf", a_layout) != NULL) {
      glLineWidth    (0.25);
      glBegin        (GL_LINE_STRIP); {
         glVertex3f  (0         , a_topp         ,  500  );
         glVertex3f  (a_left - 4, a_topp         ,  500  );
         glVertex3f  (a_left - 4, a_topp - (n * x_line) - 6,  500  );
         glVertex3f  (0         , a_topp - (n * x_line) - 6,  500  );
         glVertex3f  (0         , a_topp         ,  500  );
      } glEnd   ();
   }
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_sexit  (__FUNCTION__);
   return 0;
}

char
DRAW__desktop_stats     (char a_curr, char a_desk, short a_left, short a_topp, short a_wide, short a_tall, short a_gap, char a_lines)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_SHORT] = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_senter (__FUNCTION__);
   /*---(prepare)---------------------*/
   if (a_lines > 0 || a_desk == a_curr) {
      glPushMatrix  (); {
         glColor4f (1.00, 1.00, 1.00, 1.00);
         sprintf (t, "%d", a_desk);
         glTranslatef  (a_left + a_wide - 4, a_topp - 12,  600);
         yFONT_print  (s_font,  8, YF_TOPRIG, t);
      } glPopMatrix ();
   }
   if (a_lines > 0) {
      glPushMatrix  (); {
         glColor4f (0.80, 0.80, 0.80, 1.00);
         sprintf (t, "%d", a_lines);
         glTranslatef  (a_left + a_wide / 2.0, a_topp - (a_tall / 2.0),  600);
         yFONT_print  (s_font, 16, YF_MIDCEN, t);
      } glPopMatrix ();
   }
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_sexit  (__FUNCTION__);
   return 0;
}

char
DRAW_desktops           (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   /*---(locals)-----------+-----+-----+-*/
   char        d           =    0;
   short       x_topp      = 8 * (a_gap + a_tall);
   int         n           =    0;
   int         x_line      =   12;
   char        x_beg       =    0;
   char        x_end       =    7;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (g_scope == 'c') {
      x_beg  = x_end = s_cdesk;
   }
   /*---(cycle desktops)-----------------*/
   for (d = x_beg; d <= x_end; ++d) {
      DRAW__desktop_back  (g_layout, s_cdesk, d, a_left, x_topp, a_wide, a_tall, a_gap, s_twin [d]);
      DRAW__desktop_frame (g_layout, s_cdesk, d, a_left, x_topp, a_wide, a_tall, a_gap, s_twin [d]);
      DRAW__desktop_stats (s_cdesk, d, a_left, x_topp, a_wide, a_tall, a_gap, s_twin [d]);
      /*---(next)------------------------*/
      x_topp -= a_tall + a_gap;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          per window                          ----===*/
/*====================------------------------------------====================*/
static void  o___WINDOW__________o (void) {;}

char
DRAW__window_one        (short a_topmost, short a_index, char a_desk, short a_left, short a_tall, short l, short r, short t, short b, short a_gap, char a_back)
{
   /*---(locals)-----------+-----+-----+-*/
   float       m           = 17.0;
   float       a           = 0.50;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_senter (__FUNCTION__);
   DEBUG_GRAF   yLOG_sint   (a_topmost);
   DEBUG_GRAF   yLOG_sint   (a_index);
   DEBUG_GRAF   yLOG_sint   (a_desk);
   /*---(scale geometry)-----------*/
   l  = a_left + (l / m);
   r  = a_left + (r / m);
   t  = ((8 - a_desk) * (a_tall + a_gap)) - (t / m);
   b  = ((8 - a_desk) * (a_tall + a_gap)) - (b / m);
   /*---(set color)----------------*/
   switch (a_back) {
   case 'a' :   glColor4f (0x55 / 255.0, 0x99 / 255.0, 0           , a);  break;
   case 'b' :   glColor4f (0           , 0           , 0xdd / 255.0, a);  break;
   case 'c' :   glColor4f (0x99 / 255.0, 0x33 / 255.0, 0x66 / 255.0, a);  break;
   case 'd' :   glColor4f (0x99 / 255.0, 0x66 / 255.0, 0           , a);  break;
   case 'e' :   glColor4f (0x99 / 255.0, 0           , 0x99 / 255.0, a);  break;
   case 'f' :   glColor4f (0           , 0x77 / 255.0, 0x33 / 255.0, a);  break;
   case 'g' :   glColor4f (0           , 0x99 / 255.0, 0           , a);  break;
   case 'k' :   glColor4f (0           , 0           , 0           , a);  break;
   case 'm' :   glColor4f (0x99 / 255.0, 0           , 0x49 / 255.0, a);  break;
   case 'n' :   glColor4f (0           , 0x44 / 255.0, 0x99 / 255.0, a);  break;
   case 'o' :   glColor4f (0x99 / 255.0, 0x33 / 255.0, 0           , a);  break;
   case 'p' :   glColor4f (0x49 / 255.0, 0           , 0x99 / 255.0, a);  break;
   case 'r' :   glColor4f (0xdd / 255.0, 0           , 0           , a);  break;
   case 's' :   glColor4f (0x66 / 255.0, 0x66 / 255.0, 0x66 / 255.0, a);  break;
   case 't' :   glColor4f (0           , 0x66 / 255.0, 0x66 / 255.0, a);  break;
   case 'v' :   glColor4f (0x99 / 255.0, 0x44 / 255.0, 0x99 / 255.0, a);  break;
   case 'w' :   glColor4f (0x99 / 255.0, 0x44 / 255.0, 0x22 / 255.0, a);  break;
   case 'x' :   glColor4f (0x99 / 255.0, 0x99 / 255.0, 0x99 / 255.0, a);  break;
   case 'y' :   glColor4f (0x99 / 255.0, 0x99 / 255.0, 0           , a);  break;
   default  :   glColor4f (0xff / 255.0, 0xff / 255.0, 0xff / 255.0, a);  break;
   }
   /*---(top window)---------------*/
   if (a_index >= a_topmost)  glColor4f (1.00, 1.00, 0.00, 1.00);
   /*---(draw window)--------------*/
   glBegin         (GL_POLYGON); {
      glVertex3f  (l, t, -500 + a_index * 5);
      glVertex3f  (r, t, -500 + a_index * 5);
      glVertex3f  (r, b, -500 + a_index * 5);
      glVertex3f  (l, b, -500 + a_index * 5);
   } glEnd   ();
   /*---(done)---------------------*/
   DEBUG_GRAF   yLOG_sexit  (__FUNCTION__);
   return 0;
}

char
DRAW_windows            (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_hint      [LEN_SHORT] = "";
   char        x_desk      =    0;
   short       x_left, x_topp, x_wide, x_tall;
   short       x_righ, x_bott;
   char        x_back      [LEN_TERSE] = "";
   char        x_pubname   [LEN_LABEL] = "";
   char        x_terse     [LEN_LABEL] = "";
   char        x_type      =  '·';
   float       m           = 17.0;
   short       n           =    0;
   float       a           = 0.50;
   short       x_top       =    0;
   char        t           [LEN_DESC]  = "";
   short       x_ftopp     =    0;
   char        x_beg       =    0;
   char        x_end       =    7;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   x_top  = STACK_count () - 1;
   if (g_scope == 'c')  x_beg = x_end = s_cdesk;
   /*---(get last)-----------------------*/
   rc = STACK_by_cursor (YDLST_TAIL, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
   /*---(cycle windows)------------------*/
   while (rc > 0) {
      /*---(truncate geometry)-----------*/
      if (x_left < 0   ) {  x_wide += x_left;  x_left = 0; }
      x_righ = x_left + x_wide;
      if (x_righ > 1366) {  x_wide = 1366 - x_left;  x_righ = 1366; }
      if (x_topp < 0   ) {  x_tall += x_topp;  x_topp = 0; }
      x_bott = x_topp + x_tall;
      if (x_bott > 1536) {  x_tall = 1536 - x_topp;  x_bott = 1536; }
      /*---(only real windows)-----------*/
      if (x_wide >= 0) {
         if (g_scope == 'A') {
            DRAW__window_one (x_top, n, x_desk, a_left, a_tall, x_left, x_righ, x_topp, x_bott, a_gap, x_back [0]);
         }
         else if (x_desk == s_cdesk) {
            DRAW__window_one (x_top, n, 0     , a_left, a_tall, x_left, x_righ, x_topp, x_bott, a_gap, x_back [0]);
         }
      }
      /*---(next)------------------------*/
      ++n;
      rc = STACK_by_cursor (YDLST_PREV, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          per window                          ----===*/
/*====================------------------------------------====================*/
static void  o___DETAILS_________o (void) {;}

char
DRAW_titles             (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   short       i           =    0;
   short       x_ftopp     =    0;
   int         x_line      =   12;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
   if (g_scope == 'A') {
      for (i = 0; i < 8; ++i) {
         if (s_twin [i] <= 0) continue;
         x_ftopp = (8 - i) * (a_tall + a_gap) - x_line;
         glPushMatrix(); {
            glColor4f (0.00, 0.50, 0.00, 1.00);
            glTranslatef (a_left -  8, x_ftopp, 0);
            if      (g_layout == 'f')  yFONT_print  (s_font,  8, YF_TOPRIG, STACK_line ('-', -1));
            else if (g_layout == 'p')  yFONT_print  (s_font,  8, YF_TOPRIG, "-----------terse--- t ht m");
         } glPopMatrix ();
      }
   } else if (s_twin [s_cdesk] > 0) {
      x_ftopp = (8 - 0) * (a_tall + a_gap) - x_line;
      glPushMatrix(); {
         glColor4f (0.00, 0.50, 0.00, 1.00);
         glTranslatef (a_left -  8, x_ftopp, 0);
         if      (g_layout == 'f')  yFONT_print  (s_font,  8, YF_TOPRIG, STACK_line ('-', -1));
         else if (g_layout == 'p')  yFONT_print  (s_font,  8, YF_TOPRIG, "-----------terse--- t ht m");
      } glPopMatrix ();
   }
   /*---(complete------------------------*/
   DEBUG_GRAF   yLOG_exit   (__FUNCTION__);
   return 0;
}

char
DRAW_context            (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_hint      [LEN_SHORT] = "";
   char        x_desk      =    0;
   short       x_left;
   char        x_terse     [LEN_LABEL] = "";
   char        x_type      =  '·';
   char        x_mark      =  '·';
   short       n           =    0;
   char        t           [LEN_DESC]  = "";
   short       x_ftopp     =    0;
   short       x_topp, x_bott, x_righ;
   int         x_line      =   12;
   char        x_wins      [8] = { 0, 0, 0, 0, 0, 0, 0, 0};
   char        x_beg       =    0;
   char        x_end       =    7;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (g_scope == 'c') {
      x_beg  = x_end = s_cdesk;
   }
   /*---(get first)----------------------*/
   rc = STACK_by_cursor (YDLST_HEAD, x_hint, &x_desk, &x_left, NULL, NULL, NULL, NULL, NULL, x_terse, &x_type);
   /*---(cycle windows)------------------*/
   while (rc > 0) {
      /*---(calculate top)---------------*/
      if (g_scope == 'A')  x_ftopp = (8 - x_desk) * (a_tall + a_gap) - x_line;
      else                 x_ftopp = (8 - 0     ) * (a_tall + a_gap) - x_line;
      /*---(filter)----------------------*/
      if (x_left >= 0 && x_desk >= x_beg && x_desk <= x_end) {
         if (g_layout != 't' && x_wins [x_desk] < 6) {
            /*---(draw)---------------------*/
            x_topp = x_wins [x_desk] + 1;
            glPushMatrix(); {
               if (n == 0)      glColor4f (1.00, 1.00, 0.00, 1.00);
               else             glColor4f (0.00, 0.00, 0.00, 1.00);
               sprintf (t, "%.17s %c %-2.2s %c", x_terse, x_type, x_hint, x_mark);
               glTranslatef (a_left -  8, x_ftopp - x_topp * x_line, 0);
               switch (g_layout) {
               case 'p' :
                  yFONT_print  (s_font,  8, YF_TOPRIG, t);
                  break;
               case 'f' :
                  yFONT_print  (s_font,  8, YF_TOPRIG, STACK_line ('-', n));
                  break;
               }
            } glPopMatrix ();
            ++(x_wins [x_desk]);
         }
      }
      /*---(next)------------------------*/
      ++n;
      rc = STACK_by_cursor (YDLST_NEXT, x_hint, &x_desk, &x_left, NULL, NULL, NULL, NULL, NULL, x_terse, &x_type);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         main drivers                         ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o (void) {;}

char
DRAW__preparation       (char r_dwin [LEN_TERSE], char r_twin [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_desk      =    0;
   short       x_left      =    0;
   long        d           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   for (x_desk = 0; x_desk < LEN_TERSE; ++x_desk) {
      r_twin [x_desk] = r_dwin [x_desk] = 0;
   }
   DEBUG_GRAF   yLOG_complex ("r_twin"    , "%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d", r_twin [0], r_twin [1], r_twin [2], r_twin [3], r_twin [4], r_twin [5], r_twin [6], r_twin [7], r_twin [8], r_twin [9]);
   DEBUG_GRAF   yLOG_complex ("r_dwin"    , "%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d", r_dwin [0], r_dwin [1], r_dwin [2], r_dwin [3], r_dwin [4], r_dwin [5], r_dwin [6], r_dwin [7], r_dwin [8], r_dwin [9]);
   /*---(get first)----------------------*/
   rc = STACK_by_cursor (YDLST_HEAD, NULL, &x_desk, &x_left, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   /*---(cycle windows)------------------*/
   while (rc > 0) {
      /*---(filter)----------------------*/
      if (x_left >= 0 && x_desk >= 0 ) {
         if (r_twin [x_desk] < 6)   ++(r_dwin [x_desk]);
         ++(r_twin [x_desk]);
      }
      /*---(next)------------------------*/
      rc = STACK_by_cursor (YDLST_NEXT, NULL, &x_desk, &x_left, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      /*---(done)------------------------*/
   }
   DEBUG_GRAF   yLOG_complex ("r_twin *"  , "%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d", r_twin [0], r_twin [1], r_twin [2], r_twin [3], r_twin [4], r_twin [5], r_twin [6], r_twin [7], r_twin [8], r_twin [9]);
   DEBUG_GRAF   yLOG_complex ("r_dwin *"  , "%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d", r_dwin [0], r_dwin [1], r_dwin [2], r_dwin [3], r_dwin [4], r_dwin [5], r_dwin [6], r_dwin [7], r_dwin [8], r_dwin [9]);
   /*---(active desktop)-----------------*/
   GET_property (YX_ROOT, 'D', &d, NULL);
   s_cdesk = d;
   DEBUG_GRAF   yLOG_value   ("s_cdesk"   , s_cdesk);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DRAW_pager              (short a_left, short a_topp, short a_wide, short a_tall)
{
   short       x_min, x_max, x_len;
   short       y_min, y_max, y_len;
   int         x, y;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(clear)--------------------------*/
   glClearColor    (0.50, 0.50, 0.50, 0.25);
   glClear         (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*---(setup view)---------------------*/
   DEBUG_GRAF   yLOG_note    ("set up the view");
   glViewport      (0, 0, a_wide, a_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         (0, a_wide, 0, a_tall, -1000, 1000);
   glMatrixMode    (GL_MODELVIEW);
   /*---(pager-view)---------------------*/
   DEBUG_GRAF   yLOG_note    ("draw a background for ortho/flat");
   glColor4f (1.00, 0.00, 0.00, 1.00);
   switch (g_layout) {
   case 't' :
      DRAW_windows  (s_left, a_tall, 80, 90, 4);
      DRAW_desktops (s_left, a_tall, 80, 90, 4);
      DRAW_context  (s_left, a_tall, 80, 90, 4);
      DRAW_mask     (s_left, a_tall, 80, 90, 4);
      break;
   case 'p' :
      DRAW_windows  (s_left, a_tall, 80, 90, 4);
      DRAW_context  (s_left, a_tall, 80, 90, 4);
      DRAW_titles   (s_left, a_tall, 80, 90, 4);
      DRAW_desktops (s_left, a_tall, 80, 90, 4);
      DRAW_mask     (s_left, a_tall, 80, 90, 4);
      break;
   case 'f' :
      DRAW_windows  (s_left, a_tall, 80, 90, 4);
      DRAW_context  (s_left, a_tall, 80, 90, 4);
      DRAW_titles   (s_left, a_tall, 80, 90, 4);
      DRAW_desktops (s_left, a_tall, 80, 90, 4);
      DRAW_mask     (s_left, a_tall, 80, 90, 4);
      break;
   case '-' : case 'h' :
      DRAW_mask     (s_left, a_tall, 80, 90, 4);
      break;
   }
   /*---(force-out)----------------------*/
   glXSwapBuffers (YX_DISP, YX_BASE);
   glFlush         ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DRAW_main               (char a_layout)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   rc = DRAW__preparation (s_dwin, s_twin);
   DEBUG_GRAF   yLOG_value   ("prep"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = DRAW_pager  (0, s_wtall, s_wwide, s_wtall);
   DEBUG_GRAF   yLOG_value   ("pager"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DRAW_mask               (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   /*---(locals)-----------+-----+-----+-*/
   Pixmap      x_bounds;
   GC          x_gc;
   short       x_topp      = a_gap;
   long        x_desk      =    0;
   int         n           =    0;
   int         x_line      =   12;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(prepare)------------------------*/
   x_bounds  = XCreatePixmap (YX_DISP, YX_BASE, s_wwide, s_wtall, 1);
   x_gc      = XCreateGC     (YX_DISP, x_bounds, 0, NULL);
   XSetForeground (YX_DISP, x_gc, 0);
   XFillRectangle (YX_DISP, x_bounds, x_gc, 0, 0, s_wwide, s_wtall);
   XSetForeground (YX_DISP, x_gc, 1);
   if (strchr ("tpf", g_layout) != NULL) {
      if (g_scope == 'A') {
         for (x_desk = 0; x_desk < 8; ++x_desk) {
            DRAW__desktop_mask (g_layout,  s_cdesk, x_desk, a_left, x_topp, a_wide, a_tall, a_gap, s_twin [x_desk], x_gc, x_bounds);
            x_topp += a_tall + a_gap;
         }
      } else {
         DRAW__desktop_mask (g_layout, s_cdesk, s_cdesk, a_left, x_topp, a_wide, a_tall, a_gap, s_twin [s_cdesk], x_gc, x_bounds);
      }
   }
   /*---(set mask)-----------------------*/
   XShapeCombineMask (YX_DISP, YX_BASE, ShapeBounding, 0, 0, x_bounds, ShapeSet);
   /*---(free)---------------------------*/
   XFreePixmap (YX_DISP, x_bounds);
   XFreeGC     (YX_DISP, x_gc);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}




