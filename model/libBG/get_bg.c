/* get_bg.c   02-04June1999          j.bennett @ IPAC                       */
/*            19jun2002 to call earthsun with added argument, jd.           */
/*            25-28Mar2002 removed unused code at end and placed it in file */
/*             get_bg_pt2.c .Also updated comments and some code.           */
/*            18-20Mar2002 to remove references to get_bg_minmax (which will*/
/*                      no longer be used in this set of routines).         */
/*            14Mar2002 to update to changes in new IDL version             */
/*            14June1999  comments updated + update in get_bg_minmax.       */

/* Gets zody, interstellar medium, cosmic IR background, total background   */
/* given ra,dec,wl,day,year (using get_bg).                                 */

/* There are 3 user-entry points in the get_bg package:                     */
/*                                                                          */
/*1. int get_bg_init(char *pathname)  must be called once, before any other */
/*         calls to routines in this package so that the pathname to the    */
/*         Schlegel interstellar dust map FITS files is known.              */
/*         As of Feb2000, the pathname may be no longer than 232 chars.     */
/*         For SIRTF, as of 04Jun1999, the path is /proj/ssc/Data/IRAC/SFD  */
/*     A return code -1 indicates too long a pathname was specified.        */
/*                                                                          */
/* After the above initialization, calls to the following entry may be made:*/
/*                                                                          */
/*2. int get_bg(double ra, double dec, double wl, double day, int iyear,    */
/*             double *zody, double *ism, double *cib, double *totbg,       */
/*             int *ES_warn)                                                */
/*     Please see comments at beginning of get_bg() for more info on get_bg.*/
/*                                                                          */
/* After the user is finished with all calls to get_bg,                     */
/*                                                                          */
/*3. int get_bg_fini() must be called to release memory.                    */
/*     A return code of -1 indicates an error in deallocations of memory.   */
/*                                                                          */
/*                                                                          */
/*  GENERAL CONSTRAINTS FOR INPUTS to get_bg():                             */
/*   ra is in degrees and must be between 0.0 and 360.0 (inclusive).        */
/*   dec is in degrees and must be between -90.0 and +90.0 (inclusive).     */
/*   day (for input in get_bg) must be between 1.0 and 366.9999 (inclusive).*/
/*   year should be >= 2003. Day and year must be a date in the sirtf       */
/*     ephemeris.                                                           */
/*   wl is wavelength in microns and must be > 0.0 .                        */
/*     It is assumed that the caller will have checked, for valid ranges,   */
/*     the values of ra, dec, day, wl.  And at least check that year is     */
/*     >= 2003 . Max. year depends on what is available in the ephemeris    */
/*     file (cspice or HORIZONS, depending on method selected).             */


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "zdata_s.h"
#include "zfuncs_s.h"
#include "sirtfd_s.h"

#pragma ident "@(#) get_bg.c updated 19June2002."

#define EQU_J2000  0        /* jcnvc2 code for J2000 equatorial pos. */
#define EQU_YEAR    2000.0
#define ECL_J2000 13        /* jcnvc2 code for J2000 ecliptic pos,   */
#define ECL_YEAR    2000.0
#define GALACTIC   2        /* jcnvc2 code for J2000 galactic pos.   */
#define GAL_YEAR    1950.0

#define QINTERP  1     /*  1 for linearly interpolating value from nearest */
                       /*    four pixels.                                  */
#define QJMAP    0     /*  0 for Ebv FITS files.                           */


#define JD_OUT_OF_RANGE -100  /* for report of error from julday() if input */
                              /* year not between 1901 and 2099  (therefore */
                              /* julian day cannot be computed by julday()).*/

double SolElong_deg;   /* Caller may use extern double SolElong_deg   */
                       /*  to capture value when ES_warn is not zero. */

#define DEBUG 0

int jcnvc2(int sycod1, double eqx1, double lon1, double lat1,
	 int sycod2, double eqx2, double *lon2, double *lat2, double tobs);

static int julday(int year, double day, double *jd);

int earthsun(double day, int iyear, double elon, double elat, double jd,
	   struct zdata_s *zdata, struct zfuncs_s *zpos);

int sirtfsun(double jd, struct sirtfdata_s *sirtfdata);

int get_zmod(double lambda, struct zdata_s *zdata, struct zfuncs_s *zpos,
	   struct sirtfdata_s *sirtfdata, double *z);

int dust_getval_sub(double glon, double glat, int interp, int jmap, float *ans);

int ism_spec_mod(double wl, double *ans);  /* in modspec.c */
int cib_spec_mod(double wl, double *ans);  /* in modspec.c */

  /* ************************************************************************ */
  /* It is callers responsibility to ensure that ra in range of 0. thru 360., */
  /* dec in range of -90. thru +90., day is between 1.0 and 366.9, and that   */
  /* wl is > 0.  and year >= 2003                                             */
  /* ************************************************************************ */
int get_bg(
  double ra,     /* j2000 equatorial right ascension of position (dec. deg.) */
                 /*  0.0 thru 360.0 degrees.                                 */
  double dec,    /* j2000 equatorial declination of position (decimal deg.)  */
                 /*  -90.0 thru +90.0 degrees.                               */
  double wl,     /* wavelength in microns, must be > 0.0                     */
  double day,    /* day no. in range 1.0 thru 365.9999 (or 366.9999 for leap)*/
  int iyear,     /* integer year (>= 2003 )  Max. year depends on what is    */
                 /*  available in the ephemeris file (cspice or HORIZONS,    */
                 /*  depending on the implementation being used).            */
                  /*  OUTPUTS:                              */
  double *zody,     /* zodiacal light in MJy/sr             */
  double *ism,      /* interstellar medium in MJy/sr        */
  double *cib,      /* cosmic infrared background in MJy/sr */                         
  double *totbg,    /* total background in MJy/sr which is  */
                    /*   the sum of the preceding 3 values. */
  int    *ES_warn)  /* warning from func earthsun; +1=warn, 0=no warning */
                    /* warning occurs if |Solar elongation computed by   */
                    /* earthsun()| > 20.1 degrees.                       */

     /* WARNING: The function get_bg_init() must be called, once, before */
     /*          any calls are made to this function.                    */
     /*    Also: If the cspice library is used for the sirtf ephemeris   */
     /*          (i.e. not HORIZONS file input), Booth Hartley's         */
     /*          load_kernels() function must be called before calls to  */
     /*          get_bg() are made.                                      */

     /* Note: cib not to be displayed unless cib > (1.0e-04 * (zody + ism)) */
     /* When ES_warn != 0, value of SolElong_deg may be output if caller    */
     /*   uses extern double SolElong_deg; and prints its value.            */

     /* Return code = 0 for normal return. Non-zero if error occurs.       */
     /* The return codes -100 &-101 imply something the user can maybe     */
     /* remedy if he did a typo on the input day and/or year.              */
     /*  -100 if input year not between 1901 and 2099 and therefore julday */
     /*         routine cannot compute julian day number.                  */
     /*  -101 if julian day not in the ephermeris file (either cspice or   */
     /*         HORIZONS depending on which version of get_s_vals() is     */
     /*         linked).  Note: if cspice, the error has not been          */
     /*         cleared and caller may find exact error from the cspice    */
     /*         routines. If processing is to continue, user must clear    */
     /*         the cspice internal error flag and try another date.       */
     /*  -102 thru -108 N.A. for use of cspice ephemeris.  Reserved for use*/
     /*         with HORIZONS file reading.                                */
     /* No messages are output on stderr with return codes -100 & -101.    */
     /* Codes other than 0 and -100 and  -101 imply problems not remedial  */
     /*     by the user. In most cases codes other than 0,-100 and -101 are*/
     /*     accompanied with message(s) on stderr.                         */
     /*   Note: No valid data is returned if the return code is non-zero.  */

{
  static double rtod = -1;   /* will be set first time through this func.  */

  extern int sirtf_eph_flavor;  /* value available after first call to      */
                                /* get_s_vals().  0 indicates program linked*/
                                /* with get_s_vals that reads HORIZONS eph.,*/
                                /* 1 indicates that function uses cspice.   */

  struct zdata_s zdata;
  struct zfuncs_s zpos;
  struct sirtfdata_s sirtfdata;

  double glon, glat;
  double elon, elat;
  double jd;

  int    retcode;
  float  bg_ism_f;
  double z, bg_ism, bg_cib;
  double fac_ism, bg_total;



  if (rtod < 0.0)
    rtod = 45.0 / atan(1.0);

  /* check that wl > 0.0, if not,then set retcode = -2 and answers to zeroes */
  if (wl <= 0.0)
  {                /* This is supposed to be checked in caller.  Just  */
                   /*  a safety net here because zero would be major   */
                   /*  problem for these routines.                     */

    retcode = -2;
    *zody = *ism = *cib = *totbg = 0.0;
    fprintf(stderr,
      "get_bg(): Input ERROR:  wl = %20.4f; wl must be > 0.0 microns.\n", wl);
    return(retcode);
  }

  if (julday(iyear, day, &jd) != 0)
  {               /* out of range date */
    retcode = JD_OUT_OF_RANGE;
    *zody = *ism = *cib = *totbg = 0.0;
    return(retcode);
  }

  jcnvc2(EQU_J2000, EQU_YEAR,    ra,  dec,
         ECL_J2000, ECL_YEAR, &elon, &elat, 0.0);
  jcnvc2(EQU_J2000, EQU_YEAR,    ra,  dec,
         GALACTIC, GAL_YEAR, &glon, &glat, 0.0);
  *ES_warn = 0;

               /* earth sun fills zdata and zpos structs except for zdata.wl */
  if (earthsun(day, iyear, elon, elat, jd, &zdata, &zpos)  != 0)
    *ES_warn = -1;  /* will not occur now - earthsun now returns only zero. */

               /* test was 20.0, loosened to 20.1 09apr1999 */
  SolElong_deg = zdata.SolElong * rtod;
  if (fabs(SolElong_deg - 100.0) > 20.1)
    *ES_warn = +1;

                             /* sirtfsun fills the sirtfdata structure */
  if ((retcode = sirtfsun(jd, &sirtfdata)) != 0)
    return(retcode);  /* value of retcode is just passed thru sirtfsun from */
                      /*  get_s_vals().                                     */


  #if DEBUG
    fprintf(stdout,"\nday = %9.4f, iyear = %4d, wave_length = %10.2f um\n",
            day, iyear, wl);
    fprintf(stdout,"\n  ra        = %15.11f, dec         = %15.11f deg.\n",
            ra, dec);

    fprintf(stdout,"\nglon        = %15.11f, glat        = %15.11f deg.",
            glon,glat);
    fprintf(stdout,"\nelon(J2000) = %15.11f, elat(J2000) = %15.11f deg.\n",
            elon, elat);
    fprintf(stdout,"SolElong = %15.11f radians, Earth_Dis = %15.10f AU,\n"
            "Earth_Lon =%15.11f radians, Earth_Mean_Lon = %15.11f radians\n",
            zdata.SolElong, zdata.Re, zdata.Theta,
            zdata.Earth_Mean_Lon);

    fprintf(stdout,"\njulian_day  = %20.8f", sirtfdata.julian_day);
    fprintf(stdout,"\nsirtf_lon   = %15.11f, sirtf_lat   = %15.11f deg.\n",
            sirtfdata.lon_sirtf, sirtfdata.lat_sirtf);
    fprintf(stdout,"\nr_sirtf = %15.10f AU, SolElong_sirtf = %15.11f radians",
            sirtfdata.r_sirtf, sirtfdata.SolElong_sirtf);
    fprintf(stdout,"\nxsirtf, ysirtf, zsirtf = %15.10f,%15.10f,%15.10f\n",
            sirtfdata.xsirtf, sirtfdata.ysirtf, sirtfdata.zsirtf);
  #endif


  get_zmod( wl, &zdata, &zpos, &sirtfdata, &z);

  if ((retcode =
       dust_getval_sub(glon, glat, QINTERP, QJMAP, &bg_ism_f)) != 0)
    bg_ism = 0.0;  /*retcode will be retained and returned to caller*/
  else
  {
    ism_spec_mod(wl, &fac_ism);
    bg_ism = (double) bg_ism_f / 0.016 * fac_ism;

  #if DEBUG
    fprintf(stdout,"bg_ism_f from dust_getval_sub()= %20.6f\n", bg_ism_f);
    fprintf(stdout,"fac_ism from ism_spec_mod()    = %20.8f\n", fac_ism);
  #endif
  }

  cib_spec_mod(wl, &bg_cib);
  bg_total = z + bg_ism + bg_cib;

  *zody = z;
  *ism = bg_ism;
  *cib = bg_cib;
  *totbg = bg_total;
  return(retcode);
}
/* ------------------------------------------------------------------------ */

int get_bg_init(char *ipath)
{
  int retcode;
  int dust_getval_init(char *ipath);
                           /* as of feb2000, max ipath len is 232 chars. */
  retcode = dust_getval_init(ipath);     /* retcode -1 if ipath too long */
  return(retcode);
}
/* ------------------------------------------------------------------------- */

int get_bg_fini()
{
  int retcode;
  int dust_getval_fini();

  retcode = dust_getval_fini();  /* retcode -1 if error dealloc. memory */
  return(retcode);
}
/* ------------------------------------------------------------------------- */
/* copy of j.bennett's julday() in static int version for this file.       */

/*  julday to compute Julian Day between years 1901 and 2099, inclusively, */
/*   from input year (1901 - 2099) and day within year (1.0 - 365.9999     */ 
/*                                 [366.9999for leap yrs])                 */

/*  J.Bennett @ IPAC 28Feb2002. 01Mar2002 to add function unjulday()       */

/*   Equation from Meeus, Jean,  1991, Astronomical Algorithms,            */
/*      Willmann-Bell Inc., p.62.                                          */

/*   The following are test cases with valid results:
  year, day, jd = 2002,   365.37916667,   2452639.87916667
  year, day, jd = 2003,    10.50,         2452650.00000000
  year, day, jd = 1988,    27.00,         2447187.50000000 (note: 1988 is leap
  year, day, jd = 1988,   171.50,         2447332.00000000  year)
*/


static int julday(int year, double day, double *jd)
/* Where:
   int year is Gregorian calendar year between 1901 and 2099.
   double day is day number within year (includes fraction for time of day 
                e.g. noon on Jan 10 (U.T.) is specified as  day 10.5).
              Valid values are 1 - 365.9999 ( 1 - 366.9999 for leap years).
   double *jd is returned julian day number.

 julday returns 0 if okay, -1 if input year out of range.
*/

{
  double ym1;

  if (year < 1901 || year > 2099)
    return(-1);                    /* cannot compute with this function */

        /* compute for jan 0.0 of year and add day no. within year */

  ym1 = (double) (year - 1);
  *jd = 1721409.5 + (int) (365.25 * ym1) + day;
  return(0);
}
/* ------------------------------------------------------------------------ */
