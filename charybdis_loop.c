#include "charybdis.h"

#define     NSEC        1000000000
typedef     struct timespec   tTSPEC;

typedef     long long  llong;

static   llong l_beg   = 0;
static   llong l_end   = 0;
static   llong l_prev  = 0;

static   llong l_secs  = 0;
static   llong l_nsec  = 0;

static   llong l_exp   = 0;
static   llong l_act   = 0;
static   llong l_used  = 0;
static   llong l_sleep = 0;

char
LOOP_init               (float a_base)
{
   l_secs = l_nsec = 0.0;
   if (a_base >= 1.0)  l_secs  = trunc (a_base);
   l_nsec  = (a_base - l_secs) * NSEC;
   return 0;
}

char
LOOP_beg                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tTSPEC      x_dur;
   /*---(reset timing);------------------*/
   l_beg   = 0;
   l_end   = 0;
   /*---(get beginning time)-------------*/
   clock_gettime  (CLOCK_MONOTONIC_RAW, &x_dur);
   l_beg   = x_dur.tv_sec * NSEC;
   l_beg  += x_dur.tv_nsec;
   /*---(complete)-----------------------*/
   return 0;
}

char
LOOP_end                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tTSPEC      x_dur;
   /*---(get beginning time)-------------*/
   clock_gettime  (CLOCK_MONOTONIC_RAW, &x_dur);
   l_end   = x_dur.tv_sec * NSEC;
   l_end  += x_dur.tv_nsec;
   /*---(calc diffs)---------------------*/
   l_exp   = (l_secs * NSEC) + l_nsec;
   l_act   = l_end - l_prev;
   l_used  = l_end - l_beg;
   l_sleep = l_exp - l_used;
   /*---(calc run diffs)-----------------*/
   l_prev  = l_end;
   /*---(sleep)--------------------------*/
   x_dur.tv_sec  = l_sleep / NSEC;
   x_dur.tv_nsec = l_sleep % NSEC;
   nanosleep      (&x_dur, NULL);
   /*---(complete)-----------------------*/
   return 0;
}


