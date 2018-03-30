/* zkernel.c                                                               */
/*       C version of IDL zkernel_nopartial.pro                            */
/*   converted/modified from IDL version    j.bennett @ IPAC               */
/*   08-10,18,22,23,25feb1999                                              */
/*  Modified 13-15Apr1999 to remove det references and use wavelength      */
/*     references only. Added funcs get_Alb,get_Emis. Renamed and modified */
/*     zsrcfunc1 (new name zsrcfunc2).                                     */
/*  03June1999 to make the following functions local (i.e static) :        */
/*     gaussint, get_Alb,get_Emis,zcloud,zsrcfunc2,migband,solring.        */
/*  16May2000:    function get_Alb -- to correct Albedo computation per    */
/*     Bill Reach's corrected equation.                                    */
/*  18May2000:  To change lambda 1.2 to 1.25 in get_Alb function.          */
/*  15Mar-16Apr2002: Many changes to conform to new IDL version algorithm. */
/*        updates to functions zcloud, migband, solring, and zkernel.      */
/*  19Jun2002: updates (per IDL changes) in function solring().            */

/* functions in this C file:  zkernel, gaussint, get_Alb, get_Emiss,       */
/*     zcloud, zsrcfunc2, migband, solring.                                */

/* COMMENTS from IDL version of zkernel:                                   */
/*
;============================================================================
;
;                               ZKERNEL
;
;    DIRBE Kernel for use by ZMODEL to Fit the Zodiacal Dust Cloud
;
;                       Last Revision: 17 November 1995
;
; removed partial derivative calculations for similicity
; inserted emissivity & albedo interpolation for non-DIRBE wavelengths
;
;============================================================================
;-------------------------------------------------------------------------------
; IDL Function ZKERNEL.PRO
;
; Zodiacal Dust Model
;
; Primary Authors: B.A. Franz, ARC, (301) 513-7776
;                  W.T. Reach, USRA (301) 286-4255
; Inputs:
;    x - n-element structured array of independent variables which at least
;        contains the following fields.  (See ZODI_STR.PRO)        
;        x.longitude  : Heliocentric Ecliptic longitude of LOS in degrees
;        x.latitude   : Heliocentric Ecliptic latitude of LOS in degrees
;        x.day        : Day Number of Observation
;        x.year       : Year
;        x.wave_len   : Wavelength in um
;
;    a - m-element floating array of model parameters
;
; Optional Inputs:
;    indxpar=indxpar - array of indices of variable parameters (Def=0..m-1)
;
; Outputs:
;    f  - n-element floating array of Model Flux in MJy/Sr
;
; Optional Outputs:
;    df - nxm array of partial derivatives for model parameters at each x
: Modification History
;  Updated dust bands, solar ring, and trailing blob models
;   using the Kelsall '98 paper; B.Bhattacharya
;                                SIRTF Science Center, 7/01
;  Start LOS at SIRTF instead of at Earth, BB, 8/01
;
;-------------------------------------------------------------------------------
;  pro zkernel,data,a,f,df,indxpar=indxpar,losinfo=losinfo
*/

#include <stdio.h>
#include <math.h>
#include "zdata_s.h"
#include "zfuncs_s.h"
#include "sirtfd_s.h"
#include "ztrig_s.h"

#define DEBUG 0            /* 1 for print statements, 0 for none */

#define DMOD(RJ,CJ) ( (RJ) - (int) ((RJ)/(CJ)) * (CJ) )

#define  EPS  1.0e-20
#define  NUMPTS    50 /* size of grid used in gaussint, etc. */

#pragma ident "@(#) zkernel.c updated 19June2002."

     /* note: 14apr99 vers. does not use nSrc_..,,iSrc_.. values. */
                      /* lengths for various arrays       */
#define  nScatt     9
#define  nTherm     4
#define  nDens_C   19
#define  nSrc_C    12
#define  nDens_B1  14
#define  nSrc_B1   12
#define  nDens_B2  14
#define  nSrc_B2   12
#define  nDens_B3  14
#define  nSrc_B3   12
#define  nDens_B4  14
#define  nSrc_B4   12
#define  nDens_RB  21
#define  nSrc_RB   12
                        /* starting points in zpar array:  */
#define  iTherm    10
#define  iDens_C   14
#define  iSrc_C    33
#define  iDens_B1  45
#define  iSrc_B1   59
#define  iDens_B2  71
#define  iSrc_B2   85
#define  iDens_B3  97
#define  iSrc_B3  111
#define  iDens_B4 123
#define  iSrc_B4  137
#define  iDens_RB 149
#define  iSrc_RB  170
  
static int gaussint(double grid[], double wts[]);
static int get_Alb(double wave_len, double *Albedo);
static int get_Emis(double wave_len, double *Emiss);
static int zcloud(double x[], double y[], double z[], double R[],
		double a[], double Dens[], int npts);
static int zsrcfunc2(double Albedo, double Emiss, double Scatt[],
		   double Therm[], double Source[], int npts);
static int migband(double x[], double y[], double z[], double R[],
		 double a[], double Dens[], int npts);
static int solring(double x[], double y[], double z[], double R[],
		 double Theta, double a[], double Dens[], int npts);

int zkernel(a, nzpar, zdata, zpos, sirtfdata, z)
double a[];     /*  the zpar array  */
int nzpar;      /*  no. in zpar array - presently 256 */
struct zdata_s *zdata;
struct zfuncs_s *zpos;
struct sirtfdata_s *sirtfdata;

double *z;
{
  int i, k;
  double Sxy;

  double Albedo, Emiss;
  double aScatt[nScatt];
  double aTherm[nTherm];
  double aDens_C[nDens_C];
  double aDens_B1[nDens_B1];
  double aDens_B2[nDens_B2];
  double aDens_B3[nDens_B3];
  double aDens_B4[nDens_B4];
  double aDens_RB[nDens_RB];

  double los[NUMPTS], gqwts[NUMPTS];
  double X[NUMPTS];
  double Y[NUMPTS];
  double Z[NUMPTS];
  double R[NUMPTS];
  double Scatt[NUMPTS];
  double Therm[NUMPTS];
  double Dens_C[NUMPTS];
  double Dens_B1[NUMPTS];
  double Dens_B2[NUMPTS];
  double Dens_B3[NUMPTS];
  double Dens_B4[NUMPTS];
  double Dens_RB[NUMPTS];
  double Src_[NUMPTS];
  double Flux;
  double f;

/*  Xearth,Yearth,Zearth,Rearth do no seem to be used in this implementation*/
/*    -- will comment out here.                                             */
/*  double Xearth[NUMPTS], Yearth[NUMPTS], Zearth[NUMPTS], Rearth[NUMPTS]; */

  int scattfunc(double los[], double R[], double Re, double SolElong,
                double a[], int numa, double wave_len, double Scatt[],
                int npts);
  int thermfunc(double Lambda, double R[], double a[], double Therm[],
                int npts);


                                /* Set density function selection flag   */
                                /* IDL used function  fix(a(0))          */
                                /* C version does not use FuncIndx       */

                         /*-- Get Scatt Function Parameters (Place Holder)--*/
  for (i = 0; i < nScatt; i++)
    aScatt[i] = 0.0;

                                       /*-- Get Therm Function Parameters --*/
  for (i = iTherm, k = 0; k < nTherm; i++, k++)
    aTherm[k] = a[i];

                                       /*--  Get Cloud Parameters         --*/

  for (i = iDens_C, k = 0; k < nDens_C; i++, k++)      /* Density params    */
    aDens_C[k] = a[i];

                                          /*-- Get Dust Band 1 Parameters --*/

  for (i = iDens_B1, k = 0; k < nDens_B1; i++, k++)    /* Density params    */
    aDens_B1[k] = a[i];

                                          /*-- Get Dust Band 2 Parameters --*/

  for (i = iDens_B2, k = 0; k < nDens_B2; i++, k++)    /* Density params    */
    aDens_B2[k] = a[i];

                                          /*-- Get Dust Band 3 Parameters --*/
 
  for (i = iDens_B3, k = 0; k < nDens_B3; i++, k++)    /* Density params    */
    aDens_B3[k] = a[i];

                                          /*-- Get Dust Band 4 Parameters --*/

  for (i = iDens_B4, k = 0; k < nDens_B4; i++, k++)    /* Density params    */
    aDens_B4[k] = a[i];

                                         /*-- Get Solar Ring Parameters   --*/

  for (i = iDens_RB, k = 0; k < nDens_RB; i++, k++)    /* Density params    */
    aDens_RB[k] = a[i];


/* ; Associate wavelengths with fullband detector numbers  */
/* ; 1a=1b=1c=0 .... 9=b10                                 */
/*         fullband detector numbers not done here in C version    */


/* Calculate position of Earth in heliocentric ecliptic coords and */
/*  Solar elongation of LOS -- already done for C and is in zdata structure */


/* Set grid, weights for Gauss-Leguerre quadrature  */
/* Define LOS Integration Points (from Earth in AU) */

  gaussint(los, gqwts); /* los and gqwts each dimensioned global NUMPTS */

#if DEBUG
/* DISPLAY VALUES for TEST */
  showit(stdout, "los", los, NUMPTS);
  showit(stdout, "gqwts", gqwts, NUMPTS);
#endif

/* ; Set-up for calculation of integrated flux    */
/* if (n_elements(f) ne npts) then f = dblarr(npts) -- npts = 1, C scalar */

/* ; The code used to optimize by calculating partials was
   ; removed for deliver 6.0 as this capability will not be
   ; used by the user via SPOT.  BB, SSC, 9/01
         NOTE: this code had already been omitted from C version. JDB.
*/

/* Loop over each LOS, Time, Wavelength  */

/* for ilos = 0L,npts-1 do begin   --   npts = 1 in IDL  */

/*    ; Compute Basic Geometry -- already done - in zpos structure for C */
/*                                          and in zdata structure for C */

/*      Position of LOS elements in Heliocentric Ecliptic coord system.  */

  for (i = 0; i < NUMPTS; i++)
  {
    Sxy  = los[i] * zpos->cos_elat;

/* Xearth,Yearth,Zearth,Rearth, while computed in IDL verion, seem never used.*/
/*  Xearth[i] = zdata->Re * zpos->cos_theta + Sxy * zpos->cos_elon;
*   Yearth[i] = zdata->Re * zpos->sin_theta + Sxy * zpos->sin_elon;
*   Zearth[i] = los[i] * zpos->sin_elat;
*   Rearth[i] = sqrt( X[i] * X[i] + Y[i] * Y[i] + Z[i] * Z[i]);
*/

    X[i] = sirtfdata->xsirtf + Sxy * zpos->cos_elon;
    Y[i] = sirtfdata->ysirtf + Sxy * zpos->sin_elon;
    Z[i] = sirtfdata->zsirtf + los[i] * zpos->sin_elat;
    R[i] = sqrt( X[i] * X[i] + Y[i] * Y[i] + Z[i] * Z[i]);
  }

  #if DEBUG
  fprintf(stdout,"elonr=%16.10f\n",zpos->elonr);
  fprintf(stdout,"elatr=%16.10f\n",zpos->elatr);
  fprintf(stdout,"Re=%16.10f\n",zdata->Re);
  fprintf(stdout,"Theta=%16.10f\n",zdata->Theta);
  fprintf(stdout,"sirtfdata->xsirtf=%16.10f\n", sirtfdata->xsirtf);
  fprintf(stdout,"sirtfdata->ysirtf=%16.10f\n", sirtfdata->ysirtf);
  fprintf(stdout,"sirtfdata->zsirtf=%16.10f\n", sirtfdata->zsirtf);

  showit(stdout, "X",X, NUMPTS);
  showit(stdout, "Y",Y, NUMPTS);
  showit(stdout, "Z",Z, NUMPTS);
  showit(stdout, "R",R, NUMPTS);
  #endif

/*  ; Source Function per LOS element                                    */
/*  ; ------------------------------------------------------------------ */
/*  Lambda = double(data(ilos).wave_len) */

  scattfunc(los, R, zdata->Re, zdata->SolElong, aScatt, nScatt, 
            zdata->wave_len, Scatt, NUMPTS);

  thermfunc(zdata->wave_len, R, aTherm, Therm, NUMPTS);
 
  get_Alb(zdata->wave_len, &Albedo);

  get_Emis(zdata->wave_len, &Emiss);

  zsrcfunc2(Albedo, Emiss, Scatt, Therm,  Src_, NUMPTS);

  /*; Calculate Density            Associated With Smooth Cloud Component */
  /*; ------------------------------------------------------------------- */

  zcloud(X, Y, Z, R, aDens_C, Dens_C, NUMPTS);


/*  ; Calculate Density            Associated with Dust Bands              */
/*  ; -------------------------------------------------------------------  */

  migband(X, Y, Z, R, aDens_B1, Dens_B1, NUMPTS);

  migband(X, Y, Z, R, aDens_B2, Dens_B2, NUMPTS);

  migband(X, Y, Z, R, aDens_B3, Dens_B3, NUMPTS);

  migband(X, Y, Z, R, aDens_B4, Dens_B4, NUMPTS);


/*  ; Calculate Density            Associated with Solar Ring/Blobs        */
/*  ; -------------------------------------------------------------------  */

  solring(X, Y, Z, R, zdata->Earth_Mean_Lon, aDens_RB, Dens_RB, NUMPTS);


/*  ; Calculate Total Zodi Brightness per LOS Element                      */
/*  ; -------------------------------------------------------------------- */

#if DEBUG
  showit(stdout, "Scatt",Scatt,NUMPTS);
  showit(stdout,"aTherm", aTherm, 4);
  showit(stdout, "Therm", Therm, NUMPTS);
  showite(stdout, "Dens_C", Dens_C, NUMPTS);
  showite(stdout, "Src_:", Src_, NUMPTS);
showite(stdout, "Dens_B1", Dens_B1, NUMPTS);
showite(stdout, "Dens_B2", Dens_B2, NUMPTS);
showite(stdout, "Dens_B3", Dens_B3, NUMPTS);
showite(stdout, "Dens_B4", Dens_B4, NUMPTS);
showite(stdout, "Dens_RB", Dens_RB, NUMPTS);
fprintf(stdout,"Flux:");
#endif

  f = 0.0;
  for (i = 0; i < NUMPTS; i++)
  {
    Flux = Src_[i] * (Dens_C[i]  +
                     Dens_B1[i] +
                     Dens_B2[i] +
                     Dens_B3[i] +
                     Dens_B4[i] +
                     Dens_RB[i] );

#if DEBUG
if ((i % 4) == 0)
  fprintf(stdout,"\n");
fprintf(stdout,"%16.6f",Flux);
#endif

/*  ; Integrate Along LOS to Get Total Intensity                           */
/*  ; ------------------------------------------                           */

    f = f + (gqwts[i] * Flux);
  }

#if DEBUG
fprintf(stdout,"\n");
#endif

  *z = f;
  return(0);
}
/* ------------------------------------------------------------------------ */
/* gaussint                                                                 */

/*   COMMENTS from IDL version of gaussint:                                 */
/*
; Function GAUSSINT
;
; Returns Gauss-Laguerre abscissae and weights for 24 or 50 point gaussian 
; quadrature.
;
; Written By: BA Franz, ARC, 9/94
; Quadrature abscissae and weights computed by W. T. Reach
;
;----------------------------------------------------------------------------
;pro gaussint,a,b,grid,wts,numpts=numpts
*/
/*  C version eliminates 24 point arrays, hard coded a and b  */

static int gaussint(grid, wts)
double grid[];  /* dimensioned global NUMPTS in caller */
double wts[];   /* dimensioned global NUMPTS in caller */
{
  static int nthru = 0;

  static double grid50[NUMPTS] =
        {0.0286305183,0.1508829357,0.3709487815,0.6890906999,
         1.1056250235,1.6209617511,2.2356103759,2.9501833666,
         3.7653997744,4.6820893876,5.7011975748,6.8237909098,
         8.0510636694,9.3843453083,10.825109031,12.374981608,
         14.035754599,15.809397197,17.698070933,19.704146535,
         21.830223306,24.079151444,26.454057841,28.958376011,
         31.595880956,34.370729963,37.287510610,40.351297573,
         43.567720270,46.943043991,50.484267963,54.199244880,
         58.096828017,62.187054175,66.481373878,70.992944826,
         75.737011547,80.731404802,85.997211136,91.559690412,
         97.449565614,103.70489123,110.37385880,117.51919820,
         125.22547013,133.61202792,142.85832548,153.26037197,
         165.3856433,180.69834370};

  static double wts50[NUMPTS] =
        {0.0734786263,0.1711133062,0.2690583985,0.3672778840,
         0.4658590232,0.5648992928,0.6644999757,0.7647657788,
         0.8658052545,0.9677314398,1.0706625889,1.1747230029,
         1.2800439495,1.3867646993,1.4950336890,1.6050098453,
         1.7168640913,1.8307810796,1.9469611911,2.0656228595,
         2.1870052872,2.3113716402,2.4390128272,2.5702520038,
         2.7054499706,2.8450116969,2.9893942558,3.1391165624,
         3.2947714220,3.4570405806,3.6267137126,3.8047126447,
         3.9921226303,4.1902332693,4.4005928365,4.6250816069,
         4.8660126478,5.1262732767,5.4095283377,5.7205203736,
         6.0655271405,6.4530854743,6.8951889633,7.4093807809,
         8.0226692310,8.7795282602,9.7603031266,11.131390723,
         13.324267692,18.114146002};

  double div50;
  double a, b, factor;
  int i;

    /* IDL grid50 and wts50 values were divided by 180.69834370    */
    /* returned grid = grid50 * (b - a) + a                        */
    /* returned wts  = wts50  * (b - a)                            */
  if (nthru == 0)
  {
    div50 = 180.69834370;
    a = 0.0;
    b = 5.0;
    factor = ( b - a ) / div50;

    for ( i = 0; i < NUMPTS; i++)
     grid50[i] = grid50[i] * factor + a;

    for ( i = 0; i < NUMPTS; i++)
     wts50[i]  = wts50[i]  * factor;
    nthru = 1;
  }

  for (i = 0; i < NUMPTS; i++)
    grid[i] = grid50[i];

  for (i = 0; i < NUMPTS; i++)
    wts[i]  = wts50[i];
  return(0);
}
/* ------------------------------------------------------------------------ */
/* get_Alb   to compute Albedo as function of wavelength.                   */
/*    j.bennett @ IPAC  14Apr1999                                           */
/*    Algorithm, constants from W.Reach.                                    */
/*  updated 16may2000 for W.Reach's updated algorithm.                      */

static int get_Alb(wave_len, Albedo)
double wave_len;       /* in microns */
double *Albedo;
{
  double aa[5]      = {0.270, 0.204, 0.255, 0.210, 0.000};
  double lambdas[5] = { 0.55, 1.25,  2.2,   3.5,   4.9  };

  int interpol();
                                    /* Albedo:   */
  if (wave_len < 0.55)
    *Albedo = aa[0];
  else if (wave_len >= 4.9)
    *Albedo = 0.0;
  else                        /* here wave_len between 0.55 and 4.9  */
    interpol(lambdas, aa, 5, wave_len, &(*Albedo));

  #if DEBUG
  fprintf(stdout,"\n\n Albedo= %16g\n\n", *Albedo);
  #endif

  return(0);
}
/* ------------------------------------------------------------------------ */
/* get_Emis   to compute emissivity as function of wavelength.              */
/*   j.bennett @ IPAC   14Apr1999                                           */
/*    Algorithm, constants from W.Reach.                                    */

static int get_Emis(wave_len, Emiss)
double wave_len;     /* in microns */
double *Emiss;
{
  double work;             /* reusable work variable. */

                                    /* Emissivity */
  work = 2.2224 - 0.463 * wave_len;

                   /* Peg work so that for wavelengths < 2um, work stays at */
                   /*   value it would have at 2um.                         */
  if (work > 1.3)
    work = 1.3;
  else if (work < 0.0)
    work = 0.0;

  *Emiss = (300.0 / (280.0 + wave_len)) + work;

  #if DEBUG
  fprintf(stdout,"\n\n Emiss= %16g\n\n", *Emiss);
  #endif

  return(0);
}
/* ------------------------------------------------------------------------ */
/* zcloud                                                                   */

/*  COMMENTS from IDL version of zcloud:                                    */
/*   (note:  C version will only do funcindex = 2 (Widened Modified Fan))   */
/*
;--------------------------------------------------------------
; Function ZCloud
;
; Returns the number density of the zodiacal cloud.
;
; Written By: BA Franz, ARC, 9/95
;
; Inputs: 
;   x,y,z - heliocentric cartesian coordinates
;   R     - sqrt(x^2+y^2+z^2)
;   a     - Model parameters
;
; Keyword Inputs:
;   FuncIndex - selector for latitudinal density distribution
;		0 : Good
;		1 : Modified Fan
;		2 : Widened Modified Fan
;		3 : Ellipsoid
;		4 : Sombrero
;
; Outputs:
;   ZCloud  - number density
;   df      - partial of density wrt each parameter
;
;--------------------------------------------------------------
;function zcloud,x,y,z,R,a,df,FuncIndx=FuncIndx
*/
static int zcloud(x,y,z,R,a,Dens,npts)
double x[];
double y[];
double z[];
double R[];
double a[];
double Dens[];  /* must be dimensioned npts in caller;npts = global NUMPTS) */
int npts;
{
  extern struct ztrig_s ztrig;
  int i;
  double dif;
  double No, Alpha, Beta, Gamma;
  double Mu, Mu2, Mu3, Mu4, Mu5, Mu6, Mu7, Mu8, Mu9, Mu10;
  double Omega, Incl, Xo, Yo, Zo;
  double sino, coso, sini, cosi;
  double Xp, Yp, Zp, Rc, Zc, Zeta, AlphaR, GZR, GZR2G;
  double Dens_Cloud_Vert;
  double Dens_Cloud_Rad;

  No = a[0];

  if (No == 0.0)
  {
    for (i = 0; i < npts; i++)
      Dens[i] = 0.0;
    return(0);
  }


  Alpha    = a[1];
  Beta     = a[2];
  Gamma    = a[3];
  Mu       = a[4];
  Mu2      = a[5];
  Mu3      = a[6];
  Mu4      = a[7];
  Mu5      = a[8];
  Mu6      = a[9];
  Mu7      = a[10];
  Mu8      = a[11];
  Mu9      = a[12];
  Mu10     = a[13];

  Omega    = a[14] * ztrig.dtor;
  Incl     = a[15] * ztrig.dtor;
  Xo       = a[16];
  Yo       = a[17];
  Zo       = a[18];


/*  ; Some frequently used geometry terms      */

  sino = sin(Omega);
  coso = cos(Omega);
  sini = sin(Incl);
  cosi = cos(Incl);

  for (i = 0; i < npts; i++)
  {
                    /*  ; Translate origin from Sun to cloud center.    */
    Xp  = x[i] - Xo;
    Yp  = y[i] - Yo;
    Zp  = z[i] - Zo;
    Rc  = sqrt( Xp * Xp + Yp * Yp + Zp * Zp );


    /*  ; Rotate into coord system of cloud to determine Z-height in cloud */
    /*  ; (Zc) and radial distance in cloud symetry plane (Rc).            */
    /*  ; Perform rotation of Omega around Z-Axis to align X with cloud    */ 
    /*  ; line-of-nodes. Then, perform Incl rotation around X-Axis.        */

    Zc   = sino * sini * Xp - coso * sini * Yp + cosi * Zp;
    Zeta = fabs(Zc/Rc);


    /*    ; Compute Radial power-law parameter                          */

    dif = R[i] - 1.0;
    AlphaR = Alpha + Mu6 * dif + Mu7 * dif * dif + Mu8 * dif * dif *dif;


    /*  ; Radial and Latitudinal Density Distribution                   */
    /*  case FuncIndx of   (IDL statement)                              */

    /*  ;     John Good Density Function                                */
    /*  0: begin                                                        */
    /*      Not ported to C.                                            */
    /*                                                                  */
    /*  ; Modified Fan Density Function                                 */
    /*  1: begin                                                        */
    /*      Not ported to C.                                            */

 
    /*  ; Widened Modified Fan Density Function (FuncIndx of 2)         */

    if (Zeta < Mu)
      GZR    = 0.5 * Zeta * Zeta / Mu;
    else
      GZR    = Zeta - 0.5 * Mu;

    GZR2G  = pow(GZR,Gamma);

    Dens_Cloud_Vert = exp( -(Beta) * GZR2G );
    Dens_Cloud_Rad  = 1.0 / pow(Rc, AlphaR);

    /*  ; Ellipsoidal Density Function                                  */
    /*  3: begin                                                        */
    /*      Not ported to C.                                            */

    /*  ; Cosine (Sombrero)                                             */
    /*  4: begin                                                        */
    /*      Not ported to C.                                            */

    /*  ; Modified Fan Density Function with Polynomial z-height        */
    /*  5: begin                                                        */
    /*      Not ported to C.                                            */

    /*  endcase   (IDL statement)                                       */


    /*  ; Total Density of Cloud Component                              */

    Dens[i] = No * Dens_Cloud_Vert * Dens_Cloud_Rad;

    /* ; The code used to optimize by calculating partials was
       ; removed for deliver 6.0 as this capability will not be
       ; used by the user via SPOT.  BB, SSC, 9/01
         NOTE: this code had already been omitted from C version. JDB.
    */
  }
  return(0);
}
/* ------------------------------------------------------------------------ */
/* zsrcfunc2                                                                */

/*  COMMENTS from IDL version of zsrcfunc:                                  */
/*
;--------------------------------------------------------------
; Function ZSRCFUNC
;     note, this function seems to be ZSRCFUNC1
;
; Returns the zodiacal dust source function, the brightness per 
; dust grain.
;
; Written By: BA Franz, ARC, 9/95
;
; Inputs: 
;   Scatt    - Scattered light contribution
;   Therm    - Thermal emission contribution
;   a        - Model parameters (Emissivities/Albedos)
;
; Outputs:
;   ZSRCFUNC  - Brightness per particle in MJy/sr
;   df        - Partial derivative of source function wrt each 
;               parameter
;
;--------------------------------------------------------------
;function zsrcfunc1,wave_len,Scatt,Therm,a
*/

/*  re C-version:                                                          */
/*  Modified                13Apr1999 to remove det references and use     */
/*      wavelength references only.                                        */
/*  14Apr1999 removed Albedo and Emissivity computations from this function */
/*    and placed in get_Alb and get_Emis.  Renamed zsrcfunc1 to zsrcfunc2.  */

static int zsrcfunc2(Albedo, Emiss, Scatt, Therm, Source, npts)
double Albedo;       /* Albedo      (computed in get_Alb)   */
double Emiss;        /* Emissivity  (computed in get_Emis)  */
double Scatt[];      /* Scattered light contribution.       */
double Therm[];      /* Thermal emission contribution.      */
double Source[];     /* Output: brightness per particle in MJy/sr  */
int npts;            /* no. values in Scatt, Therm and Source arrays. */
{
  double work;             /* reusable work variable. */
  int i;

/*  ; Calculate Total Source per LOS Element                               */
/*  ; ------------------------------------------                           */

  work = 1.0 - Albedo;
  for (i = 0; i < npts; i++)
    Source[i] = Albedo * Scatt[i] + Emiss * work * Therm[i];
  return(0);
}    
/* ------------------------------------------------------------------------ */
/* migband                                                                  */

/*  COMMENTS from IDL version of migband                                    */
/*
;--------------------------------------------------------------
; Function MigBand
;
; Returns the number density of a dust band.
;
; The model is a parameterization of the migrating band concept
; of W. T. Reach, as provided by Reach.
;
; Written By: BA Franz, ARC, 9/95
;
; Inputs: 
;   x,y,z - heliocentric cartesian coordinates
;   R     - sqrt(x^2+y^2+z^2)
;   a     - Model parameters
;
; Outputs:
;   migband - number density
;   df      - partial of density wrt each parameter
;
; Modification History:
;       Update expression for cloud density.  Original relation is
;       eq. 1 from Reach, et al., '97.  Replace this with eq.8
;       from Kelsall, et al., '98.
;         -B.Bhattacharya, SIRTF Science Center, 6/27/01    
;
;--------------------------------------------------------------
;function migband,x,y,z,R,a,df
*/
/* note: R (in arg. list) that had not been used in may2000 version, is now  */
/*       needed in this mar2002 version and has been put back into arg.list. */                                  

static int migband(x, y, z, R, a, Dens, npts)
double x[];
double y[];
double z[];
double R[];
double a[];
double Dens[];   /* must be dimensioned npts in caller;npts = global NUMPTS)*/
int npts;
{
  extern struct ztrig_s ztrig;
  int i;
  double No, Dz, Dr, Ro, Vi, Vr, Pi, Pr, P2r, Omega, Incl, Xo, Yo, Zo;
  double sino, coso, sini, cosi;
  double Xp, Yp, Zp, Zc,     Zeta, ZDz, RDr;
  double term1_dens, term2_dens, term3_dens, term4_dens;

  No = a[0];
  if (No == 0)
  {
    for (i = 0; i < npts; i++)
      Dens[i] = 0.0;
    return(0);
  }

  Dz       = a[1] * ztrig.dtor;
  Dr       = a[2];
  Ro       = a[3];
  Vi       = a[4];
  Vr       = a[5];
  Pi       = a[6];
  Pr       = a[7];
  P2r      = a[8];
  Omega    = a[9]  * ztrig.dtor;
  Incl     = a[10] * ztrig.dtor;
  Xo       = a[11];
  Yo       = a[12];
  Zo       = a[13];
  sino = sin(Omega);
  coso = cos(Omega);
  sini = sin(Incl);
  cosi = cos(Incl);

/*  ; Translate origin from Sun to cloud center.                         */

  for (i = 0; i < npts; i++)
  {
                 /*  : want to cut off density at Ro=3AU, BB, 6/29/01 */
    if (R[i] < Ro)
    {
      Xp  = x[i] - Xo;
      Yp  = y[i] - Yo;
      Zp  = z[i] - Zo;

/*  ; Rotate into coord system of cloud to determine Z-height in cloud   */
/*  ; (Zc) and radial distance in cloud symetry plane (Rc).              */
/*  ; Perform rotation of Omega around Z-Axis to align X with cloud      */
/*  ; line-of-nodes. Then, perform Incl rotation around X-Axis.          */

/*  ; use Kelsall, eq. 8, BB, 6/27/01                                    */

      Zc = sino * sini * Xp - coso * sini * Yp + cosi * Zp;

      Zeta   = fabs(Zc / Ro);
      ZDz    = Zeta / Dz;
      RDr    = R[i] / Dr;

      term1_dens = 3.0 * No / R[i];
      term2_dens = exp(-(ZDz * ZDz * ZDz * ZDz * ZDz * ZDz));
      term3_dens = Vi + pow(ZDz, Pi);
      term4_dens = 1.0 - exp(- pow(RDr, 20.0)); 

      Dens[i] = term1_dens * term2_dens * term3_dens * term4_dens;
    }
    else
      Dens[i] = 0.0;
  }

/*   ; The code used to optimize by calculating partials was
     ; removed for deliver 6.0 as this capability will not be
     ; used by the user via SPOT.  BB, SSC, 9/01
       NOTE: partials code had already been omitted from C version.  JDB.
*/
  return(0);
}
/* ------------------------------------------------------------------------ */
/*  COMMENTS from IDL version of solring:                                   */
/*
;--------------------------------------------------------------
; Function SolRing
;
; Returns the number density of the Earth's resonant dust ring.
; The model is a gaussian toroidal ring with Earth-leading and
; Earth-trailing gaussian blobs.  It is a parameterization of
; W. T. reach based on numerical images published by S. Dermott.
;
; Written By: BA Franz, ARC, 9/95
;
; Inputs: 
;   x,y,z - heliocentric cartesian coordinates
;   R     - sqrt(x^2+y^2+z^2)
;   Theta - Relative to Earth mean longitude
;   a     - Model parameters
;
; Outputs:
;   solring - number density
;   df      - partial of density wrt each parameter
; Modification History:
;       Update expression for solar ring density.  Replace
;     original equation with eq.9 from Delsall, et al., '98.
;       -B.Bhattacharya, SIRTF Science Center, 7/6/01
;       Angular ranges need to be accounted for:
;        ATAN(Yp,Xp): -(!pi) to +(!pi)
;        Theta (EML): 0.0 to 359.9999)
;        dTheta: -359.9999 to +(359.9999)
;        dTheta = ATAN(Yp,Xp), - Theta; IDL does not step smoothly
;         with position, e.g., dTheta = -8.2 degs at one pos and then
;         dTheta = 352.0 degs at the next pos when stepping around the sky
;       -B.Bhattacharya, SSC, 6/12/02
;
;--------------------------------------------------------------
;function solring,x,y,z,R,Theta,a,df
*/

static int solring(x, y, z, R, Theta, a, Dens, npts)
double x[];
double y[];
double z[];
double R[];
double Theta;    /* Earth mean longitude in radians. */
double a[];
double Dens[];   /* must be dim. npts (= global NUMPTS) in caller. */
int npts;
{
  extern struct ztrig_s ztrig;
  int i;
  double dif;                /* temporary, volatile work variable */
  double ang;

  double SR_No, LB_No, TB_No;
  double SR_R, SR_dR, SR_dZ;
  double LB_R, LB_dR, LB_Theta, LB_dTheta, LB_dZ;
  double TB_R, TB_dR, TB_Theta, TB_dTheta, TB_dZ;
  double Omega, Incl, Xo, Yo, Zo;
  double sino, coso, sini, cosi;
  double SR_Z, dTheta,                     Dens_SR,          Dens_TB;
  double Xp, Yp, Zp;
  double expval1, expval2, expval3, expval4, expval5;

  SR_No = a[0];
  LB_No = a[4];
  TB_No = a[10];

  if (SR_No == 0.0 && LB_No == 0.0 && TB_No == 0)
  {
    for (i = 0; i < npts; i++)
      Dens[i] = 0.0;
    return(0);
  }
                                   /*    Set Parameters        */

                                   /*    Ring                  */
  SR_R        = a[1];
  SR_dR       = a[2];
  SR_dZ       = a[3];
                                   /*    Leading Blob          */
  LB_R        = a[5];    
  LB_dR       = a[6];
  LB_Theta    = a[7] * ztrig.dtor;
  LB_dTheta   = a[8] * ztrig.dtor;
  LB_dZ       = a[9];
                                   /*    Trailing Blob         */
  TB_R        = a[11];
  TB_dR       = a[12];
  TB_Theta    = a[13] * ztrig.dtor;
  TB_dTheta   = a[14] * ztrig.dtor;
  TB_dZ       = a[15];
                                   /*    Symmetry plane        */
  Omega       = a[16] * ztrig.dtor;
  Incl        = a[17] * ztrig.dtor;
  Xo          = a[18];
  Yo          = a[19];
  Zo          = a[20];

/*  ; Some frequently used geometry terms                                  */

  sino = sin(Omega);
  coso = cos(Omega);
  sini = sin(Incl);
  cosi = cos(Incl);

/*  ; Compute Density                                                      */

  for (i = 0; i < npts; i++)
  {
    Xp = x[i] - Xo;
    Yp = y[i] - Yo;
    Zp = z[i] - Zo;

/*  ;vertical distance above midplane of the ring     */

    SR_Z     = Xp * sino * sini - Yp * coso * sini + Zp * cosi;

/*  :=============================================================    */

/*  ;The following is the Kelsall '98 solar ring and trailing blob
    ;relation.  They do not include the leading blob.  BB, 7/9/01

    ;For atan(Yp,Xp) > 179.5, angle becomes negative as atan range
    ; is 0.0 --> +180.0 --> -180.0 --> 0.0
*/
    ang = atan2(Yp, Xp);    /* ;angle of LOS - observatory (SIRTF) radians */
    if (ang < 0.0)
      ang = ztrig.pi2 + ang;

    dTheta   =    ang  - Theta;  /* ; angle of LOS - EML      */
                                 /* ;dTheta should range from -180 to +180 */
    if (dTheta > ztrig.pi)
      dTheta = dTheta - ztrig.pi2;
    if (dTheta <= (-ztrig.pi) )
      dTheta = ztrig.pi2 + dTheta;

    dif = R[i] - SR_R;
    expval1 = (dif * dif ) / (2.0 * SR_dR * SR_dR);
    expval2 = fabs(SR_Z) / SR_dZ;
    dif = R[i] - TB_R;
    expval3 = (dif * dif) / (2.0 * TB_dR * TB_dR);
    expval4 = fabs(SR_Z) / TB_dZ;
    dif = dTheta + fabs(TB_Theta);
    expval5 = (dif * dif) / (2.0 * TB_dTheta * TB_dTheta);

    Dens_SR  = SR_No * exp(-(expval1) - expval2);
    
    Dens_TB  = TB_No * exp(-(expval3) - expval4 - expval5);

    Dens[i] = Dens_SR + Dens_TB;

/*   ; The code used to optimize by calculating partials was
     ; removed for deliver 6.0 as this capability will not be
     ; used by the user via SPOT.  BB, SSC, 9/01
       NOTE: partials code had already been omitted from C version.  JDB.
*/

  }
  return(0);
}
/* ------------------------------------------------------------------------ */
