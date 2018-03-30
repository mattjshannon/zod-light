#include<math.h>

jbposa(xlon1i,xlat1i,xlon0i,xlat0i,distd,pa,ipf)
int    *ipf;
double xlon1i,xlat1i,xlon0i,xlat0i,*distd, *pa;
{
/*
*
* 23feb91 updated some comments.
* 22feb91 to make sure pa is 0 to 360 (i.e.never < 0 )in sub jbelpt.
* 19feb91 jbposa2.f  to allow 0 to 360 deg pa
*  26,29oct90 to add pole test to jbposa sub.
*
* 28sep90 to include slightly modified ipac prec. routines
* 26sep90 to improve comments & use smaller value for value variable small.
* 10sep90 jbposa modification of ned version of jbdist to return pa
*   (0 - 180 deg. E of N) if distd < 90 deg.
* jbposa finds distance between 2 positions & (if distd <90.deg) finds
*   position angle (pa) of (xlon1,xlat1) with respect to (xlon0,xlat0)
*
* all non-integer args in double precision degrees.distd,pa,ipf returned.
*
* for discussion of xlon1, etc. see pa explanation below.
*
* distd is distance between 2 positions on sky - results in degrees.
*
* pa is position angle (degrees E of N) -  0 to 180 deg. returned and
*  is valid only if distd < 90. deg.
*  pa is sensitive to what is called xlon1,xlat1 and what is called
*  xlon0,xlat0.
*   for use w/ ellipse  xlon1,xlat1 is pt on ellipse;
*                       xlon0,xlat0 is center of ellipse.
*
* ipf is integer flag for pa compution: 0 = not computed; 1 = computed;
*      2=computed and one of points treated as though exactly at pole.
*      if ipf = 0, and distd .ne. 0, distd was computed.
*
* NOTE: reference for the equations used:
*   A Compendium of Spherical Astronomy by Simon Newcomb
*    Section 56 (page 111)
*
*  where s is distance (in degrees), p is position angle (in degrees)
*        dela = alpha' - alpha
*     sin(s)*sin(p) =                           cos(d')*sin(dela)
*     sin(s)*cos(p) = cos( d)*sin(d') - sin( d)*cos(d')*cos(dela)
*            cos(s) = sin( d)*sin(d') + cos( d)*cos(d')*cos(dela)
*        Therefore:
*          s = acos(cos(s))
*          p = atan2(sin(p) , cos(p))
*      p is counted from the meridian passing north through position
*        at (alpha,d)  toward the east.
*
* NOTE: maximum precision in distd is .01" on machine with a 64-bit
*       double precision word. On cyber, with 128-bit double precision
*       word, precision may exceed .00001" .  The worst case for distd
*       is near the poles.
*       Precision for posangle is within .5 degree which is usually
*       considered, astronomically speaking, more than satisfactory.
*       Actual test cases have indicated pa is within 1.2d-06" on
*       a UNIX machine (i.e. 64-bit double precision)- however not
*       all possible cases have been tested.
*
*/
   static int nthru = 0;
   static double d2r, r2d;

   int ipole, itest = 2, itesti;
   double xlon1,xlat1,xlon0,xlat0,xlon1r,xlon0r, xlat1r,xlat0r,costh;
   double ds,delar,sinp,cosp,p;
   double dellat,dellon,dist,dwork;
   double small = 1.0e-10, tfac = 36000000.0; 

      if (nthru == 0)
      {
         d2r = atan(1.0)/45.0;
         r2d = 1.0/d2r;
         nthru = 1;
      }

      *pa  = 0.0;
      *ipf = 0;

/* ipole=1 if xlat0 at a pole; ipole=2 if xlat1 is at a pole. */
      ipole = 0;

      if((fabs(xlat0i) == 90.0) && (xlat1i == xlat0i)) 
      {
          *distd = 0.0;
          return 0;
      }

      xlat0 = xlat0i;
      xlon0 = xlon0i;
      xlat1 = xlat1i;
      xlon1 = xlon1i;

/* if lat is within .0002" of pole, treat as pole: */
      if(fabs(xlat0i) > 89.9999) 
      {
         itesti = (int)((90.0-fabs(xlat0i)) * tfac);
         if(abs(itesti) <= itest)
         {
	    xlat0 = 90.0*xlat0i/fabs(xlat0i);
            xlon0 = 0.0;
            ipole = 1;
         }
      }

      if(fabs(xlat1i) > 89.9999) 
      {
         itesti = (int)((90.0-fabs(xlat1i)) * tfac);
         if(abs(itesti) <= itest) 
         {
	    xlat1 = 90.0*xlat1i/fabs(xlat1i);
            xlon1 = 0.0;
            ipole = 2;
         }
      }

      if(fabs(xlat0) == 90.0 && xlat1 == xlat0) 
      {
         *distd = 0.0;
         return 0;
      }

      xlon1r = d2r * xlon1;
      xlon0r = d2r * xlon0;
      xlat1r = d2r * xlat1;
      xlat0r = d2r * xlat0;
      delar = xlon1r - xlon0r;
      costh = cos(xlat1r)*cos(xlat0r)*cos(delar) + sin(xlat1r)*sin(xlat0r);

      if(costh < -1.0) costh = -1.0;
      if(costh >  1.0) costh =  1.0;

      ds = acos(costh);
      *distd = r2d * ds;

      if(fabs(*distd) >= 90.0 ) return 0;
      if(xlat1 == xlat0 && xlon1 == xlon0) return 0; 
      if(fabs(*distd) < 1.667e-02)
      {
/* alternate method okay for short distances and avoids using so many
   trig functions that may exceed precision of machine being used. */
         dellat = xlat1 - xlat0;
         dellon = fabs(xlon1 - xlon0);
         if(dellon > 180.0) dellon = 360.0 - dellon;
         dellon = dellon * cos(xlat0r);
         dist = sqrt(dellat*dellat + dellon*dellon);
         *distd = dist;
         ds = d2r * *distd;
         if(dist <= small) return 0;
      }

/* compute position angle if possible */
      *ipf = 1;
      if(ipole > 0) *ipf = 2;
      sinp = cos(xlat1r)*sin(delar) / sin(ds);
      if(fabs(sinp) < 1.0e-09)
      {
         *pa = 0.0;
         if(90.0-fabs(xlat0) >= *distd)
         {
            if(xlat1 < xlat0) 
	       *pa = 180.0;
         }
         else
	 {
            dwork = fabs(xlon0 - xlon1);
            if(dwork > 180.0) 
	       dwork = fabs(dwork -360.0);
            if(xlat0 >   0.0) 
	    {
/*                                          north pole: */
               if(dwork <= 1.0 && ipole != 2) *pa = 180.0;
	    }
	    else
	    {
/*                                          south pole: */	       
	       if(dwork >= 179.0 || ipole == 2) *pa = 180.0;
	    }
         }
         return 0;
      }
      cosp = (cos(xlat0r)*sin(xlat1r)-sin(xlat0r)*cos(xlat1r)*
              cos(delar)) / sin(ds);
      if(fabs(cosp) < 1.0e-09) 
      {
         *pa = 90.0;
         dwork = xlon0 - xlon1;
         if(fabs(dwork) < 180.0)
	 {
            if(dwork > 0.0) 
	       *pa = 270.0;
	 }
         else 
	 {
	    if(dwork < 0.0) 
	       *pa = 270.0;
	 }
         return 0;
      } 
      p = atan2(sinp, cosp);
      *pa = r2d * p;
      if (*pa <    0.0) *pa = 360.0 + *pa;
      if (*pa >= 360.0) *pa = *pa - 360.0;

      return 0;
}

jbelpt(xlonin,xlatin,distd,pain,xlonp,xlatp,iflg)
int    *iflg;
double xlonin,xlatin,distd,pain,*xlonp,*xlatp;
{
/*
*
* finds pt on end of ellipse given center xlonin & xlatin, posang (pa)
*    and major axis semi-diameter (distd). Returns xlonp,xlatp & iflg).
*    all non-integer args are double precision degrees.
*   iflg indicates processing: 1 completely "normal"
*        0 = distd too small (new pt = old pt on return)
*        2 = posang too small (treated as though pa= 0.)
*            ( note 3 can override iflg=2)
*        3 = treated xlonin,xlatin as pole:  e.g. dabs(xlatin) close
*            to 90.deg, treated as + or - 90 & xlonin was not 0 on
*            input, but was treated as though zero.
*             Note: test for pole used here is that any position with
*                   dabs(xlatin) > 89.99999d0 (89d59'59.964") is at the
*                   pole  and is treated as pole with xlon = 0 deg. and
*                   xlat as -90. or + 90. deg as appropriate.
*        4 = xlonp,xlatp should be treated as pole, however computed
*            pos has not been revised. Calling prog. may want to revise pos.
*
* NOTE: reference for the equations used:
*   A Compendium of Spherical Astronomy by Simon Newcomb
*    Section 56 (page 111)
*
*/

      static int nthru=0;
      static double d2r, r2d;

      int donext = 1;
      double xlon0, xlat0, xlat0r, xlatpr, distr;
      double pa, par, dela, delar, part, b;
      double cosdp,sindp,sinpar;
      double small = 1.0e-09, test=89.99999;

      if (nthru == 0)
      {
         d2r = atan(1.0)/45.0;
         r2d = 1.0/d2r;
         nthru = 1;
      }

      test = 89.99999;
      if(90.0-test > distd) 
         test= 90.0 - (distd - 0.1*distd);
      *iflg = 1;
      xlon0 = xlonin;
      xlat0 = xlatin;

      if(distd < small) 
      {
         *xlonp = xlon0;
         *xlatp = xlat0;
         *iflg = 0;
         return 0;
      }

      if(fabs(xlat0) >= test) 
      {
         if(xlon0 >= 360.0) xlon0 = xlon0 - 360.0;
         if(fabs(xlon0) > small) 
         {
          *iflg = 3;
          xlon0 = 0.0;
          if(xlat0 < 0.0) xlat0 = -90.0;
          if(xlat0 > 0.0) xlat0 =  90.0;
         }
      } 

      pa = pain;
      if(pain < 0.0) pa = 360.0 + pain;
      distr = d2r * distd;
      par   = d2r * pa;
      sinpar = sin(par);

/*                                      here pa either near 0 or 180: */
      if(fabs(sinpar) < small)
      {
         if(*iflg != 3) *iflg = 2;
         *xlonp = xlon0;
         if( pa <= 90.0 || pa >= 270.0)
         {
            *xlatp = xlat0 + distd;
            if(*xlatp > 90.0)
	    {
               *xlatp = 180.0 - *xlatp;
               *xlonp = xlon0 + 180.0;
            }
         }
         else
	 {
            *xlatp = xlat0 - distd;
            if(*xlatp < -90.0)
	    {
               *xlatp = -(180.0 + *xlatp);
               *xlonp = xlon0 + 180.0;
	    }
         }
         if(*xlonp >= 360.0) *xlonp = *xlonp - 360.0;
         if(fabs(*xlatp) >= test) *iflg = 4;
	 return 0;
      } 

      part   = sin(distr) * sinpar;
      xlat0r = d2r * xlat0;

      if(distd <= 1.667e-02 && fabs(xlat0) <= 89.0)
      {
         xlatpr = xlat0r + distr*cos(par);
         delar  = distr * sinpar/cos(xlatpr);
         dela   = r2d*delar;
         if(dela <= 360.0) 
	    donext=0;
      }

      if (donext == 1)
      {
         if(fabs(xlat0) < small)
         {
            b = cos(distr) / part;
            delar = atan(1.0 / b);
            cosdp = part/sin(delar);
            sindp = (sin(distr)*cos(par)+cosdp*sin(xlat0r)*cos(delar))/
                     cos(xlat0r);
            xlatpr = atan(sindp/cosdp);
         }
         else
         {
            b = (cos(distr)*cos(xlat0r) - 
                 sin(distr)*cos(par)*sin(xlat0r))/part;
            delar = atan(1.0 / b);
            cosdp = part/sin(delar);
            sindp = (cos(distr) - b*cos(xlat0r)*part)/sin(xlat0r);
            if(fabs(cosdp) >= 1.0e-09)
               xlatpr = atan2(sindp,cosdp);
            else
               xlatpr = asin(sindp);
         }

         dela = r2d*delar;
      }

      *xlonp = xlon0 + dela;
      if(*xlonp >= 360.0) *xlonp = *xlonp - 360.0;
      if(*xlonp <    0.0) *xlonp = *xlonp + 360.0;

      *xlatp = r2d * xlatpr;
      if(fabs(*xlatp) < test) return 0;

      if(fabs(*xlatp) > 90.0) 
      {
         if(*xlatp < 90.0)
            *xlatp = -(180.0 + *xlatp);
         else
            *xlatp = 180.0 - *xlatp;
         *xlonp = *xlonp - 180.0;
         if(*xlonp < 0.0) *xlonp = *xlonp + 360.0;
      }


/* set flag so user may reset xlonp,xlatp to pole val w/ ra=0 */

      return 0;
}
