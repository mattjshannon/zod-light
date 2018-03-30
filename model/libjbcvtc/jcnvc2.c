#include <math.h>

jcnvc2(isys1,epoki,x0,y0,isys2,epoko,xnew,ynew,tobsin)
int isys1, isys2;
double epoki,x0,y0, epoko,*xnew, *ynew, tobsin;
{
/*
*  26mar-01apr92 j.bennett
*  This updated version, aug92, is totally restructured in order to
*    handle the addition of ecliptic of date; also problems in the pre-release
*    version have been fixed. (Pre-release version failed to produce 
*    accurate results if input was Bess. equ (isys1 =1) with equinox other
*    than 1950.0d0 and output was Julian equatorial (isys2 =0) with equinox
*    2000.0d0 (e.g. B1975 to J2000 conversion was wrong).  Also,
*    when isys1  was 2,3,or 4 (galactic,ecliptic,supergalactic) and 
*    isys2  =1 (Bess. equ) and equinox not 1950.0d0, result was wrong).
*
*  jcnvc2 is version of jcnvcd that can handle J2000 input and output and
*         do appropriate conversions before/after calling ecl,gal,sgl conv.
*         routines.
*         E-term removal/replacement assumed in B1950 <==>J2000 conversions.
*
*  16Sep92 to comment out write on unit 0
*  24aug92 updated comments only.
*  11-13,20aug92 to implement ecliptic of date capability.
*
*    calls gtjul2, unjul2, nwcprc, fk5prc
*    calls ecleqx,equecx,equgad,galeqd,galsgl,sglgal
*     the new subroutines compute(the first time thru & store) values for
*     the galactic <==> equatorial conversion matrices from the
*     actual angles so that maximum precision may be maintained for
*     the machine being used for computation. Also better agreement is
*     obtained when returning to input coord system from output values.
*
*    Also radian to deg., etc. computed using machine functions (i.e.
*     radian-to-deg = datan(1.0d0) / 45.0d0 )
*    If |dec| or |lat| = 90.0d0, the corresponding ra or lon = 0.0d0.
*
* x0,y0     = input lon and lat in degrees,
* xnew,ynew = output lon and lat in degrees.
* epoki,epoko input output equinox (used if jsys1,jsys2 0, 1, 3, 11,or 13).
*    epok_ ignored for galactic (jsys_=2) and supergalactic (isys_=4).
*    If epok_ input as 0.0d0, then 1950.0d0 used if isys_ is 1 or 3;
*                                  2000.0d0 used if isys_ is 0,11 or 13.
* isys1  = input coordinate system#;  isys2 = output system#.
*     0 = equ(Jul. ),     1 = equ(Bess),
*     2 = gal,            3 = ecl(Bess),     4 = supergalactic (sgl)
*     11= equ(Jul.),      13= ecl(Jul.)
* tobsin is observation epoch used when B1950 <==> J2000 conversions required.
*   Note:gal and sgl conversions require B1950 as input, give B1950 as output
*   when working with the original subroutines. jcnvc2 will convert between
*   B1950 and J2000 as needed and as appropriate.
*
*/
      int jsys1,jsys2,iauf1,iauf2;
      double x,y,tobs;
      double xin,yin,poki,poko;

      tobs = tobsin;
      if(tobs == 0.0) tobs = 1950.0;
/*
*                       iauf1 = 1 for Besselian; iauf1 = 2 for Julian.
*                       iauf2 = 1 for Besselian; iauf2 = 2 for Julian.
*
*/
      iauf1 = 1;
      iauf2 = 1;
      jsys1 = isys1;
      jsys2 = isys2;

      if(isys1 == 11 || isys1 == 0)
      {
         iauf1 = 2;
         jsys1 = 0;
      }
      else if (isys1 == 13) 
         iauf1 = 2;

      if(isys2 == 11 || isys2 == 0)
      {
         iauf2 = 2;
         jsys2 = 0;
      }
      else if (isys2 == 13) 
         iauf2 = 2;

      poki = epoki;
      poko = epoko;
      if(jsys1 == 2 || jsys1 == 4)
         poki = 1950.0;
      else
      {
         if(poki == 0.0) 
         {
            if(iauf1 == 2)
               poki = 2000.0;
            else
               poki = 1950.0;
         }
      }

      if(jsys2 == 2 || jsys2 == 4) 
         poko = 1950.0;
      else
      {
         if(poko == 0.0) 
         {
            if(iauf2 == 2) 
               poko = 2000.0;
            else
               poko = 1950.0;
         }
      }

      if(jsys1 == jsys2 && poki == poko) 
      {
         *xnew = x0;
         *ynew = y0;
         return 0;
      }

      xin = x0;
      yin = y0;
/*
* see if ecliptic is involved in input - if so, convert now to equatorial
*   and reset input flags.
*/
      if(jsys1 == 3 || jsys1 == 13)
      {
         ecleqx(iauf1,poki,x0,y0,&xin,&yin);
         if(iauf1 == 1) 
            jsys1 = 1;
         else
            jsys1 = 0;
         if(jsys2 == jsys1 && poki == poko)
	 {
            *xnew = xin;
            *ynew = yin;
            return 0;
         }
      }

      else if(jsys1 == 4) 
      {
/*                      see if supergalactic or galactic on input: */
          sglgal(xin,yin,&x,&y);
          if(jsys2 == 2)
	  {
             *xnew = x;
             *ynew = y;
             return 0;
	  }
          galeqd(x,y,&xin,&yin);
          jsys1 = 1;
      }

      else if(jsys1 == 2) 
      {
         if(jsys2 == 4) 
         {
            galsgl(xin,yin,xnew,ynew);
            return 0;
         }
         galeqd(xin,yin,&x,&y);
         xin = x;
         yin = y;
         jsys1 = 1;
      }

      if(isys2 == jsys1 && poki == poko)
      {
         *xnew = xin;
         *ynew = yin;
         return 0;
      }
/*
* see if ecliptic output desired - first set up to get appropriate
*                intermediate equatorial output:
*/
      if(isys2 == 3) 
         jsys2 = 1;
      else if (isys2 == 13) 
         jsys2 = 0;
/*
* see if equatorial (or redefined equatorial on input ( bess or julian )
*/
      if(jsys1 == 0) 
      {
/*                   input is Julian; is output (iauf2=1) to be bess? */
       if(iauf2 == 1) 
       {
          if(poki != 2000.0) 
	  {
             fk5prc(poki,xin,yin,2000.0,&x,&y);
             xin = x;
             yin = y;
             poki = 2000.0;
	  }
          unjul2(xin,yin,tobs,1,poko,&x,&y);
          if(isys2 == 1)
          {	
             *xnew = x;
             *ynew = y;
             return 0;
	  }
          else
	  {
             xin = x;
             yin = y;
             poki = poko;
             iauf1 = 1;
             jsys1 = 1;
	  }
       }
       else
       {
/*            else(iauf2=2) output to be Julian (equ or ecl) */
           if(poki != poko) 
	   {
              fk5prc(poki,xin,yin,poko,&x,&y);
              xin = x;
              yin = y;
              poki = poko;
	   }

           if(isys2 == 0 || isys2 == 11)
	   {
              *xnew = xin;
              *ynew = yin;
              return 0;
	   }
	}
      }
      else if(jsys1 == 1) 
      {
/*                               input Bess; is output Bess ? */
         if(iauf2 == 1) 
         {
            if(poki != poko) 
	    {
               nwcprc(poki,xin,yin,poko,&x,&y);
               xin = x;
               yin = y;
               poki = poko;
	    }
            if(isys2 == 1) 
	    {
               *xnew = xin;
               *ynew = yin;
               return 0;
	    }
         }
         else
         {
/*                else(iauf2=2) ouput is to be Julian (equ or ecl) */
            gtjul2(poki,xin,yin,tobs,1,&x,&y);
            if(poko != 2000.00) 
	    {
               xin = x;
               yin = y;
               fk5prc(2000.0,xin,yin,poko,&x,&y);
	    }
            if(isys2 == 0 || isys2 == 11) 
	    {
               *xnew = x;
               *ynew = y;
               return 0;
	    }
            else
	    {
               xin = x;
               yin = y;
               poki = poko;
               iauf1 = 2;
               jsys1 = 0;
	    } 
         }
      }

      if(isys2 == 3 || isys2 == 13) 
      {
         equecx(iauf2,poko,xin,yin,xnew,ynew);
         return 0;
      }
      if(isys2 == 4 || isys2 == 2) 
      {
         equgad(xin,yin,&x,&y);
         if(isys2 == 4) 
             galsgl(x,y,xnew,ynew);
         else
         {
            *xnew = x;
            *ynew = y;
         }
         return 0;
      }
      return 0;
}
