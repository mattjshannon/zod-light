#include <math.h>


jbdist(xlon1,xlat1,xlon2,xlat2,distd)
double xlon1,xlat1,xlon2,xlat2,*distd;
{
/*
*
* updated 09jun92 to test costh ( to prevent acos error message when
*   round off near theta = 0. or 180.deg creates a dabs(cos(theta)) slightly
*   greater than 1.0d0)
* \ned\jbdist.f  28feb91  like prev. versions except does not call
*     fdtor2/frtod2
* f77 -c jbdist.f
* for vax, dacos is used instead of darcos.
* all args in double precision degrees.  distd returned.
*
* distd is distance between 2 positions on sky - results in degrees
*
*/

   static int nthru = 0;
   static double dtor, rtod;

   double xlon1r,xlon2r, xlat1r,xlat2r,costh;

   if(nthru == 0)
   {
      dtor = atan(1.0) / 45.0;
      rtod = 1.0 / dtor;
      nthru = 1;
   }

   xlon1r =  dtor * xlon1;
   xlon2r =  dtor * xlon2;
   xlat1r =  dtor * xlat1;
   xlat2r =  dtor * xlat2;

   costh = cos(xlat1r)*cos(xlat2r)*cos(xlon1r-xlon2r) +
           sin(xlat1r)*sin(xlat2r);

   if(fabs(costh) > 1.0) costh = costh/fabs(costh);
   *distd =  rtod*( acos(costh));
   return 0;
}
