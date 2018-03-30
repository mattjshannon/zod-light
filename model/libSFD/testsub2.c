/* testsub2.c to test new subroutine version of dust_getval */
/*    tests getting E(B-V) magnitudes from Ebv map and      */
/*                  mask values from mask map.              */
/*  J.Bennett @ IPAC  07June1999                            */

/*  link with SFD routines in dust_getval_sub.c, subs_lambert_serv.c,       */
/*                            subs_fits.c, subs_memory.c and subs_inoutput.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main()
{
  double glon, glat;
  float  val;
  int retcode;

  char inpath[] = "/proj/ssc/Data/IRAC/SFD";
  int interp = 1;                            /* interpolate pixels */
  int jmap = 0;                              /* Ebv */
  int hcons;
  char mask_chars[6][8];

  int dust_getval_sub();
  int dust_getval_init();
  int dust_getval_fini();
  void trans_mask_bits();

  if ((retcode = dust_getval_init(inpath)) != 0)
  {
    fprintf(stdout, "ERROR from dust_getval_init = %d.\n", retcode);
    fflush(stdout);
    exit(9);
  }
   /* First get E(B-V) (using jmap = 0) values fora a couple of cases: */

  glon = 121.0;
  glat = -21.5;
  retcode = dust_getval_sub(glon,glat,interp,jmap, &val);
  fprintf(stdout,
    "glon, glat, E(B-V)mag.,retcode = %7.2f %7.2f %10.5f %d\n",
    glon,glat,val,retcode);

  glon = 12.0;
  glat = +34.5;
  retcode = dust_getval_sub(glon,glat,interp,jmap, &val);
  fprintf(stdout,
    "glon, glat, E(B-V)mag.,retcode = %7.2f %7.2f %10.5f %d\n",
    glon,glat,val,retcode);

    /* Now, set jmap = 4 to get mask fora a case: */

  glon = 121.0;
  glat = -21.5;
  jmap = 4;      /* Mask */
  retcode = dust_getval_sub(glon,glat,interp,jmap, &val);
  if (retcode != 0)
    fprintf(stdout, "retcode = %d\n",retcode);
  trans_mask_bits( &val, &hcons, mask_chars);
  fprintf(stdout,
    "glon, glat, hcons, mask_chars = %7.2f %7.2f\n %d %s %s %s %s %s %s\n",
    glon,glat,hcons,mask_chars[0],mask_chars[1],mask_chars[2],mask_chars[3],
    mask_chars[4],mask_chars[5]);


  if ((retcode = dust_getval_fini()) != 0)
  {
    fprintf(stdout,"ERROR from dust_getval_fini = %d.\n", retcode);
    fflush(stdout);
    exit(9);
  }
  exit(0);
}
