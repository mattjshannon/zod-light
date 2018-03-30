/*   zdata_s.h      08feb1999     j.bennett @ IPAC                        */

/*    updated 06Mar2002 to add year to structure to conform to changes made */
/*      to the IDL code by B. Bhattacharya in Aug2001.                      */

#pragma ident "@(#) zdata_s.h updated 06March2002."

/*C-version of "Required information" part of ZODI_FIT struct in zodi_str.pro*/
/*    + 4 extra items (that appear in COMMON EARTH_PARAMS in the new IDL     */
/*    version) computed in earthsun)                                         */

struct zdata_s
{
  double wave_len;              /* wavelength in microns                  */
  double day;                   /* day no.                                */
  double year;                  /* year (from 4-digit integer,e.g. 2003.0)*/
  double longitude;             /* ecliptic longitude (degrees)           */
  double latitude;              /* ecliptic latitude (degrees)            */
  double SolElong;              /* solar elongation in radians            */
  double Re;                    /* Earth_Dis  (AU)                        */
  double Theta;                 /* Earth_Lon  (radians)                   */
  double Earth_Mean_Lon;        /* Earth mean longitude (radians)         */
};
