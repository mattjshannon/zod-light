/* dust_getval_sub.c  modification of dust_getval_serv.c for subroutine.    */

/*   10dec1999 to make dust_getval_fini more technically correct.           */
/*    --definitely will not hurt anything, may help multiple fini call prob.*/
/*   07,09June199  comments updated.                                        */
/*   28May,01June1999, J.Bennett @ IPAC - subroutine mods                   */
/*    server  mods done 20-23,30apr1999  j.bennett @ IPAC                   */

/*  updated 25may1999 to include subs_lambert_serv.h instead of             */
/*       subs_lambert.h and to move func subs_getval_serv to file           */
/*       subs_lambert_serv.c. Also removed include for subs_asciifile.h.    */
/*  updated 24may1999 to remove include for subs_common_string.h            */
/*  updated 11may1999 to output with %13.6f instead of %12.5f - for better  */
/*        agreement with the IDL version usage.                             */
/*   26,30Apr1999  comments updated.                                        */

/* SUBROUTINE usage -- there are 4 user-entry points-of which 3 MUST be used
 *     if any are used.
 *   Note: for more detail on arguments and return codes please see comments
 *         in the function code for each function in this file.
 *
 * 1.  int dust_getval_init(char * filenam_north, char * filenam_south)
 *        must be called once, before any other calls to routines in this
 *        package so pathname to the Schlegel interstellar dust map FITS
 *        files is known.
 *  After the above initialization, calls to the following entries may be made.
 *
 * 2.  int dust_getval_sub(double gallon, double gallat, int interp, int imap,
 *                         float *ans)
 *         For more detail on arguments and return codes please see comments
 *         in the function code in this file.
 *         Please note: ans is a float (this is so it may hold a bitstring
 *                      appropriately when mask values are desire with imap=4.
 *                      ans is always passed by address so float is maintained.
 *         Note: for NED,  interp = 1, imap = 0 (to use Ebv map) is desired.
 *
 *   After the user is finished with all calls to dust_getval_sub,
 *
 * 3.  int dust_getval_fini( )  MUST be called to release memory.
 *           This routine may be called at any time to free memory for 
 *           a while before calling this package again.  To resume 
 *           processing just call dust_getval_sub again
 *           (no need to call dust_getval_ini again).
 *
 *   EXAMPLE:
 *    double gallon, gallat;
 *    float ans;
 *    int interp = 1;
 *    int imap   = 0;     -- use the Ebv map --
 *    char ipath[] = "/proj/ssc/Data/IRAC/SFD";
 *    int retcode;
 *      retcode = dust_getval_init( ipath );
 *    gallon = 121.0;
 *    gallat = -21.5;
 *      retcode = dust_getval_sub( gallon, gallat, interp, imap, &ans);
 *        -- The following will be returned in ans: 0.34226 --
 *    gallon =  12.0;
 *    gallat = +34.5;
 *      retcode = dust_getval_sub( gallon, gallat, interp, imap, &ans);
 *        -- The following will be returned in ans: 0.12773 --
 * -- Release memory before exitting --
 *      retcode = dust_getval_fini();
 *
 * Optional routine to be used only if imap=4 in dust_getval_sub call:
 * 4. void trans_mask_bits(float *ans, int *hcons, mask_chars[6][8])
 *      will translate hcon and mask bits returned in the float *ans returned
 *      by dust_getval_sub when imap=4 (for the mask value map).
 *   Example (use dust_getval_init and dust_getval_fini as in prev. example):
 *    double gallon, gallat;
 *    float ans;
 *    int interp, imap, hcons;
 *    char mask_chars[6][8];   -- 6 8-char strings (incl. term. nulls) --
 * -- Where gallon = 121.0, gallat = -21.5, interp=1, imap = 4 to find ans:--
 * --            (interp is ignored when imap = 4)                         --
 *      retcode = dust_getval_sub(gallon, gallat, interp, imap, &ans);
 *      trans_mask_bits( &ans, &hcons, mask_chars);
 *
 * The following will be returned in hcons and mask[0-5]:
 *   hcons = 1, mask[0-5] =OK     OK     OK     OK     big_obj OK
 *
*/ 

/*   comments from Schlegel's original dust_getval:
 *    (note: infile,outfile,verbose not accessible thru this subroutine form)
 * NAME:
 *   dust_getval
 *
 * PURPOSE:
 *   Read values from BH files or our dust maps.
 *
 *   Either the coordinates "gall" and "galb" must be set, or these coordinates
 *   must exist in the file "infile".  Output is written to standard output
 *   or the file "outfile".
 *
 * CALLING SEQUENCE:
 *   dust_getval gall galb map=map ipath=ipath interp=interp \
 *    infile=infile outfile=outfile 
 *
 * OPTIONAL INPUTS:
 *   gall:       Galactic longitude(s) in degrees
 *   galb:       Galactic latitude(s) in degrees
 *   map:        Set to one of the following (default is "Ebv"):
 *               I100: 100-micron map in MJy/Sr
 *               X   : X-map, temperature-correction factor
 *               T   : Temperature map in degrees Kelvin for n=2 emissivity
 *               Ebv : E(B-V) in magnitudes
 *               mask: Mask values
 *   infile:     If set, then read "gall" and "galb" from this file
 *   outfile:    If set, then write results to this file
 *   interp:     Set this flag to "y" to return a linearly interpolated value
 *               from the 4 nearest pixels.
 *               This is disabled if map=='mask'.
 *   verbose:    Set this flag to "y" for verbose output, printing pixel
 *               coordinates and map values
 *   ipath:      Path name for dust maps; default to path set by the
 *               environment variable $DUST_DIR/map, or to the current
 *               directory.
 *
 * EXAMPLES:
 *   Read the reddening value E(B-V) at Galactic (l,b)=(12,+34.5),
 *   interpolating from the nearest 4 pixels, and output to the screen:
 *   % dust_getval 12 34.5 interp=y
 *
 *   Read the temperature map at positions listed in the file "dave.in",
 *   interpolating from the nearest 4 pixels, and output to file "dave.out".
 *   The path name for the temperature maps is "/u/schlegel/".
 *   % dust_getval map=T ipath=/u/schlegel/ interp=y \
 *     infile=dave.in outfile=dave.out 
 *
 * REVISION HISTORY:
 *   Written by D. Schlegel, 19 Jan 1998, Durham
 *   5-AUG-1998 Modified by DJS to read a default path from an environment
 *              variable $DUST_DIR/map.
 */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "subs_inoutput.h"
#include "subs_fits.h"
#include "subs_memory.h"
#include "subs_lambert_serv.h"

                      /* globals for this file only */
static int lastmap = -1;
static char pDefPath[] = "./";

static char pathn[MAX_FILE_NAME_LEN] = "\0"; /* dim. too big, but better    */
                                             /* that than too small. Already*/
                                             /* have MAX.. defined.         */
   /* Declarations for data file names */
static char     pFileN[MAX_FILE_NAME_LEN];
static char     pFileS[MAX_FILE_NAME_LEN];


/******************************************************************************/
int dust_getval_sub
  (double   tmpl,    /* gallon */
   double   tmpb,    /* gallat */
   int      interp,  /* 0=do not interpolate. Anything but 0, do interpolate*/
   int      jmap,    /* 0=Ebv,1=I100,2=X,3=Y,4=mask. <0 and >4 default to Ebv*/
   float   *ans)     /* If jmap=0,ans is Ebv value. If jmap=4,ans is mask */
                     /* to be translated by function trans_mask_bits.     */
      /*  Return codes:  0 = normal return, -9 = could not find FITS files*/
      /*                -1 = error in glon or glat range.                 */
{   
 extern int lastmap;
 extern char pathn[MAX_FILE_NAME_LEN];
 extern char pFileN[MAX_FILE_NAME_LEN];
 extern char pFileS[MAX_FILE_NAME_LEN];

   int      imap;
   int      qInterp;
   int      qVerbose;
   int      nGal;
   float    pGall[2];   /* only 1st element used */
   float    pGalb[2];   /* only 1st element used */
   float    Mapval;



   struct   mapParms {
      char *   pName;
      char *   pFile1;
      char *   pFile2;
   } ppMapAll[] = {
     { "Ebv" , "SFD_dust_4096_ngp.fits", "SFD_dust_4096_sgp.fits" },
     { "I100", "SFD_i100_4096_ngp.fits", "SFD_i100_4096_sgp.fits" },
     { "X"   , "SFD_xmap_ngp.fits"     , "SFD_xmap_sgp.fits"      },
     { "T"   , "SFD_temp_ngp.fits"     , "SFD_temp_sgp.fits"      },
     { "mask", "SFD_mask_4096_ngp.fits", "SFD_mask_4096_sgp.fits" }
   };

  imap = jmap;

  qInterp = 1; /* do interpolation--this is different from orig. dust_getval*/
  qVerbose = 0; /* not verbose */

  if (imap < 0 || imap > 4)
    imap = 0;                   /*  default to Ebv   */
  if (interp == 0)
    qInterp = interp;

  if (imap == 4)
    qInterp = 0;   /* Disable interpolation if reading the mask */

  nGal = 1;

  if (imap != lastmap)
  {
    if (pathn[0] == '\0')
      strcpy(pathn, pDefPath);

              /* Determine the file names to use */
    sprintf(pFileN, "%s/%s",  pathn, ppMapAll[imap].pFile1);
    sprintf(pFileS, "%s/%s",  pathn, ppMapAll[imap].pFile2);
    lastmap = imap;

  /* check for existence of files, since return codes do not seem to be */
  /*        checked elsewhere.  Test added 23Apr1999; j.bennett         */

    if (inoutput_file_exist(pFileN) != IO_GOOD )
    {
      fprintf(stderr,
              "dust_getval_sub: ERROR -  file <%s> not found.\n",pFileN);
      fflush(stderr);
      return(-9);
    }
    if (inoutput_file_exist(pFileS) != IO_GOOD )
    {
      fprintf(stderr,
              "dust_getval_sub: ERROR -  file <%s> not found.\n",pFileS);
      fflush(stderr);
      return(-9);
    }
  }

  pGall[0] = (float) tmpl;
  pGalb[0] = (float) tmpb;

  if (pGall[0] <   0.0 || pGall[0] > 360.0 ||
      pGalb[0] < -90.0 || pGalb[0] > +90.0)
  {
    fprintf(stderr,"ERROR in glon and/or glat\n");
    fflush(stderr);
    return(-1);
  }
              /* Read values from FITS files in Lambert projection */
  Mapval = lambert_getval_serv(pFileN, pFileS, nGal, pGall, pGalb,
              qInterp, qVerbose);

  *ans = Mapval;
  return(0);
}
/* ------------------------------------------------------------------------ */

int dust_getval_init(char *pathin)
  /* receives pathname to prepend to canned fits file names. */
{
  extern char pathn[MAX_FILE_NAME_LEN];
  extern int  lastmap;
  int diff;
  int dust_getval_fini();

  if (lastmap >= 0)
  {
    if (strcmp(pathin,pathn) == 0)
      return(0);                      /* same path as before */
    dust_getval_fini();               /* new, so clean up old first */
    lastmap = -1;          /* set so path will be reset in dust_getval_sub  */
  }

  if (strlen(pathin) < (size_t) (MAX_FILE_NAME_LEN - 23))
  {
    strcpy(pathn, pathin);
    return(0);
  }
  else
  {
    strcpy(pathn,pDefPath);
    diff = MAX_FILE_NAME_LEN - 23;
    fprintf(stderr,
     "dust_getval_init: ERROR - pathname (%s) >= %d chars (too long)\n",
      pathin, diff);

       /*(MAX_FILE_NAME_LEN - SFD_dust_4096_ngp.fits - 1)=(80 - 22  -1)= 57 */
    fflush(stderr);
    return(-1);
  }
}
/* ------------------------------------------------------------------------ */

int dust_getval_fini()
          /* close up shop - free memory, reset lastmap */
{
  extern int lastmap;
  extern char pFileN[MAX_FILE_NAME_LEN];
  extern char pFileS[MAX_FILE_NAME_LEN];

  float dummy1[2], dummy2[2];
  float Ans;
  dummy1[0] = dummy1[1] = dummy2[0] = dummy2[1] = 0.0;

     /* clean up allocated arrays in lambert_getval_serv  */
  Ans = lambert_getval_serv(pFileN, pFileS, 0, dummy1, dummy2, 0, 0);
  lastmap = -1;
  if (Ans == (float) 0.0)
    return(0);
  else
  {
    fprintf(stderr,"dust_getval_fini: ERROR - deallocating memory.\n");
    fflush(stderr);
    lastmap = -1;
    return(-1);
  }
}

/* ------------------------------------------------------------------------ */
/*     translate mask bits that are embedded in Mapval                      */
void trans_mask_bits(
  float *Mapval,
  int   *hcons,
  char  mask_chars[][8])    /* dim. [6][8] in caller */

     /*   sample for caller to use to output hcons and mask_chars       */
     /*       with original Schlegel dust_getval.c format:              */
     /*           Mapval is answer from dust_getval_sub when map = mask */
     /*           Mapval address is passed so that no promotion to double*/
     /*           occurs.                                               */
     /*  float *Mapval;                                                 */
     /*  int hcons, ii;                                                 */
     /*  char mask_chars[6][8];                                         */
     /*                                                                 */
     /*  trans_mask_bits(&Mapval, &hcons, mask_chars);                   */
     /*  fprintf(stdout,   "MASK:  %1dhcons", hcons);                   */
     /*  for (ii=0; ii < 6; ii++) {                                     */
     /*     fprintf(stdout,   " %7s", mask_chars[ii]);                  */
     /*  fprintf(stdout,   "\n");                                       */

{
  int      bitval[8];
  int      ii;

   /* Bit mask string values */
  char ppBitname[][8] = {
   "       " , "       ",
   "       " , "       ",
   "OK     " , "asteroi",
   "OK     " , "glitch ",
   "OK     " , "source ",
   "OK     " , "no_list",
   "OK     " , "big_obj",
   "OK     " , "no_IRAS" };


   for (ii=0; ii < 8; ii++)
     bitval[ii] = ( (int) *Mapval & (int)pow(2,ii) ) > 0;
   *hcons =  2*bitval[0]+bitval[1];

   for (ii=2; ii < 8; ii++)
     strcpy( &mask_chars[ii-2][0], ppBitname[bitval[ii]+2*ii]);
   return;
}
