/* subs_memory.c  for Schlegel routines                                    */
/*   modified 24,25May1999 by J.Bennett @IPAC to remove unused routines.   */

#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* Library for malloc(), realloc() and free() */
#include "subs_memory.h"

#ifdef OLD_SUNCC
/******************************************************************************/
/*
 * Copy one section of memory (a string) to another, even if they overlap.
 * The advertised C library routine by this name does not actually exist
 * in old SunOS.
 */
void memmove
  (void  *  s,
   const void  *  ct,
   MEMSZ    n)
{
   MEMSZ    i;
   char  *  ps = (char *)s;
   const char  *  pct = (const char *)ct;
 
   /* Do nothing if ps == pct */
   if (ps > pct) for (i=0; i < n; i++) *(ps+n-i-1) = *(pct+n-i-1);
   else if (ps < pct) for (i=0; i < n; i++) *(ps+i) = *(pct+i);
}
#endif

/******************************************************************************/
/*
 * Allocate *pMemSize bytes of data.  The starting memory location is *ppData.
 */
void ccalloc_
  (MEMSZ *  pMemSize,
   void  ** ppData)
{
   *ppData = (void *)malloc((size_t)(*pMemSize));
}

/******************************************************************************/
/*
 * Free the memory block that starts at address *ppData.
 */
void ccfree_
  (void  ** ppData)
{
   free(*ppData);
   *ppData = NULL;
}

/******************************************************************************/

