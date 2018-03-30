#include <stdio.h>
#include <math.h>
// #include <values.h>
#include <limits.h>
// #include <float.h>
#include "SpiceUsr.h"

/* prototypes */
int background_elat(double jd, double *elon, double *elat, double *range);

char *observer;

#define DEBUG 0

int background_elat(jd, elon, elat, rangeAU)
double jd;
double *elon, *elat;
double *rangeAU;
{
#define                 WORD_SIZE  48

   SpiceDouble             et;
   SpiceDouble             lt1;
   SpiceDouble             state1 [6];

   SpiceChar               utc  [WORD_SIZE];
   SpiceChar*              corr;
   SpiceChar*              ref_ecliptic;

#define UTCOUT_LENGTH 30
   SpiceChar               utcout[UTCOUT_LENGTH];
#define JDOUT_LENGTH 30
   SpiceChar               jdout[JDOUT_LENGTH];
   SpiceDouble             ra, dec, range;


   /*
   Set the reference frame  ECLIPJ2000.  
   We need an aberration correction.  "LT+S", light time plus stellar
   aberration, is normally used, but for background we use NONE.
   */

   /* corr   = "LT+S";  */
   corr   = "NONE";
   ref_ecliptic    = "ECLIPJ2000";

    sprintf(utc, "JD %14.6f", jd);

   /* Convert the time string to ephemeris time J2000. */
    str2et_c ( utc, &et );

    /* Compute the state of SIRTF (-79) from sun  at time et  */
    spkezr_c (  observer, et, ref_ecliptic, corr, "sun", state1, &lt1 );
    if (failed_c())
	return (1);

    /* convert to lon, lat, range */
    recrad_c ( state1,  &range,      elon,     elat );

    /* change range from KM to AU */
    convrt_c ( range, "KM", "AU", rangeAU);

    /* change range from radians to deg */
    *elon = *elon * dpr_c();
    *elat = *elat * dpr_c();



   if (DEBUG)
   {
       /* Output */
       et2utc_c ( et ,   "C",  3, UTCOUT_LENGTH, utcout );
       et2utc_c ( et ,   "J",  3, JDOUT_LENGTH, jdout );

       printf("%s %s  %8.4f %6.4f %f\n", 
	   utcout, jdout, 
	   *elon, *elat, *rangeAU);
   }
   return (0);

}
