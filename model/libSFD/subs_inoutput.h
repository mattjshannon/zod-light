/*  subs_inoutput.h    for Schlegel's routines  */
/* modified 04Feb2000 by J.Bennett to define MAX_FILE_NAME_LEN and        */
/*   IO_FORTRAN_FL with 256 instead of 80 (in spite of original comment   */
/*   about IO_FORTRAN_FL being used from a FORTRAN call, it appears this  */
/*   declaration is used with normal C calls too).                        */
/*  modified 24May1999 by J.Bennett to remove inoutput_free_file_pointer */
/*         declaration from this file and put it in .c file (making func */
/*         static).                                                      */

#ifndef __INCsubs_inoutput_h
#define __INCsubs_inoutput_h

#include <stdio.h> /* Include for definition of FILE */

#define MAX_FILE_LINE_LEN 500 /* Maximum line length for data files */
#define MAX_FILE_NAME_LEN 256
#define IO_FOPEN_MAX       20  /* Files must be numbered 0 to IO_FOPEN_MAX-1 */
#define IO_FORTRAN_FL     256  /* Max length of file name from a Fortran call */
#define IO_GOOD             1
#define IO_BAD              0

extern FILE  *  pFILEfits[];

int inoutput_file_exist
  (char  *  pFileName);
int inoutput_open_file
  (int   *  pFilenum,
   char     pFileName[],
   char     pPriv[]);
int inoutput_close_file
  (int      filenum);

#endif /* __INCsubs_inoutput_h */
