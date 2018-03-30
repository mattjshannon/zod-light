/* modspec.c                                                                */
/* C versions of IDL ism_spec_mod and cib_spec_mod (that were in modspec.pro)*/
/* converted from IDL version  j.bennett @ IPAC                             */
/*  27,29Apr1999                                                            */


/* COMMENTS from IDL version of ism_spec_mod:                               */
/*
;---------------------------------------------------------------
;function ism_spec_mod,wl
; returns ism normalized to 100 microns QUOTED DIRBE intensity
; reference: Arendt et al for 3.5-240
;            Boulanger et al for longer wavelength
;---------------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>

#define NOPH  147
#define NARR  10

int ism_spec_mod(wl,ans)
double wl;
double *ans;
{
/* soph and woph from /proj/ssc/Data/IRAC/bg/roph.sav on 26Apr1999 (via idl) */

    static double soph[NOPH] = {
   1.51405,   1.59453,   1.79279,   2.81383,   2.97917,   2.00780,
   2.41021,   2.59709,   3.10789,   4.54187,   5.62538,   5.65917,
   5.72497,   5.45159,   5.66551,   6.70442,   9.55741,  13.92131,
  22.29956,  31.02179,  32.29724,  29.31002,  20.44099,  16.15559,
  14.30209,  12.60695,  12.73688,  12.82516,  13.31335,  13.64733,
  14.11172,  15.40356,  15.63435,  16.42760,  16.73938,  17.07488,
  18.96120,  20.62828,  24.27766,  26.85144,  30.75578,  35.92010,
  43.55998,  48.39736,  50.72347,  50.71996,  49.27995,  47.04572,
  44.32765,  40.96308,  36.41304,  33.06361,  30.27491,  28.16143,
  26.69187,  26.27238,  25.99155,  25.66584,  26.10001,  27.62916,
  28.26149,  29.49283,  29.90168,  29.13898,  27.50326,  23.94060,
  21.11277,  18.10856,  16.61812,  15.59536,  14.20699,  14.03037,
  13.84261,  13.41645,  13.02332,  12.20279,  11.85019,  12.23137,
  13.05260,  13.35732,  13.95603,  13.37537,  13.18743,  12.97286,
  12.58204,  13.94350,  14.73366,  15.40425,  16.47586,  17.44332,
  20.38802,  27.80027,  36.17069,  48.83816,  49.11320,  43.51113,
  34.01853,  26.46252,  24.36779,  24.69203,  24.96208,  25.89799,
  25.84772,  27.75343,  28.72910,  28.83103,  29.93831,  29.76105,
  28.69962,  26.56755,  22.08438,  21.90445,  20.14079,  19.25631,
  19.52709,  19.56260,  19.32651,  18.23704,  17.31908,  16.48603,
  16.79447,  16.98239,  16.56844,  16.16768,  16.02761,  15.22225,
  14.81068,  14.07148,  14.18270,  13.65633,  13.14265,  13.48339,
  12.55868,  12.86635,  12.40685,  12.06837,  12.36575,  13.03779,
  14.68119,  15.11083,  14.49409,  14.66620,  15.81190,  17.28998,
  19.42031,  20.92316,  23.27691};

    static double woph[NOPH] = {
   5.07900,   5.14100,   5.20200,   5.26400,   5.32500,   5.38500,
   5.44600,   5.50700,   5.56700,   5.62700,   5.68700,   5.74700,
   5.80700,   5.86700,   5.92600,   5.98500,   6.04400,   6.10300,
   6.16200,   6.22100,   6.27900,   6.33700,   6.39600,   6.45400,
   6.51100,   6.56900,   6.62600,   6.68400,   6.74100,   6.79800,
   6.85500,   6.91100,   6.96800,   7.02400,   7.08000,   7.13600,
   7.19200,   7.24800,   7.30300,   7.35800,   7.41400,   7.46900,
   7.52400,   7.57800,   7.63300,   7.68700,   7.74100,   7.79500,
   7.84900,   7.90300,   7.95600,   8.01000,   8.06300,   8.11600,
   8.16900,   8.22200,   8.27400,   8.32700,   8.37900,   8.43100,
   8.48300,   8.53400,   8.58600,   8.63700,   8.68900,   8.74000,
   8.79100,   8.84200,   8.89200,   8.94300,   8.99300,   9.04300,
   9.09300,   9.11300,   9.22300,   9.33300,   9.44200,   9.55100,
   9.66000,   9.76900,   9.87800,   9.98600,  10.09000,  10.20000,
  10.31000,  10.42000,  10.52000,  10.63000,  10.74000,  10.84000,
  10.95000,  11.06000,  11.16000,  11.27000,  11.37000,  11.48000,
  11.58000,  11.69000,  11.79000,  11.89000,  12.00000,  12.10000,
  12.21000,  12.31000,  12.41000,  12.51000,  12.62000,  12.72000,
  12.82000,  12.92000,  13.02000,  13.12000,  13.23000,  13.33000,
  13.43000,  13.53000,  13.63000,  13.73000,  13.83000,  13.93000,
  14.02000,  14.12000,  14.22000,  14.32000,  14.42000,  14.52000,
  14.61000,  14.71000,  14.81000,  14.91000,  15.00000,  15.10000,
  15.19000,  15.29000,  15.39000,  15.48000,  15.58000,  15.67000,
  15.77000,  15.86000,  15.96000,  16.05000,  16.14000,  16.24000,
  16.33000,  16.42000,  16.52000};

  static double wldirb[NARR] =
       {1.25, 2.2,  3.5,    4.9,    12,    25,    60,  100,140,  240};
  static double arendt[NARR] =
       {0.001,0.001,0.00183,0.00291,0.0462,0.0480,0.171,1.,1.696,1.297};
  static double ccdirb[NARR] =
       {1.0,  1.0,  1.0,    1.0,    1.02,  1.23,  0.91, .92,.94, 0.99};

  int i;
  static double sgpart[NARR];
  static double log10_sgp[NARR+2];
  static double log_wp[NARR+2];
  double logwl;
  double power;
  double work;
  double sgint;
  double sg;
  double sp3;

  static int nthru = 0;
  static double lastwl = -99.0;
  static double lastans;
 
  int interpol();
  double fiteval();

  if (nthru == 0)
  {
    nthru = 1;
    for (i = 0; i < NARR; i++)
    {
      sgpart[i] = 
      arendt[i] / ccdirb[i] - fiteval((1.0e4 / wldirb[i]), 2.0, 18.0, 6.5e-6);
      if (sgpart[i] < 1.0e-08)
        sgpart[i] = 1.0e-08;
    } 
     /*  ; fix 12 micron to match edge of ISOCAM   */

    sgpart[4] = soph[135] / 24.96 * arendt[4] / ccdirb[4];

             /* idl:  sgp=[1.e-8,sgpart,1.e-8] & wp=[0.01,wldirb,1.e4] */

    log10_sgp[0]      = log10(1.0e-08);
    log10_sgp[NARR+1] = log10_sgp[0];
    for (i = 0; i < NARR; i++)
      log10_sgp[i+1] = log10(sgpart[i]);

    log_wp[0]      = log(0.01);
    log_wp[NARR+1] = log(1.0e+04);
    for (i = 0; i < NARR; i++)
      log_wp[i+1] = log(wldirb[i]);
  }
  if (wl == lastwl)
  {
    *ans = lastans;
    return(0);
  }
        /* idl:  sgint=10.^interpol(alog10(sgp),alog(wp),alog(wl)) */
  logwl = log(wl);
  interpol(log_wp, log10_sgp, NARR+2, logwl, &power);
  sgint = pow(10.0, power);

      /* idl: l=where(wl gt 5.1 and wl lt 15.48,count)                      */
      /*; include ISOCAM spectrum, scaled to match in-filter 12-micron DIRBE*/
      /*  if count gt 0 then $                                              */
      /*  sgint(l)=interpol(soph,woph,wl(l))/24.96 * arendt(4)/ccdirb(4)    */

  if ( wl > 5.1 && wl < 15.48)
  {     /*include ISOCAM spectrum, scaled to match in-filter 12-micron DIRBE*/
    interpol(woph, soph, NOPH, wl, &sgint);
    sgint = sgint / 24.96 * arendt[4] / ccdirb[4];
  }

    /* ; include fake 3.3 micron line normalized to match Arome  */
  sg = 0.1 / sqrt(8.0 * log(2.0));  /* ; microns for FWHM=0.1 microns */

  work = wl - 3.28;
  sp3 = 1.1e-04 / sg * exp(-(work * work) / 2.0 / (sg * sg));

  *ans = (sgint + fiteval((1.0e4 / wl), 2.0, 17.5, 7.4e-6) + sp3) *
          ccdirb[7];
  lastwl  = wl;
  lastans = *ans;
  return(0);
}
/* ------------------------------------------------------------------------ */
/*  cib_spec_mod                                                            */
/* COMMENTS from IDL version of ism_spec_mod:                               */
/*
;---------------------------------------------------------------
function cib_spec_mod,wl
; fixsen et al.
*/

int cib_spec_mod(wl, ans)
double wl;
double *ans;
{
  static double lastwl;
  static double lastans;
  double firas;
  double fac;
  double dirbe;

  double fiteval();

  if (wl == lastwl)
  {
    *ans = lastans;
    return(0);
  }

  fac = pow((30.0 / 100.0) , 0.64);

  firas = fiteval((1.0e4/wl), 0.64, 18.5, 1.3e-5) * fac;

           /* ; DIRBE                                     */
           /* ; 1.18, 1.09 MJy/sr at 140, 240 microns     */
           /* ; assume color-correction=1 for nuInu=const */

  dirbe = fiteval((1.0e4 / wl), 2.0, 22.0, 0.8e-6);
  *ans = firas + dirbe;
  lastwl = wl;
  lastans = *ans;
  return(0);
}
