#include "charybdis.h"

char *s_face = "shrike";
int   s_font = 0;


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
   return 0;
}

char
DRAW_window_one         (void)
{
}

char
DRAW_windows            (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
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
   x_top  = stack_count () - 1;
   rc = stack_by_cursor (YDLST_TAIL, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
   while (rc > 0) {
      if (x_left < 0   ) {  x_wide += x_left;  x_left = 0; }
      x_righ = x_left + x_wide;
      if (x_righ > 1366) {  x_wide = 1366 - x_left;  x_righ = 1366; }
      if (x_topp < 0   ) {  x_tall += x_topp;  x_topp = 0; }
      x_bott = x_topp + x_tall;
      if (x_bott > 1536) {  x_tall = 1536 - x_topp;  x_bott = 1536; }
      if (x_wide >= 0) {
         x_wide /= m;
         x_tall /= m;
         x_left  = a_left + (x_left / m);
         x_righ  = a_left + (x_righ / m);
         x_topp  = ((8 - x_desk) * (a_tall + a_gap)) - (x_topp / m);
         x_bott  = ((8 - x_desk) * (a_tall + a_gap)) - (x_bott / m);
         x_ftopp = (8 - x_desk) * (a_tall + a_gap) - 10;
         a       = 0.50;
         if (n >= x_top)  a = 1.00;
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
         if (n >= x_top)  glColor4f (1.00, 1.00, 0.00, 1.00);
         glBegin         (GL_POLYGON); {
            glVertex3f  (x_left, x_topp, -500 + n * 20);
            glVertex3f  (x_righ, x_topp, -500 + n * 20);
            glVertex3f  (x_righ, x_bott, -500 + n * 20);
            glVertex3f  (x_left, x_bott, -500 + n * 20);
         } glEnd   ();
         /*> if (x_desk >= 0 && x_terse [0] != '·') {                                 <* 
          *>    glPushMatrix(); {                                                     <* 
          *>       if (n >= x_top)  glColor4f (1.00, 1.00, 0.00, 1.00);               <* 
          *>       else             glColor4f (1.00, 1.00, 1.00, 1.00);               <* 
          *>       sprintf (t, "%s", x_terse);                                        <* 
          *>       glTranslatef (a_left - 20, x_ftopp - x_dwin [x_desk] * 12, 0);     <* 
          *>       yFONT_print  (s_font,  7, YF_TOPRIG, t);                           <* 
          *>    } glPopMatrix ();                                                     <* 
          *>    ++(x_dwin [x_desk]);                                                  <* 
          *> }                                                                        <*/
      }
      ++n;
      rc = stack_by_cursor (YDLST_PREV, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
   }
   n = 0;
   rc = stack_by_cursor (YDLST_HEAD, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
   while (rc > 0) {
      x_ftopp = (8 - x_desk) * (a_tall + a_gap) - 10;
      if (x_left >= 0) {
         if (x_desk >= 0 && x_terse [0] != '·') {
            glPushMatrix(); {
               if (n == 0)      glColor4f (1.00, 1.00, 0.00, 1.00);
               else             glColor4f (0.00, 0.00, 0.00, 1.00);
               sprintf (t, "%s  %-2.2s", x_terse, x_hint);
               glTranslatef (a_left -  8, x_ftopp - x_dwin [x_desk] * 12, 0);
               yFONT_print  (s_font,  8, YF_TOPRIG, t);
            } glPopMatrix ();
            ++(x_dwin [x_desk]);
         }
      }
      ++n;
      rc = stack_by_cursor (YDLST_NEXT, x_hint, &x_desk, &x_left, &x_topp, &x_wide, &x_tall, x_back, x_pubname, x_terse, &x_type);
   }
   return 0;
}

char
DRAW_desktops           (short a_left, short a_topp, short a_wide, short a_tall, short a_gap)
{
   char        d           =    0;
   short       x_topp      = 8 * (a_gap + a_tall);
   long        x_desk      =    0;
   get_property (YX_ROOT, 'D', &x_desk, NULL);
   for (d = 0; d <  8; ++d) {
      if (x_desk == d)   glColor4f (1.00, 1.00, 0.00, 1.00);
      else               glColor4f (0.00, 0.00, 0.00, 0.50);
      glLineWidth    (0.25);
      glBegin        (GL_LINE_STRIP); {
         glVertex3f  (a_left         , x_topp         ,  500  );
         glVertex3f  (a_left + a_wide, x_topp         ,  500  );
         glVertex3f  (a_left + a_wide, x_topp - a_tall,  500  );
         glVertex3f  (a_left         , x_topp - a_tall,  500  );
         glVertex3f  (a_left         , x_topp         ,  500  );
      } glEnd   ();
      glLineWidth    (0.10);
      glEnable       (GL_LINE_STIPPLE);
      glLineStipple  (1, 0x3333);
      glBegin        (GL_LINES); {
         glVertex3f  (a_left         , x_topp - (a_tall / 2.0),  550);
         glVertex3f  (a_left + a_wide, x_topp - (a_tall / 2.0),  550);
      } glEnd();
      glDisable      (GL_LINE_STIPPLE);
      x_topp -= a_tall + a_gap;
   }
   return 0;
}

char
DRAW_main               (short a_left, short a_topp, short a_wide, short a_tall)
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
   /*> glOrtho         (0, a_wide, 0, a_tall, -1000, 1000);                           <*/
   glOrtho         (0, a_wide, 0, a_tall, -1000, 1000);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   DEBUG_GRAF   yLOG_note    ("draw a background for ortho/flat");
   glColor4f (1.00, 0.00, 0.00, 1.00);
   DRAW_windows  (155, a_tall, 80, 90, 4);
   DRAW_desktops (155, a_tall, 80, 90, 4);
   /*---(force-out)----------------------*/
   glXSwapBuffers (YX_DISP, YX_BASE);
   glFlush         ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

