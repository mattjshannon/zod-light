/*  subs_fits.c  modified with (signed char *) casts to avoid cc compiler   */
/*      and lint warnings.  cast applied to unsigned char * .               */
/*  26May1999 modified to eliminate some unused variables.                  */

/*      Also removed unused functions and declared others static where okay.*/
/*   modified by j.bennett @ IPAC 21,24May1999                              */

/******************************************************************************/
/*
 * Subroutines to read and write FITS format files.
 * Note all variables are passed as pointers, so the routines can be called
 * by either C or Fortran programs.
 * Remember to omit the final underscore for calls from Fortran,
 * so one says 'call fits_add_card(...)' or 'i=fits_add_card(...)' in Fortran,
 * but 'i=fits_add_card_(...)' in C.
 *
 * D Schlegel -- Berkeley -- ANSI C
 * Mar 1992  DJS  Created
 * Dec 1993  DJS  Major revisions to allow dynamic memory allocations.
 */
/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h> /* For function toupper() */
#include <stdlib.h> /* For function abs()     */
#include "subs_fits.h"
#include "subs_inoutput.h"
#include "subs_memory.h"

#define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#define max(a,b) ( ((a) > (b)) ? (a) : (b) )
#define TRUE  1
#define FALSE 0

void fits_byteswap (int      bitpix, DSIZE    nData, uchar *  pData);
void fits_bswap2 (uchar *  pc1, uchar *  pc2);

/*     declaration for static functions in file subs_fits.c  */
/*     other declarations for subs_fits.c in subs_fits.h     */

static void fits_read_subimg1
  (int      nel,
   DSIZE *  pNaxis,
   DSIZE *  pStart,
   DSIZE *  pEnd,
   int      fileNum,
   int      bitpix,
   DSIZE *  pNVal,
   uchar *  pData);
static void fits_read_fits_header_
  (int   *  pFilenum,
   HSIZE *  pNHead,
   uchar ** ppHead);
static void fits_skip_header_
  (int   *  pFilenum);
static void fits_add_required_cards_
  (HSIZE *  pNHead,
   uchar ** ppHead);
static HSIZE fits_add_card_
  (uchar    pCard[],
   HSIZE *  pNHead,
   uchar ** ppHead);
static HSIZE fits_purge_blank_cards_
  (HSIZE *  pNHead,
   uchar ** ppHead);
static HSIZE fits_delete_card_
  (uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead);
static HSIZE fits_find_card_
  (uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead);
static HSIZE fits_change_card_
  (uchar    pCard[],
   HSIZE *  pNHead,
   uchar ** ppHead);
static HSIZE fits_change_card_ival_
  (int   *  pIval,
   uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead);
static void fits_string_to_card_
  (uchar    pString[],
   uchar    pCard[]);
static float fits_get_rval_
  (DSIZE *  pIloc,
   int   *  pBitpix,
   float *  pBscale,
   float *  pBzero,
   uchar ** ppDdata);
static int fits_get_next_card_
  (int   *  pFilenum,
   uchar    pCard[]);
static int fits_size_from_bitpix_
  (int   *  pBitpix);

static char Label_bitpix[]  = "BITPIX  ";  /* make these local for this vers.*/
static char Label_bscale[]  = "BSCALE  ";
static char Label_bzero[]   = "BZERO   ";
static char Label_naxis[]   = "NAXIS   ";
static char Label_empty[]   = "        ";
static char Card_empty[] =
   "                                        "\
   "                                        ";
static char Card_end[] =
   "END                                     "\
   "                                        ";

static uchar *  label_bitpix  = (uchar*)Label_bitpix;
static uchar *  label_naxis   = (uchar*)Label_naxis;
static uchar *  label_empty   = (uchar*)Label_empty;
static uchar *  card_empty    = (uchar*)Card_empty;
static uchar *  card_end      = (uchar*)Card_end;

/* ---------------------------------------------------------------------- */
/* "eliminate" unused items: */             
#if 0
char Datum_zero[]    = "\0\0\0\0";
char Label_airmass[] = "AIRMASS ";
char Label_blank[]   = "BLANK   ";
char Label_cdelt1[]  = "CDELT1  ";
char Label_cdelt2[]  = "CDELT2  ";
char Label_crpix1[]  = "CRPIX1  ";
char Label_crpix2[]  = "CRPIX2  ";
char Label_crval1[]  = "CRVAL1  ";
char Label_crval2[]  = "CRVAL2  ";
char Label_date_obs[]= "DATE-OBS";
char Label_dec[]     = "DEC     ";
char Label_end[]     = "END     ";
char Label_exposure[]= "EXPOSURE";
char Label_extend[]  = "EXTEND  ";
char Label_filtband[]= "FILTBAND";
char Label_filter[]  = "FILTER  ";
char Label_ha[]      = "HA      ";
char Label_instrume[]= "INSTRUME";
char Label_lamord[]  = "LAMORD  ";
char Label_loss[]    = "LOSS    ";
char Label_naxis1[]  = "NAXIS1  ";
char Label_naxis2[]  = "NAXIS2  ";
char Label_object[]  = "OBJECT  ";
char Label_observer[]= "OBSERVER";
char Label_pa[]      = "PA      ";
char Label_platescl[]= "PLATESCL";
char Label_ra[]      = "RA      ";
char Label_rnoise[]  = "RNOISE  ";
char Label_rota[]    = "ROTA    ";
char Label_seeing[]  = "SEEING  ";
char Label_skyrms[]  = "SKYRMS  ";
char Label_skyval[]  = "SKYVAL  ";
char Label_slitwidt[]= "SLITWIDT";
char Label_st[]      = "ST      ";
char Label_telescop[]= "TELESCOP";
char Label_time[]    = "TIME    ";
char Label_tub[]     = "TUB     ";
char Label_ut[]      = "UT      ";
char Label_vhelio[]  = "VHELIO  ";
char Label_vminusi[] = "VMINUSI ";
char Card_simple[] =
   "SIMPLE  =                    T          "\
   "                                        ";
char Card_null[] =
   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"\
   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"\
   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"\
   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
char Text_T[] = "T";
char Text_F[] = "F";
#endif

#if 0
uchar *  datum_zero    = (uchar*)Datum_zero;
uchar *  label_airmass = (uchar*)Label_airmass;
uchar *  label_blank   = (uchar*)Label_blank;
uchar *  label_bscale  = (uchar*)Label_bscale;
uchar *  label_bzero   = (uchar*)Label_bzero;
uchar *  label_cdelt1  = (uchar*)Label_cdelt1;
uchar *  label_cdelt2  = (uchar*)Label_cdelt2;
uchar *  label_crpix1  = (uchar*)Label_crpix1;
uchar *  label_crpix2  = (uchar*)Label_crpix2;
uchar *  label_crval1  = (uchar*)Label_crval1;
uchar *  label_crval2  = (uchar*)Label_crval2;
uchar *  label_date_obs= (uchar*)Label_date_obs;
uchar *  label_dec     = (uchar*)Label_dec;
uchar *  label_end     = (uchar*)Label_end;
uchar *  label_exposure= (uchar*)Label_exposure;
uchar *  label_extend  = (uchar*)Label_extend;
uchar *  label_filtband= (uchar*)Label_filtband;
uchar *  label_filter  = (uchar*)Label_filter;
uchar *  label_ha      = (uchar*)Label_ha;
uchar *  label_instrume= (uchar*)Label_instrume;
uchar *  label_lamord  = (uchar*)Label_lamord;
uchar *  label_loss    = (uchar*)Label_loss;
uchar *  label_naxis1  = (uchar*)Label_naxis1;
uchar *  label_naxis2  = (uchar*)Label_naxis2;
uchar *  label_object  = (uchar*)Label_object;
uchar *  label_observer= (uchar*)Label_observer;
uchar *  label_pa      = (uchar*)Label_pa;
uchar *  label_platescl= (uchar*)Label_platescl;
uchar *  label_ra      = (uchar*)Label_ra;
uchar *  label_rnoise  = (uchar*)Label_rnoise;
uchar *  label_rota    = (uchar*)Label_rota;
uchar *  label_seeing  = (uchar*)Label_seeing;
uchar *  label_skyrms  = (uchar*)Label_skyrms;
uchar *  label_skyval  = (uchar*)Label_skyval;
uchar *  label_slitwidt= (uchar*)Label_slitwidt;
uchar *  label_st      = (uchar*)Label_st;
uchar *  label_telescop= (uchar*)Label_telescop;
uchar *  label_time    = (uchar*)Label_time;
uchar *  label_tub     = (uchar*)Label_tub;
uchar *  label_ut      = (uchar*)Label_ut;
uchar *  label_vhelio  = (uchar*)Label_vhelio;
uchar *  label_vminusi = (uchar*)Label_vminusi;
uchar *  card_simple   = (uchar*)Card_simple;
uchar *  card_null     = (uchar*)Card_null;
uchar *  text_T        = (uchar*)Text_T;
uchar *  text_F        = (uchar*)Text_F;
#endif
/* ---------------------------------------------------------------------- */

/******************************************************************************/
/*
 * Read in FITS format data.  Assume the header is a multiple of
 * 2880-byte blocks, with the last block containing an END card.
 * Any data that follows is ignored.
 */
void fits_read_file_fits_header_only_
  (char     pFileName[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   int      fileNum;
   char     pPrivR[] = "r\0";

   inoutput_open_file(&fileNum, pFileName, pPrivR);

   /* Read header */
   fits_read_fits_header_(&fileNum, pNHead, ppHead);

   inoutput_close_file(fileNum);
}

/******************************************************************************/
/*
 * Read subimage from a FITS format data file, indexed from pStart to pEnd
 * in each dimension.
 *
 * The header is assumed to already be read using
 * fits_read_file_fits_header_only_(), to avoid reading it upon every
 * call to this routine.  The axis dimensions and BITPIX are read from
 * the header that is passed.  The dimensions of pLoc must agree with
 * the dimensions specified by NAXIS in this header.
 *
 * The data values are rescaled to 32-bit reals.
 *
 * Returned value is 0 unless the FITS file contains fewer data points
 * than requested, in which case the difference is returned.
 */

/* Comment by J.Bennett, may1999:
 *  lint messages indicate "pointer cast may result in improper alignment".
 *  However, since allocations were done using void with malloc, most likely
 *  the data is okay as long as the fits data (BITPIX in header, etc.) is
 *  okay. This seems to be a "one size fits all routine" - if it works,
 *  leave it alone.
 */

DSIZE fits_read_subimg_
  (char     pFileName[],
   HSIZE    nHead,
   uchar *  pHead,
   DSIZE *  pStart,
   DSIZE *  pEnd,
   DSIZE *  pNVal,
   float ** ppVal)
{
   int      bitpix;
   DSIZE    iloc;
   DSIZE    nExpect;
   int      size;
   MEMSZ    memSize;
   int      iAxis;
   int      numAxes;
   DSIZE *  pNaxis;
   float    bscale;
   float    bzero;
   uchar *  pData;

   int      fileNum;
   char     pPrivR[] = "r\0";

   inoutput_open_file(&fileNum, pFileName, pPrivR);

   /* Skip header */
   fits_skip_header_(&fileNum);

   /* From the given header, read BITPIX and PNAXIS */
   fits_get_card_ival_(&bitpix, label_bitpix, &nHead, &pHead);
   fits_compute_axes_(&nHead, &pHead, &numAxes, &pNaxis);

   /* Allocate memory for output */
   nExpect = 1;
   for (iAxis=0; iAxis < numAxes; iAxis++)
    nExpect *= (pEnd[iAxis] - pStart[iAxis] + 1);
   size = fits_size_from_bitpix_(&bitpix);
   memSize = size * nExpect;
   ccalloc_(&memSize, (void **)&pData);

   *pNVal = 0;
   fits_read_subimg1(numAxes, pNaxis, pStart, pEnd, fileNum, bitpix,
    pNVal, pData);
#ifdef LITTLE_ENDIAN
   fits_byteswap(bitpix, *pNVal, pData);
#endif

   /* Convert data to real*4 if not already */
   if (bitpix == -32) {
      *ppVal = (float *)pData;
   } else {
      /* Get the scaling parameters from the header */
      if (fits_get_card_rval_(&bscale, (uchar *)Label_bscale, &nHead, &pHead)
       == FALSE) {
         bscale = 1.0;  /* Default value for BSCALE */
      }
      if (fits_get_card_rval_(&bzero , (uchar *)Label_bzero , &nHead, &pHead)
       == FALSE) {
         bzero = 0.0;  /* Default value for BZERO */
      }
 
      memSize = sizeof(float) * nExpect;
      ccalloc_(&memSize, (void **)ppVal);
      for (iloc=0; iloc < *pNVal; iloc++)
       (*ppVal)[iloc] = fits_get_rval_(&iloc, &bitpix, &bscale, &bzero, &pData);
   }
 
   inoutput_close_file(fileNum);

   return (nExpect - (*pNVal));
}

static void fits_read_subimg1
  (int      nel,
   DSIZE *  pNaxis,
   DSIZE *  pStart,
   DSIZE *  pEnd,
   int      fileNum,
   int      bitpix,
   DSIZE *  pNVal,
   uchar *  pData)
{
   int      iloop;
   int      ii;
   int      ipos;
   int      size;
   DSIZE    nskip;
   DSIZE    nread;
   FILE  *  pFILEin;

   pFILEin = pFILEfits[fileNum];
   size = fits_size_from_bitpix_(&bitpix);

   /* Skip "nskip" points */
   nskip = pStart[nel-1];
   for (ii=0; ii < nel-1; ii++) nskip = nskip * pNaxis[ii];
   ipos = ftell(pFILEin);
   fseek(pFILEin, (ipos + size*nskip), 0);

   if (nel > 1) {
      for (iloop=0; iloop < pEnd[nel-1]-pStart[nel-1]+1; iloop++)
       fits_read_subimg1(nel-1, pNaxis, pStart, pEnd, fileNum, bitpix,
        pNVal, pData);
   } else {
      nread = pEnd[0]-pStart[0]+1;

      /* Read in "nread" points */
      *pNVal += (int)fread(&pData[(*pNVal)*size], size, nread, pFILEin);
   }

   /* Skip "nskip" points */
   nskip = pNaxis[nel-1] - pEnd[nel-1] - 1;
   for (ii=0; ii < nel-1; ii++) nskip = nskip * pNaxis[ii];
   ipos = ftell(pFILEin);
   fseek(pFILEin, (ipos + size*nskip), 0);
}

/******************************************************************************/
/*
 * Read in one element from a FITS format data file, indexed by the
 * values in pLoc.
 *
 * The header is assumed to already be read using
 * fits_read_file_fits_header_only_(), to avoid reading it upon every
 * call to this routine.  The axis dimensions and BITPIX are read from
 * the header that is passed.  The dimensions of pLoc must agree with
 * the dimensions specified by NAXIS in this header.
 *
 * The data value is rescaled to a 32-bit real.
 *
 * Returned value is 0 unless the FITS file contains fewer data points
 * than requested (1), in which case the difference (1) is returned.
 */

/* Comment by J.Bennett, may1999:
 *  lint messages indicate "pointer cast may result in improper alignment".
 *  However, since allocations were done using void with malloc, most likely
 *  the data is okay as long as the fits data (BITPIX in header, etc.) is
 *  okay. This seems to be a "one size fits all routine" - if it works,
 *  leave it alone.
 */

DSIZE fits_read_point_
  (char     pFileName[],
   HSIZE    nHead,
   uchar *  pHead,
   DSIZE *  pLoc,
   float *  pValue)
{
   int      bitpix;
   DSIZE    iloc;
   int      nmult;
   int      size;
   MEMSZ    memSize;
   int      iAxis;
   int      numAxes;
   DSIZE *  pNaxis;
   float    bscale;
   float    bzero;
   uchar *  pData;
   DSIZE    retval;

   int      fileNum;
   int      ipos;
   char     pPrivR[] = "r\0";
   FILE  *  pFILEin;

   inoutput_open_file(&fileNum, pFileName, pPrivR);

   /* Skip header */
   fits_skip_header_(&fileNum);

   /* From the given header, read BITPIX and PNAXIS */
   fits_get_card_ival_(&bitpix, label_bitpix, &nHead, &pHead);
   fits_compute_axes_(&nHead, &pHead, &numAxes, &pNaxis);

   /* Find the 1-dimensional index for the data point requested */
   iloc = 0;
   nmult = 1;
   for (iAxis=0; iAxis < numAxes; iAxis++) {
      iloc = iloc + pLoc[iAxis] * nmult;
      nmult = nmult * pNaxis[iAxis];
   }

   /* Read one element from the data file */
   pFILEin = pFILEfits[fileNum];
   ipos = ftell(pFILEin);
   size = fits_size_from_bitpix_(&bitpix);
   memSize = size;
   ccalloc_(&memSize, (void **)&pData);
   fseek(pFILEin, (ipos + size*iloc), 0);
   retval = 1 - (int)fread(pData, size, 1, pFILEin);
#ifdef LITTLE_ENDIAN
   fits_byteswap(bitpix, 1, pData);
#endif

   /* Convert data to real*4 if not already */
   if (bitpix == -32) {
      *pValue = *( (float *)pData );
   } else {
      /* Get the scaling parameters from the header */
      if (fits_get_card_rval_(&bscale, (uchar *)Label_bscale, &nHead, &pHead)
       == FALSE) {
         bscale = 1.0;  /* Default value for BSCALE */
      }
      if (fits_get_card_rval_(&bzero , (uchar *)Label_bzero , &nHead, &pHead)
       == FALSE) {
         bzero = 0.0;  /* Default value for BZERO */
      }

      iloc = 0;
      *pValue = fits_get_rval_(&iloc, &bitpix, &bscale, &bzero, &pData);
   }

   inoutput_close_file(fileNum);

   return retval;
}

/******************************************************************************/
/*
 * Read header blocks from an open FITS file.
 * Memory for new blocks are dynamically allocated when needed.
 */
static void fits_read_fits_header_
  (int   *  pFilenum,
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    iCard;
   HSIZE    jCard;
   uchar    pCard[80];

   /* Read the header into memory until the END card */
   *pNHead = 0;
   while (fits_get_next_card_(pFilenum, pCard)) {
      /* Only include this card if it is not blank */
      if (strncmp((char *) pCard, (char *) card_empty, 80) != 0) {
         fits_add_card_(pCard, pNHead, ppHead);
      }
   }
   fits_add_card_(card_end, pNHead, ppHead);
 
   /* Finish reading to the end of the last header block (the one w/END) */
   /* ignoring, and in effect deleting, any header cards after the END card */
   jCard = (ftell(pFILEfits[*pFilenum]) % 2880)/80 ;
   if (jCard != 0) {
      for (iCard=jCard; iCard<=35; iCard++) {
         fits_get_next_card_(pFilenum, pCard);
      }
   }

   /* Delete all cards where the label is blank */
   fits_purge_blank_cards_(pNHead, ppHead);

   /* Add missing cards to the FITS header */
   fits_add_required_cards_(pNHead, ppHead);
}

/******************************************************************************/
/*
 * Skip header blocks from an open FITS file.
 * This is a modified version of fits_read_fits_header_().
 */
static void fits_skip_header_
  (int   *  pFilenum)
{
   HSIZE    iCard;
   HSIZE    jCard;
   uchar    pCard[80];

   /* Read the header into memory until the END card */
   while (fits_get_next_card_(pFilenum, pCard));
 
   /* Finish reading to the end of the last header block (the one w/END) */
   jCard = (ftell(pFILEfits[*pFilenum]) % 2880)/80 ;
   if (jCard != 0) {
      for (iCard=jCard; iCard<=35; iCard++) {
         fits_get_next_card_(pFilenum, pCard);
      }
   }
}

/******************************************************************************/
/*
 * Add any cards to the header that are required by the FITS definition
 * but are missing.
 */
static void fits_add_required_cards_
  (HSIZE *  pNHead,
   uchar ** ppHead)
{
   int      iAxis;
   int      numAxes;
   int      naxis;
   int      naxisX;
#if 0
   int      crpixX;
   float    crvalX;
   float    cdeltX;
#endif
   DSIZE *  pNaxis;
   uchar    pLabel_temp[8];

   if (fits_get_card_ival_(&naxis, label_naxis, pNHead, ppHead) == FALSE) {
      naxis = 0; /* default to no data axes */
      fits_change_card_ival_(&naxis, label_naxis, pNHead, ppHead);
   }

   fits_compute_axes_(pNHead, ppHead, &numAxes, &pNaxis);

   for (iAxis=0; iAxis < numAxes; iAxis++) {
      /* For each axis, be sure that a NAXISx, CRPIXx, CRVALx and CDELTx
       * card exists.  If one does not exist, then create it.
       * Create the labels for each axis for which to look as pLabel_temp.
       * Be certain to pad with spaces so that a NULL is not written.
       */

      snprintf((char *) pLabel_temp, 8, "NAXIS%d  ", iAxis+1);
      if (fits_get_card_ival_(&naxisX, pLabel_temp, pNHead, ppHead) == FALSE) {
         naxisX = 1; /* default to 1 */
         fits_change_card_ival_(&naxisX, pLabel_temp, pNHead, ppHead);
         printf("Adding a card %s\n", pLabel_temp);
      }

#if 0
      snprintf(pLabel_temp, 8, "CRPIX%d  ", iAxis+1);
      if (fits_get_card_ival_(&crpixX, pLabel_temp, pNHead, ppHead) == FALSE) {
         crpixX = 1; /* default to start numbering at the first pixel */
         fits_change_card_ival_(&crpixX, pLabel_temp, pNHead, ppHead);
         printf("Adding a card %s\n", pLabel_temp);
      }

      snprintf(pLabel_temp, 8, "CRVAL%d  ", iAxis+1);
      if (fits_get_card_rval_(&crvalX, pLabel_temp, pNHead, ppHead) == FALSE) {
         crvalX = 0.0; /* default to the first pixel value to be zero */
         fits_change_card_rval_(&crvalX, pLabel_temp, pNHead, ppHead);
         printf("Adding a card %s\n", pLabel_temp);
      }

      snprintf(pLabel_temp, 8, "CDELT%d  ", iAxis+1);
      if (fits_get_card_rval_(&cdeltX, pLabel_temp, pNHead, ppHead) == FALSE) {
         cdeltX = 1.0; /* default to spacing each pixel by a value of 1 */
         fits_change_card_rval_(&cdeltX, pLabel_temp, pNHead, ppHead);
         printf("Adding a card %s\n", pLabel_temp);
      }
#endif
   }

}

/******************************************************************************/
/*
 * Free the memory allocated for a FITS header or data array.
 * Return TRUE if the array existed and was freed, and FALSE otherwise.
 */
int fits_dispose_array_
  (uchar ** ppHeadOrData)
{
   int      retval;

   retval = FALSE;
   if (*ppHeadOrData != NULL) {
      ccfree_((void **)ppHeadOrData);
      retval = TRUE;
   }
   return retval;
}

/******************************************************************************/
/*
 * Compute the number of axes and the dimension of each axis.
 * This information is determined from the header cards NAXIS and NAXISx.
 */
void fits_compute_axes_
  (HSIZE *  pNHead,
   uchar ** ppHead,
   int   *  pNumAxes,
   DSIZE ** ppNaxis)
{
   int      iAxis;
   int      ival;
   DSIZE *  pNaxis;
   MEMSZ    memSize;
   uchar    pLabel_temp[8];

   fits_get_card_ival_(pNumAxes, label_naxis, pNHead, ppHead);
   if (*pNumAxes > 0) {
      memSize = (*pNumAxes) * sizeof(DSIZE);
      ccalloc_(&memSize, (void **)ppNaxis);
      pNaxis = *ppNaxis;
      for (iAxis=0; iAxis < *pNumAxes; iAxis++) {
         /* Create the label for this axis for which to look.
          * Be certain to pad with spaces so that a NULL is not written.
          */
         snprintf((char *) pLabel_temp, 8, "NAXIS%d  ", iAxis+1);
         fits_get_card_ival_(&ival, pLabel_temp, pNHead, ppHead);
         pNaxis[iAxis] = ival;
      }
   }
}
       
/******************************************************************************/
/*
 * Add a card immediately before the END card, or as the next card
 * (if no blank or END card), whichever comes first.  Return the card
 * number of the added card.
 * Memory is dynamically allocated if necessary by adding another 2880-byte
 * block.
 */
static HSIZE fits_add_card_
  (uchar    pCard[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    numCardEnd;
   MEMSZ    memSize;
   uchar    pCardTemp[80];
   uchar *  pNewHeader;

   fits_string_to_card_(pCard, pCardTemp);

   numCardEnd=fits_find_card_(card_end, pNHead, ppHead);

   /* Test to see if more memory is needed for the header */
   if ((*pNHead)%36 == 0) {
      /* Copy header to new location and change appropriate pointers */
      memSize = (36+(*pNHead)) * 80;
      ccalloc_(&memSize, (void **)&pNewHeader);
      if (*pNHead > 0) {
         memmove(pNewHeader, *ppHead, (*pNHead)*80);
         ccfree_((void **)ppHead);
      }
      *ppHead = pNewHeader;
      numCardEnd += (pNewHeader - *ppHead);
   }

   if ((*pNHead > 0) && (numCardEnd<*pNHead) ) {
      /* Copy the end card forward 80 bytes in memory */
      memmove(&(*ppHead)[(numCardEnd+1)*80], &(*ppHead)[numCardEnd*80], 80);
      /* Add the new card where the END card had been */
      memmove(&(*ppHead)[numCardEnd*80], pCardTemp, 80);
      (*pNHead)++;
      return numCardEnd;
   }
   else {
      /* There is no end card, so simply add the new card at end of header */
      memmove(&(*ppHead)[(*pNHead)*80], pCardTemp, 80);
      return (*pNHead)++;
   }
}

/******************************************************************************/
/*
 * Delete all cards where the label is blank.
 * Return the number of cards that were discarded.
 */
static HSIZE fits_purge_blank_cards_
  (HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    numDelete;

   numDelete = 0;
   while (fits_delete_card_(label_empty, pNHead, ppHead) != *pNHead) {
      numDelete++;
   }

   return numDelete;
}

/******************************************************************************/
/*
 * Delete the first card that matches the given label, or do nothing if no
 * matches are found.  Return the card number of the deleted card,
 * or return nHead (out of range) if no match was found.
 */
static HSIZE fits_delete_card_
  (uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    iCard;
   HSIZE    jCard;
   uchar *  pHead = *ppHead;

   iCard = fits_find_card_(pLabel, pNHead, ppHead);
   if (iCard < *pNHead) {
      (*pNHead)--;
      for (jCard=iCard; jCard <* pNHead; jCard++) {
         memmove(&pHead[jCard*80], &pHead[(jCard+1)*80], 80);
      }
      memmove(&pHead[jCard*80], card_empty, 80);
   }
   return iCard;
}

/******************************************************************************/
/*
 * Return the card number of the 1st header card with the label passed,
 * or return nHead (out of range) if no match was found.
 */
static HSIZE fits_find_card_
  (uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    iCard;
   uchar *  pHead;

   if (*pNHead == 0) iCard=0;
   else {
      pHead = *ppHead;
      for (iCard=0;
       (iCard<*pNHead) && (strncmp((char *) pLabel,(char *) &pHead[iCard*80],8)!=0);
       iCard++);
   }
   return iCard;
}

/******************************************************************************/
/*
 * Find the 1st header card whose label matches the label passed,
 * and return the integer value of the argument after the label.
 * Return TRUE if there is a match, and FALSE if there is none.
 */
int fits_get_card_ival_
  (int   *  pIval,
   uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    iCard;
   HSIZE    iret;
   uchar *  pHead = *ppHead;
   char     pTemp[21];

   for (iCard=0; (iCard<*pNHead) && 
       (strncmp((char *) pLabel, (char *) &pHead[iCard*80], 8)!=0);
    iCard++);
   if (iCard < *pNHead) {
#if 0
     sscanf(&pHead[iCard*80+10], "%20d", pIval);
#endif
     memmove(pTemp, &pHead[iCard*80+10], 20);
     pTemp[20] = '\0';
     sscanf(pTemp, "%d", pIval);
     iret = TRUE;
   }
   else {
     iret = FALSE;
   }
   return iret;
}

/******************************************************************************/
/*
 * Find the 1st header card whose label matches the label passed,
 * and return the real (float) value of the argument after the label.
 * Return TRUE if there is a match, and FALSE if there is none.
 */
int fits_get_card_rval_
  (float *  pRval,
   uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   int      iCard;
   int      iret;
   uchar *  pHead = *ppHead;
   char     pTemp[21];

   for (iCard=0; (iCard<*pNHead) &&
                 (strncmp((char *) pLabel, (char *) &pHead[iCard*80], 8)!=0);
    iCard++);
   if (iCard < *pNHead) {
#if 0
     sscanf(&pHead[iCard*80+10], "%20f", pRval);
#endif
     memmove(pTemp, &pHead[iCard*80+10], 20);
     pTemp[20] = '\0';
     sscanf(pTemp, "%f", pRval);
     iret = TRUE;
   }
   else {
     iret = FALSE;
   }
   return iret;
}

#if 0
/******************************************************************************/
/*
 * Return TRUE if there is a match, and FALSE if there is none.
 */
int fits_get_julian_date_
  (float *  pJulianDate,
   uchar    pLabelDate[],
   uchar    pLabelTime[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   int      iret;
   int      month;
   int      date;
   int      year;
   float    time;

   if (iret=fits_get_card_date_(month,date,year,pLabelDate,pNHead,ppHead)
    == TRUE) {
      *pJulianDate=...
      if (fits_get_card_time_(&time,pLabelTime,pNHead,ppHead) == TRUE) {
         *pJulianDate+=...
      }
   } else {
      *pJulianDate=0.0;
   }
   return iret;
}
#endif

/******************************************************************************/
/*
 * Change the 1st card that matches the passed label, or add a card if there
 * is not a match.  Return the card number of the changed or added card.
 */
static HSIZE fits_change_card_
  (uchar    pCard[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    iCard;
   uchar    pCardTemp[80];
   uchar *  pHead = *ppHead;

   fits_string_to_card_(pCard, pCardTemp);

   iCard = fits_find_card_(pCardTemp, pNHead, ppHead);
   if (iCard < *pNHead) {
      memmove(&pHead[iCard*80], pCardTemp, 80);
   } else {
      iCard = fits_add_card_(pCardTemp, pNHead, ppHead);
   }

   return iCard;
}

/******************************************************************************/
/*
 * Find the 1st header card whose label matches the label passed,
 * and change the integer value of the argument after the label.
 * If no card exists, then create one.  Return the card number of
 * the changed or added card.
 */
static HSIZE fits_change_card_ival_
  (int   *  pIval,
   uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead)
{
   HSIZE    iCard;
   uchar    pCardTemp[81]; /* Last character is for null from sprintf() */

   snprintf((char *) pCardTemp, 81, "%-8.8s= %20d", pLabel, *pIval);
   iCard = fits_change_card_(pCardTemp, pNHead, ppHead);

   return iCard;
}

/******************************************************************************/
/* Convert a character string to a FITS-complient 80-character card.
 * The string is copied until either a NULL or CR is reached or the 80th
 * character is reached.  The remainder of the card is padded with spaces.
 * In addition, the label part of the card (the first 8 characters)
 * are converted to upper case.
 *
 * Note that pCard[] must be dimensioned to at least the necessary 80
 * characters.
 */
static void fits_string_to_card_
  (uchar    pString[],
   uchar    pCard[])
{
   int      iChar;
   int      qNull;

   /* Copy the raw string into the card array */
   memmove(pCard, pString, 80);

   /* Search for a NULL or CR in the card, and replace that character and
    * all following characters with a space.
    */
   qNull = FALSE;
   iChar = 0;
   while (iChar < 80) {
      if (pCard[iChar] == '\0' || pCard[iChar] == '\n') qNull = TRUE;
      if (qNull == TRUE) pCard[iChar] = ' ';
      iChar++;
   }

   /* Convert the label (the first 8 characters) to upper case) */
   for (iChar=0; iChar < 8; iChar++) {
      pCard[iChar] = toupper(pCard[iChar]);
   }
}

/******************************************************************************/
/*
 * Return the (float) value of the data array indexed by the iloc'th elements,
 * taking care to use the proper data format as specified by bitpix.
 * Several unconventional values for bitpix are supported: 32, 8, -8.
 * For a 2-dimensional array, set iloc=x+y*naxis1.
 */


static float fits_get_rval_
  (DSIZE *  pIloc,
   int   *  pBitpix,
   float *  pBscale,
   float *  pBzero,
   uchar ** ppData)
{
   float    rval;
   uchar     * pIdata8  = (uchar     *)(*ppData);
   short int * pIdata16 = (short int *)(*ppData);
   long  int * pIdata32 = (long  int *)(*ppData);
   float     * pRdata32 = (float     *)(*ppData);
   double    * pRdata64 = (double    *)(*ppData);

   if      (*pBitpix ==-32) rval = pRdata32[*pIloc];
   else if (*pBitpix == 16) rval = pIdata16[*pIloc] * (*pBscale) + (*pBzero);
   else if (*pBitpix == 32) rval = pIdata32[*pIloc] * (*pBscale) + (*pBzero);
   else if (*pBitpix ==-64) rval = pRdata64[*pIloc];
   else if (*pBitpix ==  8) rval = pIdata8 [*pIloc] * (*pBscale) + (*pBzero);
   else if (*pBitpix == -8) rval = pIdata8 [*pIloc];
   else                     rval = 0.0; /* Invalid BITPIX! */
   return rval;
}

/******************************************************************************/
/*
 * Read the next 80-character card from the specified device.
 * Return 0 if the END card is reached.
 */
static int fits_get_next_card_
  (int   *  pFilenum,
   uchar    pCard[])
{
   int      iChar;

   for (iChar=0; iChar < 80; iChar++) {
      pCard[iChar] = fgetc(pFILEfits[*pFilenum]);
   }
   return strncmp((char *) card_end, (char *) pCard, 8);
}

/******************************************************************************/
/*
 * Determine the size of an individual datum based upon the FITS definitions
 * of the BITPIX card.
 */
static int fits_size_from_bitpix_
  (int *pBitpix)
{
   int size;

   if      (*pBitpix ==   8) size = 1;
   else if (*pBitpix ==  16) size = 2;
   else if (*pBitpix ==  32) size = 4;
   else if (*pBitpix ==  64) size = 8;
   else if (*pBitpix == -16) size = 2;
   else if (*pBitpix == -32) size = 4;
   else if (*pBitpix == -64) size = 8;
   else                      size = 0; /* Bitpix undefined! */

   return size;
}


/******************************************************************************/
/*
 * Swap bytes between big-endian and little-endian.
 */
void fits_byteswap
  (int      bitpix,
   DSIZE    nData,
   uchar *  pData)
{
   int      ibits;
   DSIZE    idata;

   ibits = abs(bitpix);
   if (ibits == 16) {
      for (idata=0; idata < nData; idata++) {
         fits_bswap2( &pData[2*idata  ], &pData[2*idata+1] );
      }
   } else if (ibits == 32) {
      for (idata=0; idata < nData; idata++) {
         fits_bswap2( &pData[4*idata  ], &pData[4*idata+3] );
         fits_bswap2( &pData[4*idata+1], &pData[4*idata+2] );
      }
   } else if (ibits == 64) {
      for (idata=0; idata < nData; idata++) {
         fits_bswap2( &pData[8*idata  ], &pData[8*idata+7] );
         fits_bswap2( &pData[8*idata+1], &pData[8*idata+6] );
         fits_bswap2( &pData[8*idata+2], &pData[8*idata+5] );
         fits_bswap2( &pData[8*idata+3], &pData[8*idata+4] );
      }
   }

}

void fits_bswap2
  (uchar *  pc1,
   uchar *  pc2)
{
   uchar    ct;
   ct = *pc1;
   *pc1 = *pc2;
   *pc2 = ct;
}
