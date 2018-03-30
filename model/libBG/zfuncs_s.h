/*  zfuncs_s.h     08feb1999     j.bennett @ IPAC                          */

struct zfuncs_s
{
  double elonr;           /* ecliptic longitude in radians  */
  double elatr;           /* ecliptic latitude in radians   */
  double cos_elon;        /* cos(elonr)                     */
  double sin_elon;        /* sin(elonr)                     */
  double cos_elat;        /* cos(elatr)                     */
  double sin_elat;        /* sin(elatr)                     */
  double cos_theta;       /* cos(Theta)                     */
  double sin_theta;       /* sin(Theta)                     */
};
