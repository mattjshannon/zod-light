/*  bg_init.c                                                              */ 
/*    to fill struct ztrig_s ztrig with pi, radians_to_degree conv. values.*/

/*    08feb1999   j.bennett @ IPAC                                         */

#include <math.h>
#include "ztrig_s.h"

struct ztrig_s ztrig;           /* struct described in ztrig_s.h  */

int bg_init()
{
  extern struct ztrig_s ztrig;

  ztrig.dtor = atan(1.0) / 45.0;
  ztrig.rtod = 1.0 / ztrig.dtor;
  ztrig.pi = 180.0 * ztrig.dtor;
  ztrig.pi2 = 2.0 * ztrig.pi;
  return(0);
}
