/*
 CSPICE private macro file.

-Particulars

   Current list of macros (spelling counts)

     MOVED
     MOVEI
     TolOrFail
     MaxVal
     MinVal
     MaxAbs
     MinAbs
     SpiceError
     Even
     Odd
     Index values
     Constants
     BLANK
     CHKFSTR
     CHKFSTR_VAL
     CHKPTR
     CHKOSTR
     CHKOSTR_VAL

-Restrictions

   This is a private macro file for use within CSPICE.
   Do not use or alter any entry.  Or else!

-Author_and_Institution
 
   N.J. Bachman    (JPL)
   E.D. Wright     (JPL) 

-Version
 
   -CSPICE Version 3.0.0, 09-JAN-1998   (NJB)
   
       Added output string check macros CHKOSTR and CHKOSTR_VAL.  
       Removed variable name arguments from macros
       
          CHKPTR
          CHKPTR_VAL
          CHKFSTR
          CHKRSTR_VAL
          
       The strings containing names of the checked variables are now 
       generated from the variables themselves via the # operator.
       
   -CSPICE Version 2.0.0, 03-DEC-1997   (NJB)
   
       Added pointer check macro CHKPTR and Fortran string check macro 
       CHKFSTR.
       
   -CSPICE Version 1.0.0, 25-OCT-1997   (EDW)
*/



#include <math.h>
#include <string.h>
#include "SpiceZdf.h"


#define MOVED( arrfrm, ndim, arrto )                \
                                                    \
        ( memmove ( (void*)               (arrto) , \
                    (void*)               (arrfrm), \
                    sizeof (SpiceDouble) * (ndim)  ) )





#define MOVEI( arrfrm, ndim, arrto )               \
                                                   \
        ( memmove ( (void*)            (arrto) ,   \
                    (void*)            (arrfrm),   \
                    sizeof (SpiceInt) * (ndim)    ) )





/* 
Define a tolerance test for those pesky double precision reals.
True if the difference is less than the tolerance, false otherwise.
The tolerance refers to a percentage. x, y and tol should be declared
double.  All values are assumed to be non-zero.  Okay?
*/

#define TolOrFail( x, y, tol )            \
                                          \
        ( fabs( x-y ) < ( tol * fabs(x) ) )





/*
Simple error output through standard SPICE error system .  Set the error
message and the type
*/

#define SpiceError( errmsg, errtype )   \
                                        \
        {                               \
        setmsg_c ( errmsg  );           \
        sigerr_c ( errtype );           \
        }






/*
Return a value which is the maximum/minimum of the absolute values of
two values.
*/

#define MaxAbs(a,b)   ( fabs(a) >= fabs(b) ? fabs(a) : fabs(b) )
#define MinAbs(a,b)   ( fabs(a) <  fabs(b) ? fabs(a) : fabs(b) )





/*
Return a value which is the maximum/minimum value of two values.
*/

#define MaxVal(A,B) ( (A) >= (B) ? (A) : (B) )
#define MinVal(A,B) ( (A) <  (B) ? (A) : (B) )





/*
Determine whether a value is even or odd
*/
#define Even( x ) ( ( (x) & 1 ) == 0 )
#define Odd ( x ) ( ( (x) & 1 ) != 0 )





/*
Array indexes for vectors.
*/

#define  SpiceX         0
#define  SpiceY         1
#define  SpiceZ         2
#define  SpiceVx        3
#define  SpiceVy        4
#define  SpiceVz        5




/*
Physical constants and dates.
*/

#define  B1900     2415020.31352
#define  J1900     2415020.0
#define  JYEAR     31557600.0
#define  TYEAR     31556925.9747
#define  J1950     2433282.5
#define  SPD       86400.0
#define  B1950     2433282.42345905
#define  J2100     2488070.0
#define  CLIGHT    299792.458
#define  J2000     2451545.0





/*
Common literal values.
*/

#define  NULLCHAR  ( (SpiceChar   )   0 )
#define  NULLCPTR  ( (SpiceChar * )   0 )
#define  BLANK     ( (SpiceChar   ) ' ' )



/*
Macro CHKPTR is used for checking for a null pointer.  CHKPTR uses
the constants 

   CHK_STANDARD
   CHK_DISCOVER
   CHK_REMAIN

to control tracing behavior.  Values  and meanings are:

   CHK_STANDARD          Standard tracing.  If an error
                         is found, signal it, check out
                         and return.

   CHK_DISCOVER          Discovery check-in.  If an
                         error is found, check in, signal
                         the error, check out, and return.

   CHK_REMAIN            If an error is found, signal it.
                         Do not check out or return.  This
                         would allow the caller to clean up
                         before returning, if necessary.
                         In such cases the caller must test
                         failed_c() after the macro call.
                         
CHKPTR should be used in void functions.  In non-void functions,
use CHKPTR_VAL, which is defined below.

*/

#define  CHK_STANDARD   1
#define  CHK_DISCOVER   2
#define  CHK_REMAIN     3

#define  CHKPTR( errHandling, modname, pointer )                     \
                                                                     \
         if ( (void *)(pointer) == (void *)0 )                       \
         {                                                           \
            if ( (errHandling) == CHK_DISCOVER  )                    \
            {                                                        \
               chkin_c ( modname );                                  \
            }                                                        \
                                                                     \
            setmsg_c ( "Pointer \"#\" is null; a non-null "          \
                       "pointer is required."             );         \
            errch_c  ( "#", (#pointer)                    );         \
            sigerr_c ( "SPICE(NULLPOINTER)"               );         \
                                                                     \
            if (    ( (errHandling) == CHK_DISCOVER  )               \
                 || ( (errHandling) == CHK_STANDARD  )   )           \
            {                                                        \
               chkout_c ( modname );                                 \
               return;                                               \
            }                                                        \
         }


#define  CHKPTR_VAL( errHandling, modname, pointer, retval )         \
                                                                     \
         if ( (void *)(pointer) == (void *)0 )                       \
         {                                                           \
            if ( (errHandling) == CHK_DISCOVER  )                    \
            {                                                        \
               chkin_c ( modname );                                  \
            }                                                        \
                                                                     \
            setmsg_c ( "Pointer \"#\" is null; a non-null "          \
                       "pointer is required."             );         \
            errch_c  ( "#", (#pointer)                    );         \
            sigerr_c ( "SPICE(NULLPOINTER)"               );         \
                                                                     \
            if (    ( (errHandling) == CHK_DISCOVER  )               \
                 || ( (errHandling) == CHK_STANDARD  )   )           \
            {                                                        \
               chkout_c ( modname );                                 \
               return   ( retval  );                                 \
            }                                                        \
         }


/*
Macro CHKFSTR checks strings that are to be passed to Fortran or
f2c'd Fortran routines.  Such strings must have non-zero length,
and their pointers must be non-null.

CHKFSTR should be used in void functions.  In non-void functions,
use CHKFSTR_VAL, which is defined below.
*/

#define  CHKFSTR( errHandling, modname, string )                     \
                                                                     \
         CHKPTR ( errHandling, modname, string );                    \
                                                                     \
         if (     (  (void *)string    !=  (void *)0  )              \
              &&  (   strlen(string)   ==  0          )  )           \
         {                                                           \
            if ( (errHandling) == CHK_DISCOVER  )                    \
            {                                                        \
               chkin_c ( modname );                                  \
            }                                                        \
                                                                     \
            setmsg_c ( "String \"#\" has length zero." );            \
            errch_c  ( "#", (#string)                  );            \
            sigerr_c ( "SPICE(EMPTYSTRING)"            );            \
                                                                     \
            if (    ( (errHandling) == CHK_DISCOVER  )               \
                 || ( (errHandling) == CHK_STANDARD  )   )           \
            {                                                        \
               chkout_c ( modname );                                 \
               return;                                               \
            }                                                        \
         }

#define  CHKFSTR_VAL( errHandling, modname, string, retval )         \
                                                                     \
         CHKPTR_VAL( errHandling, modname, string, retval);          \
                                                                     \
         if (     (  (void *)string    !=  (void *)0  )              \
              &&  (   strlen(string)   ==  0          )  )           \
         {                                                           \
            if ( (errHandling) == CHK_DISCOVER  )                    \
            {                                                        \
               chkin_c ( modname );                                  \
            }                                                        \
                                                                     \
            setmsg_c ( "String \"#\" has length zero." );            \
            errch_c  ( "#", (#string)                  );            \
            sigerr_c ( "SPICE(EMPTYSTRING)"            );            \
                                                                     \
            if (    ( (errHandling) == CHK_DISCOVER  )               \
                 || ( (errHandling) == CHK_STANDARD  )   )           \
            {                                                        \
               chkout_c ( modname );                                 \
               return    ( retval );                                 \
            }                                                        \
         }


/*
Macro CHKOSTR checks output string pointers and the associated
string length values supplied as input arguments.  Output string
pointers must be non-null, and the string lengths must be at
least 2, so Fortran routine can write at least one character to
the output string, and so a null terminator can be appended.
CHKOSTR should be used in void functions.  In non-void functions,
use CHKOSTR_VAL, which is defined below.
*/

#define  CHKOSTR( errHandling, modname, string, length )             \
                                                                     \
         CHKPTR ( errHandling, modname, string );                    \
                                                                     \
         if (     (  (void *)string    !=  (void *)0  )              \
              &&  (   length            <  2          )  )           \
         {                                                           \
            if ( (errHandling) == CHK_DISCOVER  )                    \
            {                                                        \
               chkin_c ( modname );                                  \
            }                                                        \
                                                                     \
            setmsg_c ( "String \"#\" has length #; must be >= 2." ); \
            errch_c  ( "#", (#string)                  );            \
            errint_c ( "#", (length)                   );            \
            sigerr_c ( "SPICE(STRINGTOOSHORT)"         );            \
                                                                     \
            if (    ( (errHandling) == CHK_DISCOVER  )               \
                 || ( (errHandling) == CHK_STANDARD  )   )           \
            {                                                        \
               chkout_c ( modname );                                 \
               return;                                               \
            }                                                        \
         }


#define  CHKOSTR_VAL( errHandling, modname, string, length, retval ) \
                                                                     \
         CHKPTR_VAL( errHandling, modname, string, retval );         \
                                                                     \
         if (     (  (void *)string    !=  (void *)0  )              \
              &&  (   length            <  2          )  )           \
         {                                                           \
            if ( (errHandling) == CHK_DISCOVER  )                    \
            {                                                        \
               chkin_c ( modname );                                  \
            }                                                        \
                                                                     \
            setmsg_c ( "String \"#\" has length #; must be >= 2." ); \
            errch_c  ( "#", (#string)                  );            \
            errint_c ( "#", (length)                   );            \
            sigerr_c ( "SPICE(STRINGTOOSHORT)"         );            \
                                                                     \
            if (    ( (errHandling) == CHK_DISCOVER  )               \
                 || ( (errHandling) == CHK_STANDARD  )   )           \
            {                                                        \
               chkout_c ( modname );                                 \
               return   ( retval  );                                 \
            }                                                        \
         }


