/* subs_fits.h  for Schlegel subs_fits.c routines                          */
/*  modified 24may1999 by J.Bennett @ IPAC to remove unused routines       */
/*  declarations for static functions moved to subs_fits.c                 */
/*  Modified 26May1999 to comment unneeded statements.                     */

#ifndef __INCsubs_fits_h
#define __INCsubs_fits_h

typedef unsigned char uchar;
typedef long int HSIZE;
typedef long int DSIZE;

void fits_read_file_fits_header_only_
  (char     pFileName[],
   HSIZE *  pNHead,
   uchar ** ppHead);
DSIZE fits_read_subimg_
  (char     pFileName[],
   HSIZE    nHead,
   uchar *  pHead,
   DSIZE *  pStart,
   DSIZE *  pEnd,
   DSIZE *  pNVal,
   float ** ppVal);
DSIZE fits_read_point_
  (char     pFileName[],
   HSIZE    nHead,
   uchar *  pHead,
   DSIZE *  pLoc,
   float *  pValue);
int fits_dispose_array_
  (uchar ** ppHeadOrData);
void fits_compute_axes_
  (HSIZE *  pNHead,
   uchar ** ppHead,
   int   *  pNumAxes,
   DSIZE ** ppNaxis);
int fits_get_card_ival_
  (int   *  pIval,
   uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead);
int fits_get_card_rval_
  (float *  pRval,
   uchar    pLabel[],
   HSIZE *  pNHead,
   uchar ** ppHead);

#if 0              /* comment out unneeded and unwanted statements: */
extern uchar * label_bitpix;
extern uchar * label_crpix1;
extern uchar * label_crpix2;
extern uchar * label_crval1;
extern uchar * label_crval2;
extern uchar * label_naxis;
extern uchar * label_empty;
extern uchar * card_empty;
extern uchar * card_end;

extern uchar * datum_zero;
extern uchar * label_airmass;
extern uchar * label_blank;
extern uchar * label_bscale;
extern uchar * label_bzero;
extern uchar * label_cdelt1;
extern uchar * label_cdelt2;
extern uchar * label_date_obs;
extern uchar * label_dec;
extern uchar * label_end;
extern uchar * label_exposure;
extern uchar * label_extend;
extern uchar * label_filtband;
extern uchar * label_filter;
extern uchar * label_ha;
extern uchar * label_instrume;
extern uchar * label_lamord;
extern uchar * label_loss;
extern uchar * label_naxis1;
extern uchar * label_naxis2;
extern uchar * label_object;
extern uchar * label_observer;
extern uchar * label_pa;
extern uchar * label_platescl;
extern uchar * label_ra;
extern uchar * label_rnoise;
extern uchar * label_rota;
extern uchar * label_seeing;
extern uchar * label_skyrms;
extern uchar * label_skyval;
extern uchar * label_slitwidt;
extern uchar * label_st;
extern uchar * label_telescop;
extern uchar * label_time;
extern uchar * label_tub;
extern uchar * label_ut;
extern uchar * label_vhelio;
extern uchar * label_vminusi;
extern uchar * card_simple;
extern uchar * card_null;
extern uchar * text_T;
extern uchar * text_F;
#endif

#endif /* __INCsubs_fits_h */
