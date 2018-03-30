/* planck.c                                                                */
/*   C version of IDL planck                                               */
/* converted/modified from IDL version    j.bennett @ IPAC                 */
/* 15Mar1999                                                               */
/* updated 30Apr1999 to compute pi here instead of using struct ..ztrig    */

/*  COMMENTS from IDL version of planck:                                   */
/*
;----------------------------------------------------------------------------
;+
; NAME:
;    PLANCK
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
;
; REFERENCE:
;    See Allen, Astrophysical Quantities for the Planck formula.
;-
; function planck, T, nu_or_lambda, dBdT, units=units
*/

/* C version assumes nu_or_lambda is lambda in microns.                     */
/* arg. list does not allow for dBdT nor units.                             */

#include <stdio.h>
#include <math.h>

#define MAXV(RJ,CJ)  (((RJ) > (CJ)) ? (RJ) : (CJ))

/*; Define some necessary constants */

#define  c   2.9979e14                   /* Speed of light (micron/sec) */
#define  h   6.6262e-34                  /* Planck constant             */
#define  k   1.3807e-23                  /* Boltzmann constant          */
#define  stephan_boltzmann   5.6697e-12  /* Stephan-Boltzmann constant  */

int planck(T, Lambda, Bnu)
double T;
double Lambda;
double *Bnu;
{
  static double coeff;
  static int nthru = 0;
  double pi, pi5;    /* no need for static, only nthru, coeff need saving. */
  double chi;

  if (nthru == 0)
  {
    pi = 180.0 * (atan(1.0) / 45.0);
    pi5 = pi * pi * pi * pi * pi;
    coeff = 15.0 / pi5 * stephan_boltzmann; /* ;Appropriate combination */
    nthru = 1;
  }

/* Introduce dimensionless variable chi, used to check whether we are on */
/* Wien or Rayleigh Jeans tails                                          */

  chi = h * c / (Lambda * k * MAXV(T, 1.0e-06));
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
