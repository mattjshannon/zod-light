#include <math.h>

/*
* \ned\gtjpa.f  12-13mar92
*   03aug92; updated comments only.
*   23jul92 to call nwcprc/nwcprp instead of jprecj. Also, precesses
*     to 1950 (if necessary) before call to gtjulp or from 1950 (if
*     necessary) after call to unjulp (i.e. for propermotion types only).
*
*        contains gtjpa, unjpa, gtjpap, unjpap
*/

gtjpa (eqxin,ra,dec,pab,tobsin,ieflg,raout,decout,paj,ierr)
int ieflg, *ierr;
double eqxin,ra,dec,pab,tobsin,*raout,*decout,*paj;
{
/*
*
*       ALL FLOATING POINT ARGS ARE DOUBLE PRECISION
*
* eqxin is the equinox(besselian) of the input position.
* ra,dec is input position in decimal degrees at equinox eqxin.
* pab  = input position angle (0 to 360.0d0 deg.) at eqxin.
* tobsin is year of observation (e.g. 1983.5 for iras sources)
*         if 0.0d0, then tobsin is treated as though equal to eqxin.
* ieflg  of -1 indicate to not remove E-terms from position in computing
*             position angle (any other value allows E-term removal)
* raout,decout is output position in decimal degrees at J2000.0
* paj is output position angle (0 to 360.0d0 deg.) for J2000.0
*         if ierr = 0 ( garbage otherwise).
* ierr = 0 for normal return. -1 if paj could not be computed.
*
* NOTE:
* This routine assumes input pos angle & input position are in the equatorial
*    coordinate system.
*
* calls jbposa,jbelpt,gtjul2
*
*/

   int    iflg,ipf;
   double plonp0,platp0,plonp1,platp1,plon0,plat0,plon1,plat1,dumd;
   double pain,paou,eqx,tobs;
   double dist = 0.1;

   *ierr = 0;
   eqx = eqxin; 
   if(eqxin == 0.0) eqx = 1950.0;
   tobs = tobsin;
   if(tobsin == 0.0) tobs = eqxin;

   plonp0 = ra;
   platp0 = dec;
   pain = pab;
   gtjul2(eqx, ra, dec, tobs, ieflg, raout, decout);

/*
*
* find end point at equinox eqxin
* note: plonp0 and platp0 are used for pos angle proc. and may
*        be slightly adjusted here.  To get proper central position 
*        one must precess ra and dec.
*
*/
      plon0 = *raout;
      plat0 = *decout;

      jbelpt(plonp0,platp0,dist,pain,&plonp1,&platp1,&iflg);
      if(iflg == 3)
      {
/*          recompute plonp0,platp0 for pos angle processing if near a pole:
*/
         plonp0 = 0.0;
         if(platp0 < 0.0)
            platp0 = -90.0;
         else
            platp0 = 90.0;
         gtjul2(eqx,plonp0,platp0,tobs,ieflg,&plon0,&plat0);
      }

      if(iflg == 4)
      {
         plonp1 = 0.0;
         if(platp1 < 0.0)
            platp1 = -90.0;
         else
            platp1 = 90.0;
      }

/* precess pt at apex of ellipse:*/
   gtjul2(eqx,plonp1,platp1,tobs,ieflg,&plon1,&plat1);
/*
*   get new pos angle
*/
   jbposa(plon1,plat1,plon0,plat0,&dumd,&paou,&ipf);
   if(ipf == 0) 
   {
      *ierr = -1;
      paou = 0;
   }

   *paj = paou;

   return 0;
}

unjpa (ra,dec,paj,tobsin,ieflg,eqxbou,raout,decout,pab,ierr)
int ieflg, *ierr;
double ra,dec,paj,tobsin,eqxbou,*raout,*decout,*pab;
{

/*
*
*       ALL FLOATING POINT ARGS ARE DOUBLE PRECISION
*
* ra,dec is input position in decimal degrees at J2000.0
* paj  = input position angle (0 to 360.0d0 deg.) at J2000.0
* tobsin is year of observation (e.g. 1983.5 for iras sources)
*         if 0.0d0, then tobsin is treated as though equal to eqxin.
* ieflg  of -1 indicates to not add back E-terms
*           +1  (any val other than -1) indicates E-terms are to be replaced.
* eqxbou is the equinox(besselian) of the output position & position angle.
* raout,decout is output position in decimal degrees at eqxbou.
* pab is output position angle (0 to 360.0d0 deg.) for eqxbou;
*         if ierr = 0 ( garbage otherwise).
* ierr = 0 for normal return. -1 if paou could not be computed.
*
* NOTE:
* This routine assumes input pos angle & input position are in the equatorial
*    coordinate system.
*
* calls jbposa,jbelpt,unjul2
*
*/

   int iflg, ipf;
   double plonp0,platp0,plonp1,platp1, plon0, plat0, plon1, plat1,dumd;
   double dist = 0.1,pain,paou,eqx,tobs;

      *ierr = 0;
      eqx = eqxbou;
      if(eqxbou == 0.0) eqx = 1950.0;
      tobs = tobsin;
      if(tobsin == 0.0) tobs = eqx;

      plonp0 = ra;
      platp0 = dec;
      pain = paj;
      unjul2( ra, dec,tobs,ieflg,eqx,raout,decout);
      plon0 = *raout;
      plat0 = *decout;

/* find end point at equinox J2000.0 */

      jbelpt(plonp0,platp0,dist,pain,&plonp1,&platp1,&iflg);
      if(iflg == 3)
      {
/*    recompute plonp0,platp0 for pos angle processing if near a pole: */
         plonp0 = 0.0;
         if(platp0 < 0.0)
            platp0 = -90.0;
         else
            platp0 = 90.0;
         unjul2(plonp0,platp0,tobs,ieflg,eqx,&plon0,&plat0);
      }
      if(iflg == 4)
      {
         plonp1 = 0.0;
         if(platp1 < 0.0) 
            platp1 = -90.0;
          else
            platp1 = 90.0;
      }

       unjul2(plonp1,platp1,tobs,ieflg,eqx,&plon1,&plat1);

/* get new pos angle */

      jbposa(plon1,plat1,plon0,plat0,&dumd,&paou,&ipf);
      if(ipf == 0)
      {
         *ierr = -1;
         paou = 0;
      }
      *pab = paou;

      return 0;
}

gtjpap(eqxin,ra,dec,pab,pma,pmd,pin,vin,ieflg,
       raout,decout,paj,pmaout,pmdout,ierr)
int ieflg, *ierr;
double eqxin,ra,dec,pab,pma,pmd,pin,vin,
       *raout,*decout,*paj,*pmaout,*pmdout;
{
/*
*
*       ALL FLOATING POINT ARGS ARE DOUBLE PRECISION
*
* eqxin is the equinox(besselian) of the input position & position angle.
* ra,dec is input position in decimal degrees at equinox eqxin.
* pab  = input position angle (0 to 360.0d0 deg.) at eqxin.
* pma is proper motion in ra in seconds per tropical century.
* pmd is proper motion in dec in seconds of arc per tropical century.
* pin is parallax in arc seconds.
* vin is radial velocity in km/sec
* ieflg  of -1 indicate to not remove E-terms from position in computing
*             position angle (any other value allows E-term removal)
* raout,decout is output position in decimal degrees at J2000.0
* paj is output position angle (0 to 360.0d0 deg.) for J2000.0
*         if ierr = 0 ( garbage otherwise).
* pmaout is proper motion in seconds of time per Julian century.
* pmdout is proper motion in seconds of arc per Julian century.
* ierr = 0 for normal return. -1 if paj could not be computed.
*
* NOTE:
* This routine assumes input pos angle & input position are in the equatorial
*    coordinate system.
*
* calls jbposa,jbelpt,gtjulp,gtjul2
*
*/
      int    iflg,ipf;
      double plonp0,platp0,plonp1,platp1,plon0, plat0, plon1, plat1, dumd;
      double ra50,dec50,pma50,pmd50;
      double dist=0.1,pain,paou,eqx,tobs;

      *ierr = 0;
      eqx = eqxin; 
      if(eqxin == 0.0) eqx = 1950.0;
      tobs = eqx;

      plonp0 = ra;
      platp0 = dec;
      pain = pab;

/* get central position (w/ proper motions) at J2000.0: */

      if(eqx == 1950.0)
      {
         ra50  = ra;
         dec50 = dec;
         pma50 = pma;
         pmd50 = pmd;
      }
      else
         nwcprp(eqx,ra,dec,pma,pmd,pin,vin,1950.0,
                &ra50,&dec50,&pma50,&pmd50);

      gtjulp(1950.0,ra50,dec50,pma50,pmd50,pin,vin,ieflg,
                raout,decout,pmaout,pmdout);
/*
* find end point at equinox eqxin - ignore proper motion for pos.ang. calc.
*  note: plonp0 and platp0 are used for pos angle proc. and may
*        be slightly adjusted here. 
*
*/
      jbelpt(plonp0,platp0,dist,pain,&plonp1,&platp1,&iflg);
      if(iflg == 3)
      {
/*       recompute plonp0,platp0 for pos angle processing if near a pole: */
         plonp0 = 0.0;
         if(platp0 < 0.0) 
            platp0 = -90.0;
         else
            platp0 = 90.0;
      }

      gtjul2(eqx,plonp0,platp0,tobs,ieflg,&plon0,&plat0);

      if(iflg == 4)
      {
         plonp1 = 0.0;
         if(platp1 < 0.0)
            platp1 = -90.0;
         else
            platp1 = 90.0;
      }

/* precess pt at apex of ellipse: */
      gtjul2(eqx,plonp1,platp1,tobs,ieflg,&plon1,&plat1);
/*
* get new pos angle
*/
      jbposa(plon1,plat1,plon0,plat0,&dumd,&paou,&ipf);
      if(ipf == 0) 
      {
        *ierr = -1;
        paou = 0;
      }
      *paj = paou;

      return 0;
}

unjpap(ra,dec,paj,pma,pmd,pin,vin,ieflg,eqxbou,
       raout,decout,pab,pmaout,pmdout,ierr)
int ieflg,*ierr;
double ra,dec,paj,pma,pmd,pin,vin,eqxbou,
       *raout,*decout,*pab,*pmaout,*pmdout;
{
/*
*       ALL FLOATING POINT ARGS ARE DOUBLE PRECISION 
*
* ra,dec is input position in decimal degrees at J2000.0
* paj  = input position angle (0 to 360.0d0 deg.) at J2000.0
* pma is proper motion in ra in seconds per Julian century.
* pmd is proper motion in dec in seconds of arc per Julian century.
* pin is parallax in arc seconds.
* vin is radial velocity in km/sec
* ieflg  of -1 indicates to not add back E-terms
*           +1  (any val other than -1) indicates E-terms are to be replaced.
* eqxbou is the equinox(besselian) of the output position & position angle.
* raout,decout is output position in decimal degrees at eqxbou.
* pab is output position angle (0 to 360.0d0 deg.) for eqxbou;
*         if ierr = 0 ( garbage otherwise).
* pmaout is proper motion in seconds of time per tropical century.
* pmdout is proper motion in seconds of arc per tropical century.
* ierr = 0 for normal return. -1 if paou could not be computed.
*
* NOTE:
* This routine assumes input pos angle & input position are in the equatorial
*    coordinate system.
*
* calls jbposa,jbelpt,unjulp,unjul2
*
*/
      int iflg, ipf;
      double plonp0,platp0,plonp1,platp1, plon0, plat0, plon1, plat1, dumd;
      double ra50,dec50,pma50,pmd50;
      double dist=0.1,pain,paou,eqx,tobs;

      *ierr = 0;
      eqx = eqxbou;
      if(eqxbou == 0.0) eqx = 1950.0;
      tobs = eqx;

      plonp0 = ra;
      platp0 = dec;
      pain = paj;
      unjulp( ra, dec,pma,pmd,pin,vin,ieflg,eqx,&ra50,&dec50,&pma50,&pmd50);
      if(eqx == 1950.0)
      {
         *raout  = ra50;
         *decout = dec50;
         *pmaout = pma50;
         *pmdout = pmd50;
      }
      else
         nwcprp(1950.0,ra50,dec50,pma50,pmd50,pin,vin,eqx,
		raout,decout,pmaout,pmdout);
/*
* find end point at equinox J2000.0
*/
      jbelpt(plonp0,platp0,dist,pain,&plonp1,&platp1,&iflg);
      if(iflg == 3)
      {
/*       recompute plonp0,platp0 for pos angle processing if near a pole: */
         plonp0 = 0.0;
         if(platp0 < 0.0)
            platp0 = -90.0;
         else
            platp0 = 90.0;
      }
      unjul2(plonp0,platp0,tobs,ieflg,eqx,&plon0,&plat0);

      if(iflg == 4)
      {
         plonp1 = 0.0;
         if(platp1 < 0.0) 
            platp1 = -90.0;
         else
            platp1 = 90.0;
      }

      unjul2(plonp1,platp1,tobs,ieflg,eqx,&plon1,&plat1);

/* get new pos angle
*
*/
      jbposa(plon1,plat1,plon0,plat0,&dumd,&paou,&ipf);
      if(ipf == 0)
      {
        *ierr = -1;
        paou = 0;
      }
      *pab = paou;

      return 0;
}
