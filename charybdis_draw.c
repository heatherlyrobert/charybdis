#include "charybdis.h"


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
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DRAW_main               (short a_left, short a_topp, short a_wide, short a_tall)
{
   short       x_min, x_max, x_len;
   short       y_min, y_max, y_len;
   int         x, y;
   /*---(clear)--------------------------*/
   glClearColor(0.00, 0.50, 0.00, 1.00);
   glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*---(setup view)---------------------*/
   DEBUG_GRAF   yLOG_note    ("set up the view");
   glViewport      (265 - a_wide, 20, a_wide, a_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   /*> glOrtho         (0, a_wide, 0, a_tall, -1000, 1000);                           <*/
   glOrtho         (0, a_wide, 0, a_tall, -1000, 1000);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   DEBUG_GRAF   yLOG_note    ("draw a background for ortho/flat");
   glColor4f (1.00, 0.00, 0.00, 1.00);
   for (x = 0; x <= 1000; x += 10) {
      for (y = 0; y <= 1000; y += 10) {
         /*> printf ("%5dx %5dy\n", x, y);                                            <*/
         glPushMatrix    (); {
            glTranslatef (x, y, 0);
            glBegin         (GL_POLYGON); {
               glVertex3f  ( 0    ,  5    , 0);
               glVertex3f  ( 5    ,  5    , 0);
               glVertex3f  ( 5    ,  0    , 0);
               glVertex3f  ( 0    ,  0    , 0);
            } glEnd   ();
         } glPopMatrix   ();
      }
   }
   /*> yVIEW_color_back (a_part);                                                  <*/
   /*> yCOLOR_opengl (YCOLOR_BAS, YCOLOR_ACC, 1.0);                             <*/
   /*> glBegin         (GL_POLYGON); {                                             <* 
    *>    glVertex3f  (0     , a_tall, -1000);                                     <* 
    *>    glVertex3f  (a_wide, a_tall, -1000);                                     <* 
    *>    glVertex3f  (a_wide, 0     , -1000);                                     <* 
    *>    glVertex3f  (0     , 0     , -1000);                                     <* 
    *> } glEnd   ();                                                               <*/
   /*---(force-out)----------------------*/
   glXSwapBuffers (YX_DISP, YX_BASE);
   glFlush         ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

