/*  get_cspi.c - get_s_vals() for get_bg that will use Java interface.      */
/*    j.bennett @ IPAC                                                      */
/*  this version (in get_cspi.c) get_s_vals uses booth's cspice routine     */
/*     instead of reading HORIZONS table.                                   */
/*   Note: load_kernels() must be called once before this routine is called.*/
/*  updated 25mar2002 to use int version of background_elat.                */

#include <stdio.h>
#include <math.h>

#pragma ident "@(#) get_s_vals() in get_cspi.c  25mar2002."

int sirtf_eph_flavor = 1;  /* indicates routine uses cspice (not HORIZON file)*/
                           /*   ephemeris file.                               */


int get_s_vals(double jd, double *lon, double *lat, double *r)
{
  /*  uses cspice lib instead of HORIZONS table for ephemeris.       */
  /*  calls booth's routine, background_elat.  Note: load_kernels    */
  /*  must be called once before calling background_elat (therefore, */
  /*  before calling get_s_vals()).                                  */

#define JD_NOT_AVAILABLE  -101

  int retcode;
  int background_elat(double jd, double *elon, double *elat, double *range);

  retcode = 0;

         /* assumes non-zero ret.code means julian day not found in cspice */
         /*   ephemeris file. Caller will call compose_error_message and   */
         /*   display real error when retcode not zero.                    */

  if (background_elat(jd, &(*lon), &(*lat), &(*r)) != 0)
    retcode = JD_NOT_AVAILABLE;
  return(retcode);
}
/* ----------------------------------------------------------------------- */
