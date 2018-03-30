/* subs_lambert_serv.h modification of subs_lambert.h for                  */
/*     subs_lambert_serv.c, dust_getval_serv.c, dust_getval_subs.c         */
/*  for this mod., 25May1999 by J.Bennett @ IPAC, lambert_lb2xy declaration*/
/*    has been removed from this file - appears as static in approp. files.*/

/*  func lambert_lb2fpix and lambert_lb2pix declarations made static and   */
/*   moved to subs_lambert_serv.c file.                                    */

#ifndef __INCsubs_lambert_h
#define __INCsubs_lambert_h

float lambert_getval_serv
  (char  *  pFileN,
   char  *  pFileS,
   int      nGal,
   float *  pGall,
   float *  pGalb,
   int      qInterp,
   int      qVerbose);
#if 0
extern uchar * label_lam_nsgp;
extern uchar * label_lam_scal;
#endif

#endif /* __INCsubs_lambert_h */
