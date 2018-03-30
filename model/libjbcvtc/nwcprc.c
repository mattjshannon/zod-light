#include<math.h>

nwcprc(eqx1,rain,decin,eqx2,raou,decou)
double eqx1,rain,decin,eqx2,*raou,*decou;
{
/* 22jul92: nwcprc and nwcprp.
* j.bennett
* for Besselian (aka Newcomb) precession.
*  method of fk5prc, but with constants from herget general precession(jprecj;
*              also see p.6 of Publ.Cincinnati Obs. No.24)
*  For object without proper motion, nwcprc yields essentially the same
*  results as jprecj (within +/-0.2e-08 arcseconds);  nwcprcp allows
*  rotation matrix computation method of precessing position and propermotions
*  for objects with proper motions. With jprecj propermotions had to be
*  applied outside of the jprecj routine.
*
* All arguments are double precision:
*  eqx1 is equinox of input position (in year - e.g. 1950.0d0)
*  rain, decin is input position in decimal degrees at eqx1.
*  eqx2 is eqinox of output position (in year - e.g. 1992.0d0)
*  raou, decou is position in decimal degrees at eqx2.
*
*/
      double dumpma,dumpmd;
      nwcprp(eqx1,rain,decin,0.0,0.0,0.0,0.0,
	     eqx2,raou,decou,&dumpma,&dumpmd);
      return 0; 
}

nwcprp(eqx1,rain,decin,pmain,pmdin,pin,vin,
       eqx2,raou,decou,pmaou,pmdou)
double eqx1,rain,decin,pmain,pmdin,pin,vin,
       eqx2,*raou,*decou,*pmaou,*pmdou;
{
/*  j.bennett - for Besselian (aka Newcomb) precession:
*              using constants from herget general precession (jprecj;
*              also see p.6 of Publ.Cincinnati Obs. No.24)
*
*  allows precession of proper motion values (also will make use of
*    parallax and radial velocity if not zero.
*
* All arguments are double precision:
*  eqx1 is equinox of input position (in year - e.g. 1950.0d0)
*  rain, decin is input position in decimal degrees at eqx1.
*  pmain is proper motion in ra in seconds of time per Tropical century.
*  pmdin is proper motion in dec in seconds of arc per Tropical century.
*  pin is parallax in arc seconds (0.0d0 if none)
*  vin is radial velocity in km/sec (0.0d0 if none)
*  eqx2 is eqinox of output position (in year - e.g. 1992.0d0)
*  raou, decou is position in decimal degrees at eqx2.
*  pmaou is proper motion in ra in seconds of time per Tropical century for eqx2
*  pmdou is proper motion in dec in seconds of arc per Tropical century for eqx2
*
*******************************************************************************
* method from Fricke,W.,Schwan,H.,Lederle,T.,1988,Fifth Fundamental Catalogue
*     (FK5) Part I. (pages 10-11)
*******************************************************************************
*/
      static double rtod,dtor,delt,f,p[3][3],leqx1 = -1.0, leqx2 = -1.0;

      int i;
      double  zetar,zr,thetar,zeta,z,theta;
      double  tau, t;
      double  r0[3],rdot0[3],r[3],rdot[3],pivelf;
      double  czet,cthet,cz,szet,sthet,sz;
      double  cosa,sina,cosd,sind,rar,decr;
      double  cosao,sinao,cosdo,sindo;
      double  pmas,pmds,rdiv,raour,decour,pmaous,pmdous;
      double  duda[3],dudd[3];

      if(eqx1 == eqx2)
      {
         *raou = rain;
         *decou = decin;
         *pmaou = pmain;
         *pmdou = pmdin;
         return 0;
      }

      if(eqx1 != leqx1 || eqx2 != leqx2) 
      {
      dtor = atan(1.0) / 45.0;
      rtod = 1.0 /dtor;
/* tau, t, delt in Tropical centuries: */
      tau =  (eqx1 - 1900.0) * 0.01;
      t    = (eqx2 - eqx1) * 0.01;
      delt = t;
/*                   f is value for 1 arcsec in radian measure. */
      f    = 4.8481368e-06;
      leqx1 = eqx1;
      leqx2 = eqx2;
/*
*    zeta, theta, z in seconds of arc:
*
*/
      zeta = (2304.2530 + 1.39750*tau + 0.000060*tau*tau)*t +
             (0.30230  - 0.000270*tau)*t*t + 0.018000*t*t*t;

      z = (2304.2530 + 1.39750*tau + 0.000060*tau*tau)*t +
          (1.09500 + 0.000390*tau)*t*t + 0.018320*t*t*t;

      theta = (2004.6850 - 0.85330*tau - 0.000370*tau*tau)*t -
              (0.42670 + 0.000370*tau)*t*t - 0.041800*t*t*t;
      zetar = (zeta/3600.0)*dtor;
      zr   = (z/3600.0)*dtor;
      thetar = (theta/3600.0)*dtor;
      czet = cos(zetar);
      szet = sin(zetar);
      cz   = cos(zr);
      sz   = sin(zr);
      cthet= cos(thetar);
      sthet= sin(thetar);
/*
* p matrix from Green,Robin M.,1985,Spherical Astronomy, Cambridge University
*   Press, p. 221-222.
*
*/
      p[0][0] = czet*cthet*cz - szet*sz;
      p[1][0] = czet*cthet*sz + szet*cz;
      p[2][0] = czet*sthet;
      p[0][1] = -szet*cthet*cz - czet*sz;
      p[1][1] = -szet*cthet*sz + czet*cz;
      p[2][1] = -szet*sthet;
      p[0][2] = -sthet*cz;
      p[1][2] = -sthet*sz;
      p[2][2] = cthet;

      }

      rar = dtor*rain;
      decr = dtor*decin;
      cosa = cos(rar);
      sina = sin(rar);
      cosd = cos(decr);
      sind = sin(decr);
      r0[0] = cosd*cosa;
      r0[1] = cosd*sina;
      r0[2] = sind;

      pmas = pmain * 15.0;
      pmds = pmdin;
      if(vin == 0.0 || pin == 0.0) 
      {
         rdot0[0] = f*(pmas*(-cosd)*sina + pmds*(-sind)*cosa);
         rdot0[1] = f*(pmas*cosd*cosa + pmds*(-sind)*sina);
         rdot0[2] = f*(pmds*cosd);
      }
      else
      {
         pivelf = 21.094953 * pin *vin;
         rdot0[0] = f*(pmas*(-cosd)*sina + pmds*(-sind)*cosa+pivelf*r0[0]);
         rdot0[1] = f*(pmas*cosd*cosa + pmds*(-sind)*sina + pivelf*r0[1]);
         rdot0[2] = f*(pmds*cosd + pivelf*r0[2]);
      } 
      for (i=0; i<3; i++)
      {
         rdot[i] = p[i][0]*rdot0[0] + p[i][1]*rdot0[1] + p[i][2]*rdot0[2];
         r[i] = p[i][0]*(r0[0]+rdot0[0]*delt) +
                p[i][1]*(r0[1]+rdot0[1]*delt) +
                p[i][2]*(r0[2]+rdot0[2]*delt);
      }

      raour = atan2(r[1],r[0]);
      decour = atan2(r[2], sqrt(r[0]*r[0] + r[1]*r[1]));
      rdiv = sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
      cosdo = cos(decour);
      sindo = sin(decour);
      cosao = cos(raour);
      sinao = sin(raour);

      duda[0] = -cosdo*sinao;
      duda[1] =  cosdo*cosao;
      duda[2] =  0.0;
      dudd[0] = -sindo*cosao;
      dudd[1] = -sindo*sinao;
      dudd[2] =  cosdo;
      rdot[0] = rdot[0] / rdiv;
      rdot[1] = rdot[1] / rdiv;
      rdot[2] = rdot[2] / rdiv;
      pmaous = (rdot[0]*duda[0] + rdot[1]*duda[1] + rdot[2]*duda[2])/
               (f*cosdo*cosdo);
      pmdous = (rdot[0]*dudd[0] + rdot[1]*dudd[1] + rdot[2]*dudd[2])/f;

      *raou = raour * rtod;
      if(*raou > 360.0) 
         *raou = *raou - 360.0;
      else if (*raou < 0.0) 
         *raou = *raou + 360.0;

      *decou = decour * rtod;
/*
* since decour is output of datan2(y,x) with x always positive, if dec
*  .gt. 90. it is due to rounding when being converted to degrees.
*/
      if(*decou > 90.0) 
         *decou = 90.0;
      else if (*decou < -90.0) 
         *decou = -90.0;

      *pmaou = pmaous / 15.0;
      *pmdou = pmdous;

      return 0;
}
