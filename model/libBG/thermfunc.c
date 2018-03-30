/* thermfunc.c                                                              */

/*    C versions of IDL thermfunc, zplanck                                  */
/* converted/modified from IDL version    j.bennett @ IPAC                  */
/* 12,17,18,23feb1999                                                       */
/* 03mar1999: removed colcorr from this file and put it into colcorr.c      */
/* 04mar1999 to improve a debug format.                                     */
/* updated Apr1999 to remove references to det,get_det,colcorr.             */
/* updated 03June1999 to make function zplanck local (i.e. static)          */
 
/* COMMENTS from IDL version of thermfunc:                                  */
/*
;--------------------------------------------------------------
; Function THERMFUNC
;
; Returns the thermal component of the zodiacal dust source 
; function.
;
; Written By: BA Franz, ARC, 9/95
;
; Inputs: 
;   det      - DIRBE detector number (0-9) or -1 if non-DIRBE
;   Lambda   - Wavelength in um
;   R        - sqrt(x^2+y^2+z^2) (AU)
;   a        - Model parameters
;
; Outputs:
;   THERMFUNC - Brightness per particle in MJy/sr
;   df        - Partial derivative of source function wrt each 
;               parameter
;
;--------------------------------------------------------------
;function thermfunc,det,Lambda,R,a,df
*/
 
#include <stdio.h>
#include <math.h>

#include "ztrig_s.h"

#define DEBUG 0

#define  eps  1.0e-20

static int zplanck();

int thermfunc(Lambda, R, a, Therm, npts)
double Lambda;
double R[];
double a[];    /* at least 4 elements long */
double Therm[];
int npts;       /* no. of elements in R = no. elements to be in Therm. */
{
  double To1, To2, Kappa, Delta;
  double cfact;
  int i;
  double divisor;
  double Temp1, Temp2;
  double Bnu1,  Bnu2;
  double CCtherm1, CCtherm2;


/*  ;                                                                      */
/*  ; Thermal Parameters                                                   */
/*  ;                                                                      */

  To1      = a[0];
  To2      = a[1];
  Kappa    = a[2];
  Delta    = a[3];

/*  ; Conversion from W/cm^2/Sr (Nu Bnu) to MJy/Sr (Bnu)                   */
/*  ; Lambda/3e14 Hz^-1 * 1e4 cm^2/m^2 * 1e20 (MJy)/(W/m^2)                */

  cfact = Lambda / 3.0e-10; 

#if DEBUG
  fprintf(stdout,"\nLambda,To1,To2,Kappa,Delta=%7.2f,%9.2f,%9.2f,%9.2f,%15.7f\n",
     Lambda,To1,To2,Kappa,Delta);
#endif

  for ( i = 0; i < npts; i++)
  {
    divisor = pow(R[i], Delta);
/*                              ; First Temperature Component              */

    Temp1    = To1 / divisor;
    zplanck(Temp1, Lambda, &Bnu1);
    Bnu1 = Bnu1 * cfact;

    CCtherm1 = 1.0;       

/*                               ; Second Temperature Component            */
    if (Kappa != 0.0)
    {
      Temp2    = To2 / divisor;
      zplanck(Temp2, Lambda, &Bnu2);    
      Bnu2 = Bnu2 * cfact;
      CCtherm2 = 1.0;       
    }
    else
    {
      Temp2    = 0.0;
      Bnu2     = eps;
      CCtherm2 = 1.0;
    }

#if DEBUG
if (i == 0)
  fprintf(stdout,"Temp1, Bnu1, CCtherm1:");
fprintf(stdout,"%15.7f, %15.7f, %15.7f\n", Temp1, Bnu1, CCtherm1);
#endif

/*                                ; Total Thermal Contribution             */

    Therm[i] = Bnu1 * CCtherm1  + Kappa * Bnu2 * CCtherm2;
  }
  return(0);
}

/* -------------------------------------------------------------------------*/
/*  COMMENTS from IDL version of zplanck:                                   */
/*
;----------------------------------------------------------------------------
;+
; NAME:
;    ZPLANCK
;
; PURPOSE:
;    IDL function to return the spectral radiance of a blackbody,
;    i.e. the Planck curve, in units of W/cm^2/sr.  This is 
;    nu*I_nu (=lambda*I_lambda).  The 
;    blackbody temperature and either frequency (in icm or GHz)
;    or wavelength (in microns) are inputs to the function.  The
;    routine also optionally returns the derivative with respect to 
;    temperature, in units of W/cm^2/sr/K.
;
; CALLING SEQUENCE:
;    result = planck (temperature, nu_or_lambda, [dBbT], [units=<units>])
;
; INPUTS: 
;    T              float scalar    Temperature of blackbody, in K.
;    nu_or_lambda   float [array]   Frequency or wavelength at which to
;                                   calculate spectrum.  Units are as
;                                   specified with "units" keyword.
;
; OPTIONAL INPUT:
;    units          keyword string  'Microns', 'icm', or 'GHz'  to identify 
;                                   units of nu_or_lambda.  Only first 
;                                   character is required.  If left out,
;                                   default is 'microns'.
;
; OUTPUTS:
;    planck         float [array]   Spectral radiance at each wavelength.
;                                   Units are W/cm^2/sr.
;
; OPTIONAL OUTPUT:
;    dB/dT          float [array]   (Optional) derivative of Planck 
;                                   with respect to temperature.  Units 
;                                   are W/cm^2/sr/K
;
; SIDE EFFECTS:
;    None known.
;
; PROCEDURE:
;    Identifies units using the "units" keyword, then converts the 
;    supplied independent variable into microns to evaluate the Planck 
;    function.  Uses Rayleigh-Jeans and Wien approximations for the low-
;    and high-frequency end, respectively.
;
; EXAMPLE:
;    To produce a 35 K spectrum at 2,4,6,8,...,100 microns:
;
;       wavelength = 2. + 2.*findgen(50) 
;       temp  = 35.
;       blackbody = planck (wavelength, temp, units='micron')
;
;    One could also get back the derivative by including it in the call:
;       blackbody = planck (wavelength, temp, deriv, units='m')
;
; RESTRICTIONS:
;    Temperature must be given as a real*4 input, NOT an integer!  
;    Routine also gives incorrect results for T < 1 microkelvin (so sue me).
;
; REVISION HISTORY:
;    Written by Rich Isaacman, General Sciences Corp.  17 April 1991
;    Allow array of Temperatures and single Lambda
;    Rename to avoid confusion  6/95
;
; REFERENCE:
;    See Allen, Astrophysical Quantities for the Planck formula.
;-
; function zplanck, T, lambda, dBdT
*/

/*; Define some necessary constants */

#define  c   2.9979e14                   /* Speed of light (micron/sec) */
#define  h   6.6262e-34                  /* Planck constant             */
#define  k   1.3807e-23                  /* Boltzmann constant          */
#define  stephan_boltzmann   5.6697e-12  /* Stephan-Boltzmann constant  */

static int zplanck(T, Lambda, Bnu)
double T;
double Lambda;
double *Bnu;
{
  extern struct ztrig_s ztrig;
  static double coeff;
  static int nthru = 0;
  double pi5;
  double chi;

  if (nthru == 0)
  {
    pi5 = ztrig.pi * ztrig.pi * ztrig.pi * ztrig.pi * ztrig.pi;
    coeff = 15.0 / pi5 * stephan_boltzmann; /* ;Appropriate combination */
    nthru = 1;
  }

/* Introduce dimensionless variable chi, used to check whether we are on */
/* Wien or Rayleigh Jeans tails                                          */

  chi = h * c / (Lambda * k * T);
                                       /* Start on Rayleigh Jeans side   */
  if (chi < 0.001)
    *Bnu  = coeff * T * T * T * T * chi * chi * chi;

                                       /*  Now do nonapproximate part    */
  else if (chi >= 0.001 && chi <= 50.0)
    *Bnu = coeff * T * T * T *T * chi * chi * chi * chi / (exp(chi) - 1.0);

                                       /* ...and finally the Wien tail   */
  else if (chi > 50)
    *Bnu = coeff * T * T * T * T * chi * chi * chi *chi * exp(-chi);

  else
    *Bnu = 0.0;

  return(0);
}
#undef   c
#undef   h
#undef   k
#undef   stephan_boltzmann
