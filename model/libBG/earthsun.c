/* ----------------------------------------------------------------------- */
/*  earthsun.c a C version of an IDL routine                               */
/*    converted from IDL to C by j.bennett, 03feb1999                      */
/*    26mar1999 - minor cleanup.                                           */
/*    11mar2002 - changes to some constants to agree with B.Bhattacharya's */
/*           revised IDL version.                                          */
/*     Note: the new IDL version computes lambda_solar_sirtf and           */
/*           SolElong_sirtf which do not seem to be  used.                 */
/*           If ever reinstall, put in dif. routine since values in this   */
/*           routine not used in _sirtf ones.                              */
/*    14Mar2002 - to change arg. list, fill structure zpos and most of     */
/*                structure zdata in this function.                        */
/*    19Mar2002 - add #pragma ident and update some comments.              */
/*    19Jun2002 - added arg. jd, changed computation of Earth_Mean_Lon     */

/*   comments from the IDL version:                                        */
/*
* Procedure EARTHSUN
*
* Returns Solar Elongation and Earth Position from Input Day and Lon, Lat
*
* Written By:  BA Franz, WT Reach
*
* Inputs:
*     Day   - Day Number (1 = 01 Jan)
*     Lon   - Ecliptic Lon of Line-of-Sight (Deg)
*     Lat   - Ecliptic Lat of Line-of-Sight (Deg)
*
* Modification History:
*     Update the relations for lambda_solar, mean_anomally, and
*     earth_mean_lon to reflect equations in the 2001 Astronomical
*     Almanac, p. C24.
*     -B.Bhattacharya, SIRTF Science Center, 6/26/01
*       Derive SIRTF's lambda_solar and solar elongation,BB,SSC,9/7/01
*-------------------------------------------------------------------------------
*/
/*  See sirtfsun() in sirtfsun.c for SIRTF's parms in C-version.           */

#include <stdio.h>
#include <math.h>

#include "zdata_s.h"
#include "zfuncs_s.h"

#pragma ident "@(#) earthsun.c updated 19June2002."

#define DMOD(RJ,CJ) ( (RJ) - (int) ((RJ)/(CJ)) * (CJ))
#define DSIGN(RJ,CJ) (((CJ) < (0)) ? (-fabs(RJ)) : fabs(RJ))

#define JD_REF  2452649.21458333  /* julday for 09jan03 for ref. lon of Earth*/
 

/* Return code 0 only.                                                     */

int earthsun(day, iyear, lon, lat, jd, zdata, zpos)
double day;             /* day number */
int iyear;              /* 4-digit year (e.g. 2003) to fill zdata->year .*/
double lon, lat;        /* ecliptic longitude and latitide in degrees */
double jd;              /* julian day for day,iyear  */
struct zdata_s *zdata;
struct zfuncs_s *zpos;
{
  static int nthru = 0;
  static double pi;
  static double pi2;
  static double d2r;
  static double eccen = 0.01671254;
  double work;
  double lambda_solar;
  double mean_anomaly;
  double SolElong;       /* radians */
  double Earth_Dis;      /* AU      */
  double Earth_Lon;      /* radians */
  double Earth_Mean_Lon; /* radians */
  int retcode;

  if (nthru == 0)
  {
    d2r = atan(1.0) / 45.0;
    pi  = 180.0 * d2r;
    pi2 = 2.0 * pi;
    nthru = 1;
  }

  retcode = 0;

  lambda_solar = (-80.598349 + 0.98564736 * day +
	       1.915 * cos(pi2 / 365.25 * (day - 94.8))) * d2r;

  work         =     (357.528    + 0.98560028 * day) * d2r;
  mean_anomaly = DMOD(work, pi2);


  Earth_Dis = (1.0 - eccen * eccen) /
               (1.0 + eccen * cos(mean_anomaly));          /* (AU)  */

  Earth_Lon = - DMOD((pi - lambda_solar), pi2);           /* (rad) */

  work       =     cos(lat * d2r) * cos(lon * d2r - lambda_solar);

  if ( work < -1.0 || work > +1.0)  /* handle computer roundoffs           */
    work = DSIGN(1.0, work);

  SolElong  = acos( work);

/* ;Earth_Mean_Lon = ((100.460 + 0.98564736 * day) * d2r) mod pi2
   ;use a reference longitude of the Earth on SIRTF's launch day, 1/9/03
   ;this should remove the "kink" in plots of zody vs. time for length
   ;of mission that were due to the equation above not accounting for
   ;the 0.25 days/year that contribute to leap years, BB, 5/9/02
*/
  work           = (108.9896   + 0.98564736 * (jd - JD_REF)) * d2r;
  Earth_Mean_Lon = DMOD(work, pi2);

  /*  Now fill structures:                                                 */
            /*  note: zdata->wave_len is filled in func get_zmod()   */
  zdata->day       = day;
  zdata->year      = (double) iyear;
  zdata->longitude = lon;                  /* degrees */
  zdata->latitude  = lat;                  /* degrees */
  zdata->SolElong  = SolElong;             /* radians */
  zdata->Re        = Earth_Dis;            /*  AU     */
  zdata->Theta     = Earth_Lon;            /* radians */
  zdata->Earth_Mean_Lon = Earth_Mean_Lon;  /* radians */

  zpos->elonr      = zdata->longitude * d2r;
  zpos->elatr      = zdata->latitude  * d2r;
  zpos->cos_elon   = cos(zpos->elonr);
  zpos->sin_elon   = sin(zpos->elonr);
  zpos->cos_elat   = cos(zpos->elatr);
  zpos->sin_elat   = sin(zpos->elatr);
  zpos->cos_theta  = cos(zdata->Theta);
  zpos->sin_theta  = sin(zdata->Theta);

  return(retcode);
}
/* ----------------------------------------------------------------------- */

