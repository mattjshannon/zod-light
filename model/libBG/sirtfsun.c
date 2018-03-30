/*  sirtfsun.c     19March2002     j.bennett @ IPAC                        */
/*    updated 25mar2002 to fill struct with sirtf lon and lat in degrees.  */
/*    updated 27mar2002 to update comments.                                */

/*     fills structure sirtfdata for background estimator.                 */

/*  Call appropriate routine get_s_vals() (to get ephemeris from C spice or*/
/*   HORIZONS file) by linking appropriate file (get_cspi.o for cspice or  */
/*   get_s_va.o for HORIZONS).                                             */

#include <stdio.h>
#include <math.h>
#include "sirtfd_s.h"

#pragma ident "@(#) sirtfsun.c  27March2002."

int sirtfsun(jd, sirtfdata)
double jd;                     /* julian day number  (eg. 2452650.50000)  */
struct sirtfdata_s *sirtfdata;

  /* returns 0 for okay, returns -101 if jd not available, other < 0 for err*/
{
  static double d2r = -1.0;    /* will be set appropriately first time thru. */
  static double pi;
  int ierr;
  double lon, lat, r;
  double lonr, latr;
  double coslat, coslon, sinlat, sinlon;
  double lambda_solar_sirtf;
  double work;

  int get_s_vals(double jd, double *lon, double *lat, double *r);

/*   get sirtf heliocentric ecliptic lon and lat (deg) and distance (AU) */

  if ((ierr = get_s_vals(jd, &lon, &lat, &r)) != 0)
    return(ierr);


  if (d2r < 0.0) 
  {
    d2r = atan(1.0) / 45.0;
    pi = 180.0 * d2r;
  }

  sirtfdata->julian_day = jd;
  sirtfdata->lon_sirtf = lon;
  sirtfdata->lat_sirtf = lat;
  sirtfdata->r_sirtf   = r;

  lonr = lon * d2r;
  latr = lat * d2r;

  coslon = cos(lonr);
  coslat = cos(latr);
  sinlon = sin(lonr);
  sinlat = sin(latr);

  sirtfdata->xsirtf = r * coslat * coslon;
  sirtfdata->ysirtf = r * coslat * sinlon;
  sirtfdata->zsirtf = r * sinlat;

          /* the following 2 eqs. from IDL earthsun in zkernel_nopartial.pro */
          /*  (Bidushi Bhattacharya's feb2002 version) corrected for radian  */
          /*  usage only.                                                    */
          /*  Still may be wrong.  But presently (mar2002) the result is not */
          /*  used nor displayed by the background estimator. IDL only placed*/
          /*  the SolElong_sirtf into its COMMON SIRTF_PARAMS.               */
          /*  simplification indicates final result is acos(coslat * cos(-pi)).*/
          /*  Could that be what is really intended???                       */

  lambda_solar_sirtf = pi + lonr;
  work = coslat * cos(lonr - lambda_solar_sirtf);

  if ( work < -1.0)       /* prevent fatal errors due to roundoffs */
    work = -1.0;
  else if (work > +1.0)
    work = 1.0;

  sirtfdata->SolElong_sirtf = acos(work);

  return(0);
}
/* ----------------------------------------------------------------------- */
