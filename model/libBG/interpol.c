/* interpol.c to do linear interpolation                              */
/*   04feb1999  j.bennett                                             */
/*   updated 01mar1999 to check no.pts.                               */

/*   returns -1 if interpolation cannot be done, 0 otherwise.         */
  
#include <math.h>
#include <stdio.h>

static double interp();

int interpol(x, y, npts, x0, y0)
double x[];    /* abscissa.  abscissa values must be monotonically   */
               /*   increasing or decreasing.                        */
double y[];    /* ordinate.                                          */
int npts;      /* number of x,y pairs                                */
double x0;
double *y0;
{
  int i;
  int err;

  err = 0;
  if (npts < 2)
  {
    if (npts < 1)
    {
      err = -1;
      fprintf(stderr,"#interpol: error, npts < 1.\n");
    }
    else if (npts == 1)
      *y0 = y[0];
    return(err);
  }

  if (x[1] > x[0])         /* abscissa in increasing value.  */
  {
    for (i = 1; i < npts; i++)
    {
      if (x[i] > x0)
        break;
    }
    if ( i >= npts) i = npts - 1;   /* allow extrapolation */
  }
  else                      /* abscissa in decreasing value.  */
  {
    for (i = 1; i < npts; i++)
    {
      if (x[i] < x0)
        break;
    }
    if ( i >= npts) i = npts - 1;
  }
  *y0 = interp( y[i-1], y[i], x[i-1], x[i], x0, &err);
  return(err);
}
/* ------------------------------------------------------------------------ */
/*   interp.c  C version of j.bennett FORTRAN yintrp.f (of 12feb1982) */
/*   04feb1999  j.bennett  with modifications for interpol.c          */
/*   retcode contains answer, if error occurred errflag = -1, 0 otherwise */
  
static double interp(y1, y2, x1, x2, x0, errflag)
double y1, y2, x1, x2, x0;
int    *errflag;
{
  double div;
  double ans;

  *errflag = 0;
  div = x2 - x1;
  if (div != 0.0)
  {
    ans = (((y2 - y1) / div) * (x0 - x1)) + y1;
    return(ans);
  }
  else
  {
    fprintf(stderr, "#interp: error: divisor (x2 - x1) == 0\n");
    ans = 0.0;
    *errflag = -1;
    return(ans);
  }
}
