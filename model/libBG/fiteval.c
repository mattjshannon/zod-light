/* fiteval.c                                                               */
/*   C version of IDL fiteval                                              */
/*  converted from IDL version    j.bennett @ IPAC                         */
/*  15Mar1999                                                              */
/*   Note: No comments were included in IDL version.                       */

#include <stdio.h>
#include <math.h>

#define  NU0   30.0

double fiteval(freq, alpha, t, tau)
double freq;
double alpha;
double t;
double tau;
{
  double em;
  double y;
  double bnu;

  int planck();

  em = pow((freq / NU0), alpha);
  planck(t, (1.0e+04 / freq), &bnu);
  y = tau * em * bnu / freq * 1.0e+07 * 1.0e+17 / 3.0e+10;
  return(y);
}
