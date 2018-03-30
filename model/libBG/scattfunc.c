/* scattfunc.c                                                              */
/*    C versions of IDL scattfunc, sp_solar and phasefunc                   */
/* converted/modified from IDL version    j.bennett @ IPAC                  */
/* 10,11,23feb1999                                                          */
/* 12,13Apr1999 to install new function phasefunc version.                  */
/*   Also to remove all references to "det" and get_det,                    */
/* 03June1999 to make functions solar_sp and phasefunc local (i.e.static)   */
/* 18May2000  to return 0. for solar spectrum (in solar_sp) where lambda    */
/*            less than smallest value in w[] table.                        */

/* COMMENTS from IDL version:                                               */
/*
;--------------------------------------------------------------
; Function SCATTFUNC
;
; Returns the scattered light component of the zodiacal dust 
; source function.
;
; Written By: BA Franz, ARC, 11/95
;
; Inputs: 
;   det      - DIRBE detector number (0-9) or -1 if non-DIRBE
;   LOS      - Distance from Earth (AU)
;   R        - sqrt(x^2+y^2+z^2) (AU)
;   Re       - Distance from Earth to Sun (AU)
;   SolElong - Solar elongation (radians)
;   a        - Model parameters
;
; Outputs:
;   SCATTFUNC - Brightness per particle in MJy/sr
;   df        - Partial derivative of source function wrt each 
;               parameter
;
;--------------------------------------------------------------
;function scattfunc,det,LOS,R,Re,SolElong,a,df,wave_len
*/
#include <stdio.h>
#include <math.h>
#include "ztrig_s.h"

#define DEBUG 0

static double solar_sp();
static double phasefunc();

int scattfunc(los, R, Re, SolElong, a, numa, wave_len, Scatt, npts)
double los[];
double R[];
double Re;
double SolElong;
double a[];     /* only a space holder array for now */
int numa;       /* no. of elements in a              */
double wave_len;
double Scatt[];
int    npts;    /*no. of elements in los (=no. in R and = no. to be in Scatt)*/
{
  int i;
  double sinangle;
  double SolFlux;
  double ReXsinSol;
  double ReXcosSol;
  double PhaseAng;
  double itest;
  double ScatAng;
  double Phi;
  double CCscat;
  extern struct ztrig_s ztrig;

/*  ;
    ; Solar Flux at 1 AU from Sun (courtesy W. T. Reach)
    ;
*/
       /*     SolFlux1AU array commented out 12Apr1999             */
/*
* static double SolFlux1AU[10] = 
*                { 2.3405606e+08, 
*                  1.2309874e+08, 
*                  64292872.,     
*                  35733824.,     
*                  5763843.0,     
*                  1327989.4,     
*                  230553.73,     
*                  82999.336,     
*                  42346.605,     
*                  14409.608 };
*/

  SolFlux = solar_sp(wave_len);

  ReXsinSol = Re * sin(SolElong);
  ReXcosSol = Re * cos(SolElong);

  #if DEBUG
    fprintf(stdout,"SolFlux=%16.5e\n",SolFlux);
  #endif

           /*   Evaluate Phase Function  */

  for (i = 0; i < npts; i++)
  {
     /* note original IDL construct ( < is a minimum operator) was:         */
     /* PhaseAng = asin( Re/R*sin(SolElong) < 1.0 )                         */

    sinangle = ReXsinSol / R[i];
    if (sinangle > 1.0)
      sinangle = 1.0;
    else if (sinangle < -1.0)
      sinangle = -1.0;
    PhaseAng = asin(sinangle);
    itest      = 1.0 * (double) (los[i] >= ReXcosSol);
    ScatAng    = (1. - itest) * PhaseAng + (itest) * (ztrig.pi - PhaseAng);

    Phi        = phasefunc(ScatAng,wave_len); /* ScatAng in radians,   */
                                              /*  wave_len in microns. */
  #if DEBUG
    fprintf(stdout,"PhaseAng,itest,ScatAng,Phi=%f %f %f %15.9f\n",
    PhaseAng,itest,ScatAng,Phi);
  #endif

         /*  Total Scattered Light Contribution  */

    CCscat     = 1.0;                     /* CCscat seems to be space holder*/
    Scatt[i]   = SolFlux * CCscat * Phi;
  }
  return(0);
}
/* ------------------------------------------------------------------------ */
/*   COMMENTS from IDL version of solar_sp:                                 */
/*
;============================================================================
; Solar spectrum...............W.T.Reach 3/17/94
; usage:
; answer=solar_sp(wl)
; wl=input array of wavelengths in microns
; answer=solar spectrum flux density at wl, in MJy
;
;function solar_sp,wl
*/
static double solar_sp(wl)
double wl;
{
  static int nthru = 0;
  double work;
  double solar_spectrum;
  int i;

  static double w[43] =
                 {.20,.22,.24,.26,.28,.30,.32,.34, .36, .37,
                  .38, .39, .40, .41, .42,
                  .43, .44, .45, .46, .48,
                  .50, .55, .60, .65, .70,
                  .75, .80,.90, 1.0,1.1,
                  1.2,1.4,1.6 ,1.8 ,2.0,
                  2.5 ,3.0 ,4. ,5. ,6.,
                  8.  ,10. ,12.};

  static double f[43] =
                { 1.2,4.5,6.4,13.,25.,59.,85.,114.,115.,127.,  
                 121.,115.,160.,187.,189.,
                 183.,201.,213.,215.,213.,
                 204.,198.,187.,167.,149.,
                 129.,114.,90.,74.,61.,
                  50.,33.,22.3,14.8,10.2,
                 4.97,2.63,.93,.41,.21,
                .063,.023,.012};

  int interpol();   /*  in /home/iras649/clib/interpol.c */

  if (nthru == 0)
  {
    for (i = 0; i < 43; i++)
      f[i] = f[i] * (1.e23 * 1.e4 * 1.e-6 / 2.998e14) * (w[i] * w[i]); /*; MJy */
    nthru = 1;
  }

  if (wl >= 10.0)
  {
    work = w[42] / wl;
    solar_spectrum = f[42] * work * work;
  }
  else if (wl < w[0])
    solar_spectrum = 0.0;
  else
    interpol(w, f, 43, wl, &solar_spectrum);
  return(solar_spectrum);
}
 
/* ------------------------------------------------------------------------- */
/*   COMMENTS from IDL version of phasefunc:                                */
/*
;-------------------------------------------------------------------------------
; IDL Function PHASEFUNC.PRO
;
; H-G Phase Function for ZKERNEL
;
; Primary Author: B.A. Franz, ARC, (301) 513-7776
;
; Contributors:   W.T. Reach,
;
; Inputs:
;    theta - array of scattering angles (radians)
;
; Optional Inputs:
;
; Outputs:
;    phi   - array of phase function
;
; Optional Outputs:
;
;-------------------------------------------------------------------------------
*/
/*  C-version updated 12Apr1999, note: function pf incorporated within    */
/*     this function phasefunc (ie. pf not separate code).                */

static double phasefunc(theta, wave_len)
double theta;    /* phasefunc computes for one element of array at a time.*/
                 /* theta in units of radians.                            */
                 /* -does not handle whole array at a time like IDL does. */
double wave_len; /* in microns.                                           */
{
                          /* s1-3,g1-3 for Hong phase func computation.     */
  static double s1 = 0.665;
  static double s2 = 0.330;
  static double s3;

  static double g1 =  0.70;
  static double g2 = -0.20;
  static double g3 = -0.81;
                             /* c0,c1,c2 for Kelsall phase func computation. */
  static double c0[3] = {-0.942, -0.527, -0.431};
  static double c1[3] = { 0.121,  0.187,  0.172};
  static double c2[3] = {-0.165, -0.598, -0.633};

  static double g1g1, g2g2, g3g3;
  static double norm[3];
  static int nthru = 0;

  extern struct ztrig_s ztrig;
  int i;
  double costh;
  static double lambdas[4] = { 0.55, 1.25, 2.2, 3.5};

  double pfs[4]; /* pfs[0] -> pfv, pfs[1] -> pf1, pfs[2] -> pf2, pfs[3] -> pf3*/
                 /* pfv is Hong phase func, pf1-3 Kelsall phase func at       */
                 /*   wavelengths of 1.25, 2.2 and 3.5 microns.               */
  double ans;

  int interpol();

  if (nthru == 0)
  {
    s3 = 1.0 - s1 - s2;
    g1g1 = g1 * g1;
    g2g2 = g2 * g2;
    g3g3 = g3 * g3;

          /* for Kelsall phase functions at 1.25, 2.2 and 3.5 microns */
          /* compute (one time only) norms that are not func of theta */
    for (i = 0; i < 3; i++)
      norm[i] = 2.0 * ztrig.pi * ztrig.pi * c0[i] +
                      ztrig.pi * ztrig.pi * ztrig.pi *c1[i] +
                2.0 * ztrig.pi / c2[i] * (exp(c2[i] * ztrig.pi) - 1.0);
    #if DEBUG
      fprintf(stdout,"norm[0]=%16.12f\nnorm[1]=%16.12f\nnorm[2]=%16.12f\n",
        norm[0], norm[1], norm[2]);
    #endif

    nthru = 1;
  }

    /*  Hong phase function:  */
  if (wave_len < 3.5)
  {
   costh = cos(theta);
   pfs[0] =
   s1 / 4. / ztrig.pi * (1.- g1g1) / pow((1.0 + g1g1 - 2. * g1 * costh),1.5) +
   s2 / 4. / ztrig.pi * (1.- g2g2) / pow((1.0 + g2g2 - 2. * g2 * costh),1.5) +
   s3 / 4. / ztrig.pi * (1.- g3g3) / pow((1.0 + g3g3 - 2. * g3 * costh),1.5);
  }

/*  ; combine phase functions  as appropriate for wavelength  */

  if (wave_len <= 0.55)
    ans = pfs[0];
  else if (wave_len >= 3.5)
    ans = (c0[2] + c1[2] * theta + exp(c2[2] * theta)) / norm[2];
  else     
  {                                 /* wavelength > 0.55 and < 3.5 */
    for (i = 0; i < 3; i++)
      pfs[i+1] = (c0[i] + c1[i] * theta + exp(c2[i] * theta)) / norm[i];

    interpol(lambdas, pfs, 4, wave_len, &ans);
  }

  return(ans);
}
/* ------------------------------------------------------------------------ */
