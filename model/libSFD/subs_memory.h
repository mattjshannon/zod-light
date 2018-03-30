/* subs_memory.h   for Schlegel dust model subs_memory.c routines.       */
/*  file modified 24,25may1999 by J.Bennett @IPAC to remove unused routines */

#ifndef __INCsubs_memory_h
#define __INCsubs_memory_h

typedef size_t MEMSZ;

#ifdef OLD_SUNCC
void memmove
  (void  *  s,
   const void  *  ct,
   MEMSZ    n);
#endif
void ccalloc_
  (MEMSZ *  pMemSize,
   void  ** ppData);
void ccfree_
  (void  ** ppData);

#endif /* __INCsubs_memory_h */
