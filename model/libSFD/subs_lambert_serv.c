/* subs_lambert_serv.c                                                     */
/*  13dec1999 - more cleanup.  Found some less than kosher tests -- seem   */
/*    to cause no problems now, but could with some other version of       */
/*    operating system (changes in lambert_getval_serv only).              */
/*  10dec1999 correction to function lambert_getval_serv  - to reset       */
/*         file_last so get_bg_fini may be called multiple times.          */
/*  26May1999 to handle test for last read fits file differently - so      */
/*    if required, mask and ebv may be alternated if necessary.            */
/*    Assumes that caller has checked for existence of any file to be used.*/
/*  25May1999 to place func lambert_getval_serv in this file.              */
/*  25May1999 to remove include for subs_common_string.h                   */
/*    and make func lambert_lb2xy static.                                  */
/*  additional mod. 21may1999 - removed include for subs_common_math       */
/*   modification of d.schlegel's original subs_lambert.c to allow 2-way   */
/*    communication between client and server (i.e. server prompts with ?  */
/*    and sends result after receiving response to prompt from client.     */
/*    The original function lambert_getval has been removed from this file.*/
/*    (lambert_getval_serv is in dust_getval_serv).                        */
/*    The function fort_lambert_getval__ has been removed here (not used). */
/*    The function lambert_lb2xy has been bullet proofed in this file.     */
/*    Modifications done 26Apr1999  j.bennett @ IPAC                       */

#include <string.h>
#include <stdio.h>
#include <stdlib.h> /* For call to malloc() */
#include "subs_memory.h"
#include "subs_fits.h"
#include "subs_lambert_serv.h"
#include "subs_inoutput.h"     /* for MAX_FILE_NAME_LEN */
#include <math.h>

static char Label_lam_nsgp[]  = "LAM_NSGP";
static char Label_lam_scal[]  = "LAM_SCAL";
static char Label_crpix1[]    = "CRPIX1  "; /* made local in this version. */
static char Label_crpix2[]    = "CRPIX2  ";
static char Label_crval1[]    = "CRVAL1  ";
static char Label_crval2[]    = "CRVAL2  ";

static uchar *  label_lam_nsgp  = (uchar *) Label_lam_nsgp;
static uchar *  label_lam_scal  = (uchar *) Label_lam_scal;
static uchar *  label_crpix1    = (uchar *) Label_crpix1;
static uchar *  label_crpix2    = (uchar *) Label_crpix2;
static uchar *  label_crval1    = (uchar *) Label_crval1;
static uchar *  label_crval2    = (uchar *) Label_crval2;

static void lambert_lb2fpix
  (float    gall,   /* Galactic longitude */
   float    galb,   /* Galactic latitude */
   HSIZE    nHead,
   uchar *  pHead,
   float *  pX,     /* X position in pixels from the center */
   float *  pY);    /* Y position in pixels from the center */
static void lambert_lb2pix
  (float    gall,   /* Galactic longitude */
   float    galb,   /* Galactic latitude */
   HSIZE    nHead,
   uchar *  pHead,
   int   *  pIX,    /* X position in pixels from the center */
   int   *  pIY);   /* Y position in pixels from the center */

static void lambert_lb2xy
  (float    gall,   /* Galactic longitude */
   float    galb,   /* Galactic latitude */
   int      nsgp,   /* +1 for NGP projection, -1 for SGP */
   float    scale,  /* Radius of b=0 to b=90 degrees in pixels */
   float *  pX,     /* X position in pixels from the center */
   float *  pY);    /* Y position in pixels from the center */

/******************************************************************************/
/* Read one value at a time from NGP+SGP polar projections.
 * Set qInterp=1 to interpolate, or =0 otherwise.
 * Set qVerbose=1 to interpolate, or =0 otherwise.
 */
float   lambert_getval_serv
  (char  *  pFileN,
   char  *  pFileS,
   int      nGal,
   float    pGall[],
   float    pGalb[],
   int      qInterp,
   int      qVerbose)
{
   int      iGal;
   int      ii;
   int      pNS[2]; /* 0 for NGP, 1 for SGP, only 1st element used. */
   int      xPix1;
   int      yPix1;
   DSIZE    pStart[2];
   DSIZE    pEnd[2];
   DSIZE    nSubimg;
   float *  pSubimg;
   float    dx;
   float    dy;
   float    xr;
   float    yr;
   float    pWeight[4];
   float    mapval;
   float    pOutput[2];  /* only first element used */
   float    Mapval;

   /* Variables for FITS files */
   int      numAxes;
   DSIZE *  pNaxis;
   char  *  pFileIn;       /* is set(reset) each time routine called. */
   static HSIZE    nHead;
   static uchar *  pHead;
   static char nofile[5] = "\0\0\0\0";

   static char file_last[MAX_FILE_NAME_LEN] = "\0\0\0\0";

   if (nGal > 1)
     nGal = 1;
   if (nGal <= 0)
   {     /* close up shop */
     Mapval = (float) 0.0;
     if (file_last[0] == '\0')
       return(Mapval);
     fits_dispose_array_(&pHead);
     nHead = 0;
     strcpy(file_last, nofile);
     return(Mapval);
   }

   /* Decide if   point should be read from the NGP or SGP projection */
   iGal = 0;
   pNS[iGal] = (pGalb[iGal] >= 0.0) ? 0 : 1;

            /* Read FITS header for this projection if not yet read */

               if (pNS[iGal] == 0) pFileIn = pFileN; else pFileIn = pFileS;
               if (strcmp(file_last,pFileIn) != 0)
               {
                 if (file_last[0] != '\0')
                   fits_dispose_array_(&pHead);
                 fits_read_file_fits_header_only_(pFileIn, &nHead, &pHead);
                 strcpy(file_last, pFileIn);
            }

            if (qInterp == 0) {  /* NEAREST PIXELS */

               /* Determine the nearest pixel coordinates */
               lambert_lb2pix(pGall[iGal], pGalb[iGal], nHead, pHead,
                &xPix1, &yPix1);

               pStart[0] = xPix1;
               pStart[1] = yPix1;

               /* Read one pixel value from data file */
               fits_read_point_(pFileIn, nHead, pHead, pStart, &mapval);
               pOutput[iGal] = mapval;

               if (qVerbose != 0)
                printf("#V%8.3f %7.3f %1d %8d %8d %12.5e\n",
                pGall[iGal], pGalb[iGal], pNS[iGal], xPix1, yPix1, mapval);

            } else {  /* INTERPOLATE */

               fits_compute_axes_(&nHead, &pHead, &numAxes, &pNaxis);

               /* Determine the fractional pixel coordinates */
               lambert_lb2fpix(pGall[iGal], pGalb[iGal], nHead, pHead,
                &xr, &yr);
               xPix1 = (int)(xr-0.5);
               yPix1 = (int)(yr-0.5);
               dx = xPix1 - xr + 1.5;
               dy = yPix1 - yr + 1.5;

               /* Force pixel values to fall within the image boundaries */
               if (xPix1 < 0) { xPix1 = 0; dx = 1.0; }
               if (yPix1 < 0) { yPix1 = 0; dy = 1.0; }
               if (xPix1 >= pNaxis[0]-1) { xPix1 = pNaxis[0]-2; dx = 0.0; }
               if (yPix1 >= pNaxis[1]-1) { yPix1 = pNaxis[1]-2; dy = 0.0; }

               pStart[0] = xPix1;
               pStart[1] = yPix1;
               pEnd[0] = xPix1 + 1;
               pEnd[1] = yPix1 + 1;

               /* Create array of weights */
               pWeight[0] =    dx  *    dy  ;
               pWeight[1] = (1-dx) *    dy  ;
               pWeight[2] =    dx  * (1-dy) ;
               pWeight[3] = (1-dx) * (1-dy) ;

               /* Read 2x2 array from data file */
               fits_read_subimg_(pFileIn, nHead, pHead, pStart, pEnd,
                &nSubimg, &pSubimg);

               pOutput[iGal] = 0.0;
               for (ii=0; ii < 4; ii++)
                pOutput[iGal] += pWeight[ii] * pSubimg[ii];

               ccfree_((void **)&pNaxis);
               ccfree_((void **)&pSubimg);

               if (qVerbose != 0)
                printf("#V%8.3f %7.3f %1d %8.2f %8.2f %12.5e\n",
                pGall[iGal], pGalb[iGal], pNS[iGal], xr, yr, pOutput[iGal]);
            }


   Mapval = pOutput[0];
   return(Mapval);
}

/******************************************************************************/
/* Transform from galactic (l,b) coordinates to fractional (x,y) pixel location.
 * Latitude runs clockwise from X-axis for NGP, counterclockwise for SGP.
 * This function returns the ZERO-INDEXED pixel position.
 * 
 */
static void lambert_lb2fpix
  (float    gall,   /* Galactic longitude */
   float    galb,   /* Galactic latitude */
   HSIZE    nHead,
   uchar *  pHead,
   float *  pX,     /* X position in pixels from the center */
   float *  pY)     /* Y position in pixels from the center */
{
   int      nsgp;
   float    scale;
   float    crval1;
   float    crval2;
   float    crpix1;
   float    crpix2;
   float    xr;
   float    yr;

   fits_get_card_ival_(&nsgp, label_lam_nsgp, &nHead, &pHead);
   fits_get_card_rval_(&scale, label_lam_scal, &nHead, &pHead);
   fits_get_card_rval_(&crval1, label_crval1, &nHead, &pHead);
   fits_get_card_rval_(&crval2, label_crval2, &nHead, &pHead);
   fits_get_card_rval_(&crpix1, label_crpix1, &nHead, &pHead);
   fits_get_card_rval_(&crpix2, label_crpix2, &nHead, &pHead);

   lambert_lb2xy(gall, galb, nsgp, scale, &xr, &yr);
   *pX = xr + crpix1 - crval1 - 1.0;
   *pY = yr + crpix2 - crval2 - 1.0;
}

/******************************************************************************/
/* Transform from galactic (l,b) coordinates to (ix,iy) pixel location.
 * Latitude runs clockwise from X-axis for NGP, counterclockwise for SGP.
 * This function returns the ZERO-INDEXED pixel position.
 * 
 */
static void lambert_lb2pix
  (float    gall,   /* Galactic longitude */
   float    galb,   /* Galactic latitude */
   HSIZE    nHead,
   uchar *  pHead,
   int   *  pIX,    /* X position in pixels from the center */
   int   *  pIY)    /* Y position in pixels from the center */
{
   int      nsgp;
   int      ixmax;
   int      iymax;
   float    scale;
   float    crval1;
   float    crval2;
   float    crpix1;
   float    crpix2;
   float    xr;
   float    yr;

   fits_get_card_ival_(&nsgp, label_lam_nsgp, &nHead, &pHead);
   fits_get_card_rval_(&scale, label_lam_scal, &nHead, &pHead);
   fits_get_card_rval_(&crval1, label_crval1, &nHead, &pHead);
   fits_get_card_rval_(&crval2, label_crval2, &nHead, &pHead);
   fits_get_card_rval_(&crpix1, label_crpix1, &nHead, &pHead);
   fits_get_card_rval_(&crpix2, label_crpix2, &nHead, &pHead);

   lambert_lb2xy(gall, galb, nsgp, scale, &xr, &yr);
   *pIX = floor(xr + crpix1 - crval1 - 0.5);
   *pIY = floor(yr + crpix2 - crval2 - 0.5);

   /* Force bounds to be valid at edge, for ex at l=0,b=0 */
   ixmax = crpix1 + scale - 1.5;
   iymax = crpix2 + scale - 1.5;
   if (*pIX > ixmax) *pIX = ixmax;
   if (*pIY > iymax) *pIY = iymax;
}

/******************************************************************************/
/* Transform from galactic (l,b) coordinates to (x,y) coordinates from origin.
 * Latitude runs clockwise from X-axis for NGP, counterclockwise for SGP.
 */
/* Updated (to bullet proof rho computation) 26Apr1999 by j.bennett @ IPAC  */

static void lambert_lb2xy
  (float    gall,   /* Galactic longitude */
   float    galb,   /* Galactic latitude */
   int      nsgp,   /* +1 for NGP projection, -1 for SGP */
   float    scale,  /* Radius of b=0 to b=90 degrees in pixels */
   float *  pX,     /* X position in pixels from the center */
   float *  pY)     /* Y position in pixels from the center */
{
   double   rho, rhosqd;
   static double dradeg = 180.0 / 3.1415926534;
   double dnsgp;
   double dscale;

   dnsgp = (double) nsgp;
   dscale = (double) scale;
   rhosqd = 1.0 - dnsgp * sin((double) galb / dradeg);
   if (rhosqd < 0.0)   /* Avoid possible error from inconsequential round-off*/
     rhosqd = 0.0;
   rho = sqrt(rhosqd);
   *pX = rho * cos((double) gall / dradeg) * dscale;
   *pY = -dnsgp * rho * sin((double) gall / dradeg) * dscale;
}
/******************************************************************************/
