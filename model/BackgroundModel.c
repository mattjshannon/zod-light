/*****************************************************************************
 * Copyright (C) 1999 California Institute of Technology. All rights reserved
 * US Government Sponsorship under NASA contract NAS7-918 is acknowledged
 ****************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SpiceUsr.h"
#include "SpiceZfc.h"

#define LONGMSG_LEN 1840
#define SHORTMSG_LEN 80

extern char *observer;

int get_bg_init(char *pathname);
int get_bg(double ra, double dec, double wl, double day, int year, double *zody, double *ism, double *cib, double *totbg, int *ES_warn);
int get_bg_fini();
char *compose_error_message();

static char schlegel_path[300];

int main(argc, argv)
int argc;
char *argv[];
{
    char ipath[360];
    int retval;
    int ES_warn;
    double zody,ism, cib, totbg;
    double ra, dec, wl, day, year;
    char *error_msg;
    char error_buffer[80];

    if (argc != 7)
    {
	printf("usage: BackgroundModel <ra> <dec> <wl> <day> <year> <observer>\n");
	printf("  where ra,dec are decimal degrees\n");
	printf("  wl is in microns, day is 1-366.999, year is 1901-2099\n");
	printf("  and observer is sirtf, -79, planck, etc.\n");
	printf("Example: for background at 1h 1d 160um Jan 9, 2004:\n");
	printf("BackgroundModel 15 1 160.0 9 2004 sirtf\n");
	exit(1);
    }

    ra = atof(argv[1]);
    dec = atof(argv[2]);
    wl = atof(argv[3]);
    day = atof(argv[4]);
    year = atof(argv[5]);
    observer = argv[6];

    strcpy(ipath, "schlegel");
    get_bg_init(ipath);
    strcpy(schlegel_path, ipath);

    furnsh_c ( "kernel.list" );

    retval = get_bg(ra, dec, wl, day, year, &zody, &ism, &cib, &totbg, &ES_warn);
  if (retval != 0)
  {
    fprintf(stdout,"BackgroundModel.c: ERROR from get_bg = %d\n",retval);
    fflush(stdout);

    switch(retval)
    {
	case -101:
	    error_msg = compose_error_message();
	    break;
	case -100:
	    error_msg = "Year not in range 1901-2099";
	    break;
	case -1:
	    error_msg = "Glon outside range 0:360 or glat outside range -90:90";
	    break;
	case -2:
	    error_msg = "wavelength <= 0.0";
	    break;
	case -9:
	    sprintf(error_buffer, 
    "Could not find Schlegel interstellar dust map FITS files in directory %s",
		schlegel_path);
	    error_msg = error_buffer;
	    break;
	default:
	    sprintf(error_buffer, "unknown error code: %d", retval);
	    error_msg = error_buffer;
	    break;
    }
    printf("%s\n", error_msg);
    return(1);
  }

    printf("zody = %f\n", zody);
    printf("ism = %f\n", ism);
    printf("cib = %f\n", cib);
    printf("totbg = %f\n", totbg);
    get_bg_fini();
    return(0);
}


char *compose_error_message()

{
    static char retval[LONGMSG_LEN + 1];
    char longmsg[LONGMSG_LEN + 1];
    char shortmsg[SHORTMSG_LEN + 1];
    int i;

    getsms_(shortmsg, SHORTMSG_LEN);
    for (i = SHORTMSG_LEN - 1; i > 0; i--)
    {
	if (shortmsg[i] != ' ')
	    break;
    }
    shortmsg[i+1] = '\0';

    getlms_(longmsg, LONGMSG_LEN);
    reset_c();
    for (i = LONGMSG_LEN - 1; i > 0; i--)
    {
	if (longmsg[i] != ' ')
	    break;
    }
    longmsg[i+1] = '\0';
    sprintf(retval, "CSPICE ERROR: %s: %s", shortmsg, longmsg);
    return(retval);
}
