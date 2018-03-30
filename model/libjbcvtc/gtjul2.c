#include <math.h>

extern int japply;

gtjul2(eqxin,ra,dec,tobsin,ieflg,raout,decout)
int ieflg;
double eqxin,ra,dec,tobsin,*raout,*decout;
{
/*
* 31jul92 - changes to jrmvet,jaddet; substituted more general purpose 
*     subs jgtobq,ecleqx,equec2 for limited- purpose subs gtoblq,ecleq2,equec2
* 23jul92 to call nwcprc/nwcprp instead of jprecj.
* 02jul92 - re; routines with propermotions:
*    gtjulp will assume eqx,epoch B1950 in and yield eqx,epoch J2000 out.
*    unjulp will assume eqx,epoch J2000 in and yield eqx,epoch B1950 out.
* 30jun92 to apply fk5-fk4 sys. corr. to eqx B1950 position before any other
*   corrections.
* 05,06mar92 to use astron. suppl 1984 & 1961 e-term removal equations
*    (in subs gtjul2,gtjulp,unjul,unjulp)
* 13-15jan92 updated to reflect method described in FK5 Part I. Catalog
* author:   judy bennett
*
* to precess old besselian equinox positions to equator and equinox of 
*   J2000.0.
*
*   Calls subroutines nwcprc ("old" Newcomb constants) and fk5prc and fk5prp
*      ("new" Julian prcession formulas).
*
*  Source of method:
*    Fifth Fundamental Catalogue (FK5) Part I (1988, but not avail. till 1992)
*     Section 8. (page 9)  
*     Summary of order:
*  1.  Precess each object (using precessional values adopted in that cat-
*        i.e. usually Newcomb) from the catalogue equinox to the mean equinox
*        at the object's mean epoch (i.e. observation date).
*  2.  The mean positions in all star catalogues published prior to 1984
*        contain the terms of elliptic aberration (E-terms). These terms need
*        to be removed from the star's mean position.
*  3.  The equinox correction and the systematic correction FK5-FK4 
*       (given in the FK5 cat), computed for the object's mean epoch, need to
*       be applied. Note: program applies fk5-fk4 sys.corr. at B1950 which
*       seems consistent with procedure actually described in the FK5.
*  4.  The corrected position has to be precessed to the new standard equinox
*       J2000.0 using the precessional quantities as adopted in the IAU(1976)
*       System of Astronomical Constants (also see Astronomical Almanac 1984,
*       pages S34,S36)
*
*  NOTE: these routines apply the fk5-fk4 correction at B1950.
*        if no fk5-fk4 systematic correction required, the user should
*        include the following statements in his program:
*  (a value of 0 for japply specifies no corrections; anything else uses corr.)
*        external jfk5bd
*        common /fkappl/ japply
*        japply = 0
*
* ****************************************************************************
*   All arguments are double precision, except ieflg which is integer.
*  Input values:
* eqxin is the equinox of the input position (if negative, absolute val.used)
* ra    is right ascension (decimal degrees) at eqx
* dec   is declination (decimal degrees) at eqx
* tobsin is the year of observation (i.e. when the object was observed at 
*         this position (e.g. 1983.5d0) ; if 0.0d0, value of eqxin is used.
* ieflg is flag allowing removal of E-terms of aberration if any (usually
*        they are present, therefore ieflg should = +1):
*         ieflg = -1 do not remove E-terms (there none).
*         ieflg = any value except -1 indicates  E-terms are present and
*                 are to be removed.
*  Returned values:
* raout  is right ascension at equator and equinox of J2000.0
* decout is declination at equator and equinox of J2000.0
*
* ****************************************************************************
*
* Files required: gtjul2.f (includes gtjul2,gtjulp,unjul2,unjulp,gtecle,
*                           gtetrm,itere,iterec,jaddet,jclpos,jrmvet)
*                 ecleqx.f (includes ecleqx,equecx,jgtobq)
*                 fk5prc.f (includes fk5prc,fk5prp)
*                 nwcprc.f (includes nwcprc,nwcprp)
*                 jgtfkc.f (includes jgtfkc,junfkc,jnitfk.jfk5bd,dintrp,dintr2)
*
* ****************************************************************************
*/
      double tobs, tobsj, jde, eqx, rat50, dect50;
      double rat, dect, delt, dela;
      double corra, corrd, corrpa, corrpd;

      eqx  = fabs(eqxin);
      tobs = fabs(tobsin);
      if(tobs == 0.0) tobs = eqx;

      if(japply == 0) 
      {
         rat50 = ra;
         dect50 = dec;
      }
      else
      {
/*   determine fk5-fk4 systematic correction using eqx B1950 postion */

         if(eqx != 1950.0)
             nwcprc(eqx,ra,dec,1950.0,&rat50,&dect50);
         else
         {
            rat50 = ra;
            dect50 = dec;
         }
         jgtfkc(rat50,dect50,0.0,tobs,&corra,&corrd,&corrpa,&corrpd);
         rat50 = rat50 + corra;
         dect50 = dect50 + corrd;
         jclpos(&rat50,&dect50);
         eqx = 1950.0;
      }

      if (tobs != eqx)
/*                     use old newcomb formula to prec. to tobs */
          nwcprc( eqx, rat50, dect50, tobs, &rat, &dect);
      else
      {
          rat  = rat50;
          dect = dect50;
      }
/*
* compute the correction for right ascension at the mean epoch of observations.
*   note: this correction will generally be of order +0.06s for most modern 
*         catalogs.
* delt is fraction of Julian century; adjust tobs in besselian time frame
*   to be correct
*   in terms of Julian. use jde of b1950 and tropical year in days
*   to get Julian day of observations. Then compute equivalent year for 
*   Julian prec. prog. so fraction of Julian year will be correct.
*   B1950 = JDE 2433282.4235  365.2421988 tropical days per year.
*   J2000 = JDE 2451545.00    365.25 Julian days per year. 
*
*/
      jde = ((tobs - 1950.0) * 365.2421988) + 2433282.4235;     
      tobsj = 2000.0 + ((jde - 2451545.0)/365.25);
/*
* Remove E-terms if necessary:
*/
      if(ieflg != -1) jrmvet(tobsj,&rat,&dect);
/*
* Apply the equinox correction (use tobs not tobsj):
*
*/
      delt = (tobs - 1950.0) * 0.01;
      dela = ((0.035 + 0.085*delt) * 15.0) / 3600.0;

      rat = rat + dela;
      jclpos(&rat,&dect);
/*
* now compute mean place of source at 2000 Jan. 1.5 using the mean place 
* at the mean epoch of observations tobs (with dela applied) and the
* new precession formulas (i.e. Julian prec. formulas)
*
*/
      fk5prc( tobsj, rat, dect, 2000.0, raout, decout);

}

unjul2(rain,decin,tobsin,ieflg,eqxbou,raout,decout)
int ieflg;
double rain,decin,tobsin,eqxbou,*raout,*decout;
{
/*
* author: judy bennett
* 01jul92
* 26jun92 to allow removal of fk5-fk4 systematic corrections
* 17jan92
* 05feb92 to use fk5prc instead of hcprec.
*
* unjul2 reverses what gtjul2  did - i.e. unjul2 precesses accurately
*    back to the original besselian input.
*
* to precess new Julian  equinox positions (J2000.0)back to input besselian
*   equinox positions (B1950.0).
*
*   Calls subroutines nwcprc ("old" Newcomb constants) and fk5prc ("new"
*      Julian prec.formulas).
*
*  Source of method:
*   See comments in gtjul2.
*
* ****************************************************************************
*   All arguments are double precision, except ieflg which is integer.
*  Input values:
* rain is right ascension (decimal degrees) at J2000.0
* decin is declination (decimal degrees) at J2000.0
* tobsin is the year of observation (i.e. when the object was assigned this pos)
*         if 0.0d0, eqxbou is used.
* ieflg is flag allowing restore of E-terms of aberration if any (usually
*        they are present; ieflg should usually = + 1).
*         ieflg =-1 do not replace E-terms (there none).
*         ieflg = anything but -1 indicates  E-terms are to be present
*             and are to be replaced. Usually they are to be replaced.
* eqxbou is output equinox (besselian): if 0.0d0, 1950.0d0 is used.
*  Returned values:
* raout  is right ascension at besselian equinox of eqxbou.
* decout is declination at besselian equinox of eqxbou.
*
* ****************************************************************************
*/

      double ra, dec;
      double tobsj, jde, tobsb;
      double rat, dect, delt, dela;
      double eqx1=2000.0,eqx2,rat50,dect50;
      double corra, corrd, corrpa, corrpd;

      if(eqxbou != 0.0) 
         eqx2 = fabs(eqxbou);
      else
         eqx2 = 1950.0;

      if(tobsin != 0.0) 
         tobsb = fabs(tobsin);
      else
         tobsb = eqx2;
      ra  = rain;
      dec = decin;
/*
* tobsj is year in terms of Julian years; tobsb is year in terms of tropical.
*
*/
       jde = ((tobsb - 1950.0) * 365.2421988) + 2433282.4235;     
       tobsj = 2000.0 + ((jde - 2451545.0)/365.25);

       fk5prc( eqx1, ra, dec, tobsj, &rat, &dect);
/*
* remove the equinox correction (use tobsb not tobsj):
*
*/
      delt = (tobsb - 1950.0) * 0.01;
      dela = ((0.035 + 0.085*delt) * 15.0) / 3600.0;

       rat = rat - dela;
       if(rat >= 360.0) rat = rat - 360.0;
       if(rat <   0.0)  rat = rat + 360.0;
/*
* Add back  E-terms if necessary:
*/
      if(ieflg != -1)  jaddet(tobsj,&rat,&dect);

      if(japply == 0) 
      {
         if(tobsb != eqx2) 
            nwcprc( tobsb,rat, dect, eqx2, raout, decout);
         else
	 {
            *raout = rat;
            *decout = dect;
	 }     
      }
      else
      {
/*                find and remove fk5-fk4 systematic corrections: */
          if(tobsb == 1950.0) 
	  {
             rat50 = rat;
             dect50 = dect;
	  }
          else
             nwcprc(tobsb,rat,dect,1950.0,&rat50,&dect50);

          junfkc(rat50,dect50,0.0,tobsb,&corra,&corrd,&corrpa,&corrpd);
          rat50 = rat50 - corra;
          dect50 = dect50 - corrd;
          jclpos(&rat50,&dect50);
          if(eqx2 != 1950.0) 
              nwcprc(1950.0,rat50,dect50,eqx2,raout,decout);
          else
	  {
             *raout = rat50;
             *decout = dect50;
	  }
      }

      return 0;
}

gtjulp(eqxin,rain,decin,pmain,pmdin,pin,vin,ieflg,
       raout,decout,pmaout,pmdout)
int ieflg;
double eqxin,rain,decin,pmain,pmdin,pin,vin,
       *raout,*decout,*pmaout,*pmdout;
{
/*
* 27jul92 to allow pmain,pmdin =0.0d0 to be processed as any other
*         source with given proper motions (User must use gtjul2
*         if proper motions are "intrinsically" zero (e.g. radio source)
*         or if no proper motions are given for the object).
* 02jul92 to apply fk5-fk4 systematic corrections at B1950.
*  eqxin will be ignored; jgtjulp assumes equinox,epoch B1950 for inputs.
* 24jun92 to apply fk5-fk4 systematic corrections if desired.
* 05feb92 like gtjul2, except for objects with proper motions etc.
*
* author: judy bennett
*
* to precess old besselian equinox positions to equator, equinox and epoch
*   of J2000.0.
*
*  Source of method:
*    Fifth Fundamental Catalogue (FK5) Part I (1988, but not avail. till 1992)
*     Section 8. (page 9) + discussion in FK5
*
* ****************************************************************************
*   All arguments are double precision, except ieflg which is integer.
*  Input values:
* eqxin is ignored (assumes 1950.0d0);
*       is the equinox & epoch of input position 
* rain  is right ascension (decimal degrees) at equinox & epoch of B1950 
* decin is declination (decimal degrees) at equinox & epoxh of B1950
* pmain is proper motion in ra in seconds of time per tropical century
* pmdin is proper motion in dec in seconds of arc per tropical century
* pin is parallax in arc seconds.
* vin is radial velocity in km/sec
* ieflg is flag allowing removal of E-terms of aberration if any (usually
*        E-terms are present; therefore, ieflg should = +1)
*         ieflg = -1 do not remove E-terms (there none).
*         ieflg = anything except -1 indicates  E-terms are present and
*                 are to be removed.
*  Returned values:
* raout  is right ascension at epoch, equator and equinox of J2000.0
* decout is declination at epoch, equator and equinox of J2000.0
* pmaout is proper motion in seconds of time per Julian century.
* pmdout is proper motion in seconds of arc per Julian century.
*
* ****************************************************************************
*
*/

      double pma, pmd, corra, corrd, corrpa, corrpd;
      double tobs=1950.0, tobsj, jde, eqx = 1950.0;
      double rar,decr, work;
      double rat, dect, delt, dela;
      double pmat, pmdt, dtor;

      dtor =  atan(1.0) / 45.0;

      if(japply == 0)
      {
         rat = rain;
         dect = decin;
         pma = pmain;
         pmd = pmdin;
      }
      else
/*          determine fk5-fk4 correction & apply */
      {
         rat = rain;
         dect = decin;
         jgtfkc(rat,dect,0.0,tobs,&corra,&corrd,&corrpa,&corrpd);
         rat = rat + corra;
         dect = dect + corrd;
         jclpos(&rat,&dect);
         pma = pmain + corrpa;
         pmd = pmdin + corrpd;
      }
/*
* compute the correction for right ascension at the mean epoch of observations.
*   note: this correction will generally be of order +0.06s for most modern 
*         catalogs.
* delt is fraction of Julian century; adjust tobs in besselian time frame
*   to be correct
*   in terms of Julian. use jde of b1950 and tropical year in days
*   to get Julian day of observations. Then compute equivalent year for 
*   Julian prec. prog. so fraction of Julian year will be correct.
*   B1950 = JDE 2433282.4235  365.2421988 tropical days per year.
*   J2000 = JDE 2451545.00    365.25 Julian days per year. 
*
*/
      jde = ((tobs - 1950.0) * 365.2421988) + 2433282.4235;    
      tobsj = 2000.0 + ((jde - 2451545.0)/365.25);
/*
*
*  Remove E-terms if necessary:
*/
      if(ieflg != -1) jrmvet(tobsj,&rat,&dect);
/*
* Apply the equinox correction (use tobs not tobsj):
*      equinox correction from fk5 cat (Section 3 (page 6).
*/
      delt = (tobs - 1950.0) * 0.01;
      dela = ((0.035 + 0.085*delt) * 15.0) / 3600.0;

      rat = rat + dela;
      jclpos(&rat,&dect);
/*
* convert proper motions from units per tropical century to units per Julian
*   century per p.S35 of Supp. to Astron.Alman.
* apply time-dependent portion of eqx corr to ra proper motion (0.0850 secs of
*   time per Julian century) per FK5 Catalog (Section 3 (page 6)).
* also adjust for change in precession constant per p. S35 of Supp. to
*   Astron. Alman. (note; misprint in Supp.: 0.6912 should be 0.06912)
*
*/
       rar = rat*dtor;
       decr = dect*dtor;
       if(fabs(dect) > 89.9999) 
          work = 0.0;
       else
          work = 0.0291*sin(rar)*tan(decr); 
       pmat = (pma * 1.00002136) - 0.06912 - work + 0.0850;
       pmdt = (pmd * 1.00002136) - 0.436*cos(rar);
/*
* now compute mean place of source at 2000 Jan. 1.5 using the mean place 
* at at the mean epoch of observations tobs (with dela applied) and the
* new precession formulas (i.e. Julian prec. formulas)
*
*
*/
      fk5prp( tobsj, rat, dect, pmat, pmdt, pin, vin,
                  2000.0, raout, decout, pmaout, pmdout);

      return 0;
}

unjulp(rain,decin,pmain,pmdin,pin,vin,ieflg,
       eqxbou,raout,decout,pmaout,pmdout)
int ieflg;
double rain,decin,pmain,pmdin,pin,vin,
       eqxbou,*raout,*decout,*pmaout,*pmdout;
{
/*
* 27jul92 to allow pmain,pmdin =0.0d0 to be processed as any other
*         source with given proper motions (User must use unjul2
*         if proper motions are "intrinsically" zero (e.g. radio source)
*         or if no proper motions are given for the object).
* 02jul92-eqxbou is ignored (assumed to be equinox,epoch B1950.0d0 for outputs)
* 26jun92 to allow removal for fk5-fk4 correction.
* author:  judy bennett
*
* unjulp reverses what gtjulp  did - i.e. unjulp precesses accurately
*    back to the original besselian input ( B1950).
*
* to precess new Julian  equinox positions (J2000.0)back to input besselian
*   equinox positions (B1950.0).
*
*  Source of method:
*   See comments in gtjul2.
*
* ****************************************************************************
*   All arguments are double precision, except ieflg which is integer.
*  Input values:
* rain is right ascension (decimal degrees) at epoch & equinox of J2000.0
* decin is declination (decimal degrees) at epoch & equinox of J2000.0
* pmain is proper motion in ra in seconds of time per Julian century.
* pmdin is proper motion in dec in seconds of arc per Julian century.
* pin is parallax in arc seconds.   (0.0d0 if unknown)
* vin is radial velocity in km/sec. (0.0d0 if unknown)
* ieflg is flag allowing restore of E-terms of aberration. Usually ieflg
*   should = +1 (to restore the E-terms to the B1950 position).
*         ieflg =-1 do not replace E-terms (there none).
*         ieflg = anything but -1 indicates  E-terms are to be present
*             and are to be replaced.
* eqxbou is ignored (assumed to be 1950.0d0);
*        is output equinox (besselian):
*  Returned values:
* raout  is right ascension (decimal degrees) at equinox,epoch B1950.
* decout is declination (decimal degrees) at equinox,epoch B1950.
* pmaout  is proper motion in ra in seconds of time per tropical century.
* pmdout  is proper motion in dec in seconds of arc per tropical century.
*
* ****************************************************************************
*
*/

      double ra, dec, tobsb=1950.0;
      double rat, dect, delt, dela, pmat, pmdt, rar, decr;
      double eqx1 = 2000.0, eqx2 = 1950.0, dtor;
      double corra, corrd, corrpa, corrpd;
      double jde, tobsj, work;

      dtor = atan(1.0)/45.0;

      ra = rain;
      dec = decin;

       jde = ((tobsb - 1950.0) * 365.2421988) + 2433282.4235;     
       tobsj = 2000.0 + ((jde - 2451545.0)/365.25);

       fk5prp( eqx1, ra, dec, pmain, pmdin, pin, vin,
               tobsj, &rat, &dect, &pmat, &pmdt);
/*
* re: proper motions:  remove adjustment for precession constant;
*     remove equinox corr. for proper motion in ra;
*     convert from units per Julian centry to units per tropical century:
*/
       rar  = dtor*rat;
       decr = dtor*dect;
       if(fabs(dect) > 89.9999) 
          work = 0.0;
       else
          work = 0.0291*sin(rar)*tan(decr);
       pmat = pmat + 0.06912 + work - 0.0850;
       *pmaout = pmat / 1.00002136;
       pmdt = pmdt + 0.436 * cos(rar);
       *pmdout = pmdt / 1.00002136;
/*
* remove the equinox correction (use tobsb not tobsj):
*
*/
      delt = (tobsb - 1950.0) * 0.01;
      dela = ((0.035 + 0.085*delt) * 15.0) / 3600.0;

      rat = rat - dela;
      if(rat >= 360.0)
         rat = rat - 360.0;
      else if(rat < 0.0) 
         rat = rat + 360.0;
/*
* Add back  E-terms if necessary:
*/
      if(ieflg != -1) jaddet(tobsj,&rat,&dect);

      if(japply != 0)
      {
/*           remove the fk5-fk4 systematic corrections: */
          junfkc(rat,dect,0.0,tobsb,&corra,&corrd,&corrpa,&corrpd);
          rat = rat - corra;
          dect = dect - corrd;
          *pmaout = *pmaout - corrpa;
          *pmdout = *pmdout - corrpd;
          jclpos(&rat,&dect);
      }

      *raout = rat;
      *decout = dect;

      return 0;
}

itere(ra,dec,edela,edeld)
double ra, dec, *edela, *edeld;
{
/* for adding E-term back (note: w/ supp. formulas, edela,edeld are added to
*   ra and dec to remove the E-term; therefore subtract here.
*/
      int i,iend1;
      double rwork,dwork;
      rwork = ra;
      dwork = dec;
      iend1 = 3;

      for (i=0; i<iend1;i++)
      {
          gtetrm(rwork,dwork,edela,edeld);
	  if (i == 2) return 0;
          rwork = ra    - *edela;
          dwork = dec   - *edeld;
          jclpos(&rwork,&dwork);
      }
      return 0;
}

gtetrm(ra,dec,dela,deld)
double ra, dec, *dela, *deld;
{
/*
*  from Suppl. to Astron. Alman. 1984 (also 1961 supp. page 144)
*    see also, Standish, A&A 115, 20-22 (1982)
*
* compute E-terms to be removed for  object at ra and dec.
*  all args. double precision and in decimal degrees)
*
*  Since the E-terms (terms of elliptic aberration) change so slowly
*    (Smart,Textbook on Spherical Astronomy, Sixth Ed. Section 108,p186)
*     these values do not require t as input and will be valid in the
*     1950 to 2000 time span we are dealing with.
*
*  The 1961 supp called these equations an approximation and stated that
*  small errors in this procedure are usually negligible. However, they
*  did not explain what lead up to the procedure:  "The form of the equations
*  of condition and their solution are not discussed here."
*
*/
      static int  nthrue=0;
      static double e1, e2, e3, e4, dtor;

      double dcosd, alplus; 

      if(nthrue == 0)
      {
         dtor = atan(1.0) / 45.0;
/* note:      e1 = (0.0227 * 15.0) / 3600.0 = 0.341/3600 = e3 */
         e2 = 11.25 * 15.0;
         e3 = 0.341 / 3600.0;
         e4 = 0.029 / 3600.0;
         e1 = e3;
         nthrue = 1;
      }

      alplus = ra    + e2;
      if(alplus >= 360.0) alplus = alplus - 360.0;
      alplus = alplus * dtor;
 
      dcosd = cos(dtor * dec);
      if(fabs(dec) >= 90.0 || fabs(dcosd) < 1.0e0-27)
      {
         *dela = 0.0;
         *deld = 0.0;
      }
      else
         *dela = (e1 * sin(alplus)) / dcosd;

      *deld = (e3 * cos(alplus) * sin(dec*dtor)) + (e4 * dcosd);

      return 0;
}

gtecle(epoch, lambda, beta, dela, deld)
double epoch, lambda, beta, *dela, *deld;
{
/*
*
* compute E-terms at epoch for ecliptic lambda, beta input (returned in dela,
*  deld (degrees).
*  epoch in years (e.g. 1950.000), lambda,beta,,dela,deld in decimal degrees.
*  All arguments double precision.
*
* E-term formulas from ASTRONOMICAL ALGORITHMS by Jean Meeus (1991) ch.22
*   Note: equations as presented are for computing E-terms from position
*         that does not contain E-terms.  To get better answer (when
*         splitting hairs), iterate to get best E-term (for position that
*         has E-terms) to be removed. Subroutine iterec may be called to
*         to do the iteration.
*   Note 2: these formulas for E-terms, as function of ecliptic lon and lat,
*           also appear in  Spherical Astronomy by R.Green (1985),page 192;
*           and in Textbook on Spherical Astronomy, Sixth Ed.,by Smart (1977),
*           page 186.
*
*   To remove E-terms, subtract dela & deld from lamba & beta, respectively,
*   in the calling program.
*   To add back E-terms, add dela & deld to lambda & beta, respectively, in
*   the calling program.
*   
*
*/
      static int nthrue = 0;
      static double dtor, kappa, lepoch=-1.0, e, pirad;

      double t, t2, pi, lrad, brad;

      if(nthrue == 0) 
      {
         dtor = atan(1.0) / 45.0;
/*    constant of aberration, kappa = 20.49552" "new" ("old" was 20.496) */
         kappa = 0.0056932;
         nthrue = 1;
      }

      *dela = 0.0;
      *deld = 0.0;

      if(epoch != lepoch) 
      {
         t = (epoch - 2000.0) * 0.01;
         t2 = t*t;
         lepoch = epoch;
/*
* e = eccentricity of the Earth's orbit
* pi= longitude of the perihelion of this orbit
*/
         e = 0.016708617 - 0.000042037*t - 0.0000001236*t2;
         pi= 102.93735 + 0.71953*t + 0.00046*t2;
         pirad = dtor * pi;
      }

      if(fabs(beta) > 89.999) return 0;
      lrad = dtor*lambda;
      brad = dtor*beta;

      *dela = e * kappa * cos(pirad-lrad) / cos(brad);
      *deld = e * kappa * sin(pirad-lrad) * sin(brad);

      return 0;
}

iterec(tobsj,lambda,beta,edela,edeld)
double tobsj,lambda,beta,*edela,*edeld;
{
      int i, iend1 = 3;
      double lwork,bwork;
      lwork = lambda;
      bwork = beta;
      for (i=0; i<iend1; i++)
      {
         gtecle(tobsj,lwork,bwork,edela,edeld);
         lwork = lambda - *edela;
         bwork = beta - *edeld;
         jclpos(&lwork,&bwork);
      }
      return 0;
}
jrmvet(tobsj,rat,dect)
double tobsj,*rat,*dect;
{
/*c Remove E-terms:
*  31jul92 update to use equecx and ecleqx instead of equec2 and ecleq2.
*/
      double edela,edeld;
      double lambda, beta, pole=89.999;

      if(fabs(*dect) < pole)
      {
         gtetrm( *rat, *dect, &edela, &edeld);
         *rat = *rat + edela;
         *dect = *dect + edeld;
         jclpos(rat, dect);
      }
      else
      {
/*    note: using "Julian system" (iaus=2 for equecx and ecleqx) here -
*     makes no. dif in resulting E-terms and simplifies argument
*     list required for jrmvet.
*/
         equecx(2,tobsj,*rat,*dect,&lambda,&beta);
         iterec(tobsj,lambda,beta,&edela,&edeld);
         lambda = lambda - edela;
         beta = beta - edeld;
         jclpos(&lambda, &beta);
         ecleqx(2,tobsj,lambda,beta,rat,dect);
      } 

      return 0;
}

jaddet(tobsj,rat,dect)
double tobsj,*rat,*dect;
{
/* Add back  E-terms:
*  31jul92 update to use equecx and ecleqx instead of equec2 and ecleq2.
*/
      double edela, edeld, lambda, beta, pole = 89.999;

      if(fabs(*dect) < pole) 
      {
         itere( *rat, *dect, &edela, &edeld);
         *rat = *rat - edela;
         *dect = *dect - edeld;
         jclpos(rat,dect);
      }
      else
      {
         equecx(2,tobsj, *rat, *dect, &lambda, &beta);
         gtecle(tobsj, lambda, beta, &edela, &edeld);
         lambda = lambda + edela;
         beta = beta + edeld;
         jclpos(&lambda, &beta);
         ecleqx(2,tobsj, lambda, beta, rat, dect);
      }

      return 0;
}

jclpos(rat,dect)
double *rat, *dect;
{
/* to put ra into 0 to 360 and dec into -90 to +90 ranges.*/

       if(*rat > 360.0) 
          *rat = *rat - 360.0;
       else if(*rat < 0.0)
          *rat = *rat + 360.0;

       if(fabs(*dect) > 90.0)
       {
          *rat = *rat + 180.0;
          if(*rat >= 360.0) *rat = *rat - 360.0;
          if(*dect >   0.0)
             *dect =   180.0 - *dect;
          else
             *dect = -(180.0 + *dect);
      }
      return 0;
}
