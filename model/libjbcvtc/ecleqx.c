#include <math.h>

/* ecleqx.f  contains subs jgtobq,ecleqx,equecx
   30jul92  j.bennett
*/
jgtobq( iaus, eqx, ep)
int iaus;
double eqx, *ep;
{   
   double t, t2, t3;

      if(iaus != 1)
      {
         t = (eqx - 2000.0) * 0.01;
         t2 = t*t;
         t3 = t*t2;

         *ep = (84381.448 - 46.8150*t - 0.00059*t2 + 0.001813*t3)/
                3600.0;

      }
      else
      {
         t = (eqx - 1950.0) * 0.01;
         t2 = t*t;
         t3 = t*t2;

         *ep = (84404.84  - 46.850*t  - 0.0033*t2  + 0.00182*t3)/
                3600.0;

      }
      return 0;
}

ecleqx(iaus,eqx,xlam,beta,rad,decd)
int iaus;
double eqx, xlam, beta, *rad, *decd;
{
   static int nthru = 0, laus = -99;
   static double dtor, rtod, leqx = -1.0, cose, sine;

   double cosb,cosl,sinb,sinl,xl,yl,zl,
          xe,ye,ze,xlamr,betar,rar,decr,e;
   
      if(nthru == 0)
      {
         dtor = atan(1.0) / 45.0;
         rtod = 1.0 / dtor;
         nthru = 1;
      }

      if(eqx != leqx || iaus != laus) 
      {
	 jgtobq(iaus,eqx,&e);
         e = e * dtor;
         cose = cos(e);
         sine = sin(e);
         leqx = eqx;
         laus = iaus;
      }

      xlamr = xlam*dtor;
      betar = beta*dtor;

      cosb = cos(betar);
      cosl = cos(xlamr);
      sinb = sin(betar);
      sinl = sin(xlamr);

      xl = sinb;
      yl = -(cosb*sinl);
      zl = cosb*cosl;

      xe = cose*xl - sine*yl;
      ye = sine*xl + cose*yl;
      ze = zl;

      rar = atan2(-ye,ze);
      *rad = rar * rtod;
      if(*rad < 0.0) *rad = 360.0 + *rad;

/* try to catch pole on any machine (& return ra=0 then) */

      if(fabs(xe) > 1.0)
      {
	 *decd = 90.0*xe/fabs(xe);
         *rad = 0.0;
      }
      else
      {
         decr =  asin(xe);
         *decd = decr * rtod;
         if(fabs(*decd) >= 90.0)
	 {
            *rad = 0.0;
            if(*decd >  90.0) *decd =  90.0;
            if(*decd < -90.0) *decd = -90.0;
	 }
      }
      return 0;
}

equecx(iaus,eqx,rad,decd,xlam,beta)
int iaus;
double eqx, rad, decd, *xlam, *beta;
{
   static int nthru = 0, laus = -99;
   static double dtor, rtod, leqx = -0.1, cose, sine;

   double cosd,cosr,sind,sinr,xl,yl,zl, xe,ye,ze,
	  xlamr,betar,rar,decr,e;

      if(nthru == 0) 
      {
         dtor = atan(1.0) / 45.0;
         rtod = 1.0 / dtor;
         nthru = 1;
      }

      if(eqx != leqx || iaus != laus) 
      {
         jgtobq(iaus,eqx,&e);
         e = e * dtor;
         cose = cos(e);
         sine = sin(e);
         leqx = eqx;
         laus = iaus;
      }

      rar = rad*dtor;
      decr = decd*dtor;

      cosd = cos(decr);
      cosr = cos(rar);
      sind = sin(decr);
      sinr = sin(rar);

      xe = sind;
      ye = -(cosd*sinr);
      ze = cosd*cosr;

      xl = cose*xe + sine*ye;
      yl = -sine*xe + cose*ye;
      zl = ze;

      if(fabs(xl) > 1.0)
      {
	 *beta = 90.0*xl/fabs(xl);
         *xlam = 0.0;
         return 0;
      }
      else
      {
         betar =  asin(xl);
         xlamr = atan2(-yl,zl);
         *xlam = xlamr * rtod;
         if(*xlam < 0.0) *xlam = 360.0 + *xlam;
         *beta = betar * rtod;
         if(fabs(*beta) >= 90.0) 
	 {
            if(*beta >  90.0) *beta =  90.0;
            if(*beta < -90.0) *beta = -90.0;
            *xlam = 0.0;
	 }
      }
      return 0;
}
