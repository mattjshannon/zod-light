/* get_zmod.c replaces IDL functions get_zmod, mk_zdata and                 */
/*    procedure zfunc and the table zpar_fd5.xdr (note zpar values here     */
/*    are from the xdr table mar2002 (may need updating in future?)         */

/*  04-08feb1999     j.bennett @ IPAC                                       */

/*  updated 14-15-mar2002 to revise arg.list and where structures are filled.*/
/*   structure zpos and most of zdata are filled in earthsun now.           */

/*  updated 05,06mar2002 to use new zpar array from new zpar_fd5.xdr and    */
/*      incorporate changes B.Bhattacharya made to IDL get_zmod.pro .           */

/*  updated 25mar1999 to call bginit only on 1st time through get_zmod.     */

/* COMMENTS from IDL GETZMOD:
*---------------------------------------------------------------------
* Function GET_ZMOD
*
* Function to compute the ZODI model flux for a given set of LOS, Wavelength, 
* and Time.
*
* Written By: 	BA Franz
* Date:		23 January 1995
* updated Dec 95 by Reach for current zkernel
* updated Aug 01 by Bhattacharya for SIRTF use
*
* Usage:
*     zodi = GET_ZMOD(lambda,day,year,lon,lat)
*
* Inputs:
*     lambda - wavelength(s) in um 
*     day    - day number(s), where 1.0 = Jan 1 00:00
*     year   - year
*     lon    - ecliptic longitude(s)
*     lat    - ecliptic latitude(s)
*
*     note: lambda, day, lon, lat can be any mixture of scalars or arrays,
*           but all arrays must be of same length.
*
* Outputs:
*
*     zodi - scalar or array of zodi model flux
*
* Optional Keyword Inputs:
*
*     zpar=zpar - array of zodi model params.  If not specified, the
*                 default will be restored from DBIDL.
*
* Modification History:
*
*       Include the year, so that SIRTF ephemeris data can be used to
*        determine observatory position.
*        -- B. Bhattacharya, SIRTF Science Center, 8/13/01
*---------------------------------------------------------------------
*/
/*  COMMENTS from IDL MK_ZDATA:
*---------------------------------------------------------------------
* Function MK_ZDATA
*
* Function to generat a zmodel input dataset for a set of LOS, Wavelength, 
* and Time.
*
* Written By: 	BA Franz
* Date:		23 January 1995
* updated Dec 95 by Reach for current zkernel
* updated Aug 01 by Bhattacharya for SIRTF use
*
* Usage:
*     zdata = MK_ZDATA(lambda,day,year,lon,lat)
*
* Inputs:
*     lambda - wavelength(s) in um 
*     day    - day number(s), where 1.0 = Jan 1 00:00
*     year   - year
*     lon    - ecliptic longitude(s)
*     lat    - ecliptic latitude(s)
*
*     note: lambda, day, lon, lat can be any mixture of scalars or arrays,
*           but all arrays must be of same length.
*
* Outputs:
*
*     zdata - data structure for input into ZMODEL or one of its kernel
*             functions (ZFUNC, ZTFUNC, ZPCFUNC, DZDTFUNC)
*
*     where:    
*         zdata(i).wave_len   = input wavelength (um)
*         zdata(i).longitude  = input ecliptic longitude (deg)
*         zdata(i).latitude   = input ecliptic latitude (deg)
*         zdata(i).day        = input day number (1.0 = 1 Jan  00:00)
*         zdata(i).year       = input year
*
* Modification History:
*
*       Include the year, so that SIRTF ephemeris data can be used to
*        determine observatory position.
*       -- B. Bhattacharya, SIRTF Science Center, 8/13/01
*
*---------------------------------------------------------------------
*/
/* COMMENTS from IDL function ZFUNC.PRO (that is in zmodel.pro file)
*---------------------------------------------------------------------
* IDL Function ZFUNC.PRO
* Calculates the zodiacal dust model (unmodified).
*
* Written By: BA Franz, ARC
*
*---------------------------------------------------------------------
*/
#include <stdio.h>
#include <math.h>
#include "zdata_s.h"
#include "zfuncs_s.h"
#include "sirtfd_s.h"
#include "ztrig_s.h"

#define  NZPAR  256     /* no. of elements in zpar array */

#pragma ident "@(#) get_zmod.c updated 15March2002."

struct ztrig_s ztrig;

int get_zmod(lambda, zdata, zpos, sirtfdata, z)
double lambda;                 /* microns  */
struct zdata_s *zdata;
struct zfuncs_s *zpos;
struct sirtfdata_s *sirtfdata;
double *z;                     /* MJy/sr   */
{
   /* zpar values from IDL file zpar_fd5.xdr                                */
   /* WARNING: the C version of the background estimator assumes that       */
   /*           zpar[0] continues to have the value of 2.0 (for the IDL     */
   /*           version, this was value of FuncIndx that directs func.      */
   /*           zcloud() to use Widened Modified Fan Density Function which */
   /*           is the only one coded into the C-version of zcloud).        */

  double zpar[NZPAR] =
  {    2.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,       286.00000,       0.0000000,
       0.0000000,      0.43579335,   1.1300000E-07,       1.3400000,
       4.1399999,      0.94199997,      0.18900000,       0.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,       0.0000000,
       0.0000000,       0.0000000,       0.0000000,       0.0000000,
       77.699997,       2.0300000,     0.011900000,    0.0054799998,
   -0.0021500001,      0.35717684,      0.46477734,      0.28495831,
       0.0000000,       2.3421700,       1.0555413,       1.0111800,
       1.0000000,      0.73307112,      0.64202173,      0.74837976,
      0.68726869,   5.5900001E-10,       8.7799997,       1.5000000,
       3.0000000,      0.10000000,       0.0000000,       4.0000000,
       0.0000000,       0.0000000,       80.000000,      0.56000000,
       0.0000000,       0.0000000,       0.0000000,      0.35717684,
      0.46477734,      0.28495831,       0.0000000,       2.3421700,
       1.0555413,       1.0111800,       1.0000000,      0.73307112,
      0.64202173,      0.74837976,      0.68726869,   1.9899999E-09,
       1.9900000,      0.94000000,       3.0000000,      0.89999998,
       0.0000000,       4.0000000,       0.0000000,       0.0000000,
       30.299999,       1.2000000,       0.0000000,       0.0000000,
       0.0000000,      0.35717684,      0.46477734,      0.28495831,
       0.0000000,       2.3421700,       1.0555413,       1.0111800,
       1.0000000,      0.73307112,      0.64202173,      0.74837976,
      0.68726869,   1.4400001E-10,       15.000000,       1.5000000,
       3.0000000,     0.050000001,       0.0000000,       4.0000000,
       0.0000000,       0.0000000,       80.000000,      0.80000001,
       0.0000000,       0.0000000,       0.0000000,      0.35717684,
      0.46477734,      0.28495831,       0.0000000,       2.3421700,
       1.0555413,       1.0111800,       1.0000000,      0.73307112,
      0.64202173,      0.74837976,      0.68726869,       0.0000000,
       0.0000000,       0.0000000,       0.0000000,       0.0000000,
       0.0000000,       0.0000000,       0.0000000,       0.0000000,
       0.0000000,       0.0000000,       0.0000000,       0.0000000,
       0.0000000,      0.35717684,      0.46477734,      0.28495831,
       0.0000000,       2.3421700,       1.0555413,       1.0111800,
       1.0000000,      0.73307112,      0.64202173,      0.74837976,
      0.68726869,   1.8300000E-08,       1.0300000,     0.025000000,
     0.054000001,       0.0000000,       0.0000000,       0.0000000,
       0.0000000,       0.0000000,       0.0000000,   1.9000000E-08,
       1.0599999,      0.10000000,      -10.000000,       12.100000,
     0.090999998,       22.299999,     0.048999999,       0.0000000,
       0.0000000,       0.0000000,      0.35717684,      0.46477734,
      0.28495831,       0.0000000,       2.3421700,       1.0555413,
       1.0111800,       1.0000000,      0.73307112,      0.64202173,
      0.74837976,      0.68726869,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000,
      -1.0000000,      -1.0000000,      -1.0000000,      -1.0000000 };

  static int nthru = 0;

  int bg_init();
  int zkernel(double zpar[], int num, struct zdata_s *zdata,
              struct zfuncs_s *zpos, struct sirtfdata_s *sirtfdata,
              double *zody);

  if (nthru == 0)
  {
    bg_init();           /* fills struct ztrig_s ztrig with rtod,pi,etc.  */
    nthru = 1;
  }
             /* Note: rest of zdata struct was filled in earthsun()     */
  zdata->wave_len  = lambda;               /* microns */

  zkernel(zpar, NZPAR, &(*zdata), &(*zpos), &(*sirtfdata), &(*z));
  return(0);
}
/* ------------------------------------------------------------------------ */
