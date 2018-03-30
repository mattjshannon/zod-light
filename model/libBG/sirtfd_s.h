/*  sirtfd_s.h    14Mar2002    j.bennett @ IPAC                             */
/*    For new C version of new IDL background estimator.                    */

#pragma ident "@(#) sirtfd_s.h  14Mar2002."

struct sirtfdata_s
{
        /*  first 4 quantities from CSPICE or a Horizons input file */
        /*  last 4 quantities computed in sirtfsun()                */
  double julian_day;   /* julian day computed from input year and day.     */
  double lon_sirtf;    /* heliocentric ecliptic lon (degrees) of sirtf s/c.*/
  double lat_sirtf;    /* heliocentric ecliptic lat (degrees) of sirtf s/c.*/
  double r_sirtf;      /* distance (AU) for s/c.                           */
  double xsirtf;       /* X computed from Cspice/Horizons values.          */
  double ysirtf;       /* Y computed from Cspice/Horizons values.          */
  double zsirtf;       /* Z computed from Cspice/Horizons values.          */
  double SolElong_sirtf;  /* radians. Computed from above values, but does */
                          /* not seem to be used at present.               */
};
/* ------------------------------------------------------------------------- */
