#include "charybdis.h"



static char *s_face = "shrike";
static int   s_font = 0;

static char   s_exist = '-';

static short  s_wwide = 0;
static short  s_wtall = 0;


static short  s_left  = 0;
static short  s_topp  = 0;
static short  s_wide  = 0;
static short  s_tall  = 0;
static short  s_gap   = 4;


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
   /*---(vertical)-----------------------*/
   s_wtall = 90 * 8 + 36;
   s_tall  = s_wtall - 40;
   s_topp  = 20;
   s_gap   = 4;
   /*---(horizontal)---------------------*/
   switch (a_layout) {
   case 't' : s_wwide =   90; break;
   case 'p' : s_wwide =  250; break;
   case 'f' : s_wwide =  970; break;
   }
   s_wide  = 80;
   s_left  = s_wwide - s_wide;
   /*---(resize)-------------------------*/
   if (s_exist == '-') {
      yX11_start ("charybdis", s_wwide, s_wtall, YX_HIDDEN, YX_FIXED, YX_SILENT);
   } else {
      yX11_resize (s_wwide, s_wtall);
   }
   s_exist = 'y';
   yX11_move  (1366 - s_wwide - 4,    6);
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
   char        x_dwin      [8] = { 0, 0, 0, 0, 0, 0, 0, 0};
   char        t           [LEN_DESC]  = "";
   short       x_ftopp     =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   x_top  = STACK_count () - 1;
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
         /*---(scale geometry)-----------*/
         x_wide /= m;
         x_tall /= m;
         x_left  = a_left + (x_left / m);
         x_righ  = a_left + (x_righ / m);
         x_topp  = ((8 - x_desk) * (a_tall + a_gap)) - (x_topp / m);
         x_bott  = ((8 - x_desk) * (a_tall + a_gap)) - (x_bott / m);
         x_ftopp = (8 - x_desk) * (a_tall + a_gap) - 10;
         /*---(set alpha)----------------*/
         a       = 0.50;
         if (n >= x_top)  a = 1.00;
         /*---(set color)----------------*/
         switch (x_back [0]) {
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
         if (n >= x_top)  glColor4f (1.00, 1.00, 0.00, 1.00);
         /*---(draw window)--------------*/
         glBegin         (GL_POLYGON); {
            glVertex3f  (x_left, x_topp, -500 + n * 20);
            glVertex3f  (x_righ, x_topp, -500 + n * 20);
            glVertex3f  (x_righ, x_bott, -500 + n * 20);
            glVertex3f  (x_left, x_bott, -500 + n * 20);
         } glEnd   ();
         /*---(done)---------------------*/
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

char
DRAW_titles             (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   short       i           =    0;
   short       x_ftopp     =    0;
   for (i = 0; i < 8; ++i) {
      x_ftopp = (8 - i) * (a_tall + a_gap) - 10;
      glPushMatrix(); {
         glColor4f (0.00, 0.50, 0.00, 1.00);
         glTranslatef (a_left -  8, x_ftopp, 0);
         if      (g_layout == 'f')  yFONT_print  (s_font,  8, YF_TOPRIG, STACK_line ('-', -1));
         else if (g_layout == 'p')  yFONT_print  (s_font,  8, YF_TOPRIG, "---terse--------- t ht");
      } glPopMatrix ();
   }
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
   short       n           =    0;
   char        x_dwin      [8] = { 1, 1, 1, 1, 1, 1, 1, 1};
   char        t           [LEN_DESC]  = "";
   short       x_ftopp     =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
   /*---(get first)----------------------*/
   rc = STACK_by_cursor (YDLST_HEAD, x_hint, &x_desk, &x_left, NULL, NULL, NULL, NULL, NULL, x_terse, &x_type);
   /*---(cycle windows)------------------*/
   while (rc > 0) {
      /*---(calculate top)---------------*/
      x_ftopp = (8 - x_desk) * (a_tall + a_gap) - 10;
      /*---(filter)----------------------*/
      if (x_left >= 0 && x_desk >= 0 && x_dwin [x_desk] < 7 && x_terse [0] != '·') {
         /*---(draw)---------------------*/
         glPushMatrix(); {
            if (n == 0)      glColor4f (1.00, 1.00, 0.00, 1.00);
            else             glColor4f (0.00, 0.00, 0.00, 1.00);
            sprintf (t, "%s %c %-2.2s", x_terse, x_type, x_hint);
            glTranslatef (a_left -  8, x_ftopp - x_dwin [x_desk] * 12, 0);
            switch (g_layout) {
            case 'p' :
               yFONT_print  (s_font,  8, YF_TOPRIG, t);
               break;
            case 'f' :
               yFONT_print  (s_font,  8, YF_TOPRIG, STACK_line ('-', n));
               break;
            }
         } glPopMatrix ();
         /*---(add to dest)--------------*/
         ++(x_dwin [x_desk]);
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

char
DRAW_desktops           (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   /*---(locals)-----------+-----+-----+-*/
   char        d           =    0;
   short       x_topp      = 8 * (a_gap + a_tall);
   long        x_desk      =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
   /*---(active desktop)-----------------*/
   GET_property (YX_ROOT, 'D', &x_desk, NULL);
   /*---(cycle desktops)-----------------*/
   for (d = 0; d <  8; ++d) {
      /*---(color)-----------------------*/
      if (x_desk == d)   glColor4f (1.00, 1.00, 0.00, 1.00);
      else               glColor4f (0.00, 0.00, 0.00, 1.00);
      /*---(frame)-----------------------*/
      glLineWidth    (0.25);
      glBegin        (GL_LINE_STRIP); {
         glVertex3f  (a_left         , x_topp         ,  500  );
         glVertex3f  (a_left + a_wide, x_topp         ,  500  );
         glVertex3f  (a_left + a_wide, x_topp - a_tall,  500  );
         glVertex3f  (a_left         , x_topp - a_tall,  500  );
         glVertex3f  (a_left         , x_topp         ,  500  );
      } glEnd   ();
      /*---(divider)---------------------*/
      glLineWidth    (0.10);
      glEnable       (GL_LINE_STIPPLE);
      glLineStipple  (1, 0x3333);
      glBegin        (GL_LINES); {
         glVertex3f  (a_left         , x_topp - ((a_tall + a_gap) / 2.0),  550);
         glVertex3f  (a_left + a_wide, x_topp - ((a_tall + a_gap) / 2.0),  550);
      } glEnd();
      glDisable      (GL_LINE_STIPPLE);
      /*---(next)------------------------*/
      x_topp -= a_tall + a_gap;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit   (__FUNCTION__);
   return 0;
}

char
DRAW_pager              (short a_left, short a_topp, short a_wide, short a_tall)
{
   short       x_min, x_max, x_len;
   short       y_min, y_max, y_len;
   int         x, y;
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
      DRAW_windows  (    5, a_tall, 80, 90, 4);
      DRAW_desktops (    5, a_tall, 80, 90, 4);
      break;
   case 'p' :
      DRAW_windows  (  165, a_tall, 80, 90, 4);
      DRAW_context  (  165, a_tall, 80, 90, 4);
      DRAW_titles   (  165, a_tall, 80, 90, 4);
      DRAW_desktops (  165, a_tall, 80, 90, 4);
      break;
   case 'f' :
      DRAW_windows  (  885, a_tall, 80, 90, 4);
      DRAW_context  (  885, a_tall, 80, 90, 4);
      DRAW_titles   (  885, a_tall, 80, 90, 4);
      DRAW_desktops (  885, a_tall, 80, 90, 4);
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
DRAW_launch             (short a_left, short a_topp, short a_wide, short a_tall)
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
   int         i           =    0;
   short       c           =    0;
   short       n           =    0;
   short       x, y;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter  (__FUNCTION__);
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
   /*---(prepare)------------------------*/
   c  = STACK_count ();
   /*---(launch-view)--------------------*/
   rc = STACK_by_cursor (YDLST_HEAD, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
   /*---(cycle windows)------------------*/
   for (i = 0; i < c; ++i) {
      x = a_left + 20;
      y = a_topp - 40 - (n * 15);
      /*> printf ("%3di  %3dx  %3dy\n", i, x, y);                                     <*/
      /*---(draw)---------------------*/
      glPushMatrix(); {
         if (i == 0)      glColor4f (1.00, 1.00, 0.00, 1.00);
         else             glColor4f (0.00, 0.00, 0.00, 1.00);
         glTranslatef (x, y, 0);
         yFONT_print  (s_font,  8, YF_TOPLEF, STACK_line ('-', i));
      } glPopMatrix ();
      /*---(next)------------------------*/
      rc = STACK_by_cursor (YDLST_NEXT, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
      ++n;
      if (i > 0 && (i + 1) % 5 == 0) ++n;
      /*---(done)------------------------*/
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
   DRAW_pager  (0, s_wtall, s_wwide, s_wtall);
   /*> if      (a_layout == 'p')  DRAW_pager  (0, s_wtall, s_wwide, s_wtall);         <* 
    *> else if (a_layout == 't')  DRAW_pager  (0, s_wtall, s_wwide, s_wtall);         <* 
    *> else                       DRAW_launch (0, s_wtall, s_wwide, s_wtall);         <*/
   return 0;
}




