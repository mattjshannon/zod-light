/*

-Header_File SpiceZdf.h ( CSPICE definitions )

-Abstract

   Define CSPICE data types via typedefs; also define some user-visible
   enumerated types.
      
-Copyright

   Copyright (1997), California Institute of Technology.
   U.S. Government sponsorship acknowledged.

-Required_Reading

   None.
   
-Particulars
   
   CSPICE data types
   =================
   
   To assist with long-term maintainability of CSPICE, NAIF has elected
   to use typedefs to represent data types occurring in argument lists
   and as return values of CSPICE functions. These are:
 
      SpiceBoolean
      SpiceChar
      SpiceDouble
      SpiceInt
      ConstSpiceBoolean
      ConstSpiceChar
      ConstSpiceDouble
      ConstSpiceInt
 
   The SPICE typedefs map in an arguably natural way to ANSI C types:
 
      SpiceBoolean -> int  
      SpiceChar    -> char
      SpiceDouble  -> double
      SpiceInt     -> int or long
      ConstX       -> const X  (X = any of the above types)
 
   The type SpiceInt is a special case: the corresponding type is picked
   so as to be half the size of a double. On most currently supported
   platforms, type double occupies 8 bytes and type long occupies 4 
   bytes.  Other platforms may require a SpiceInt to map to type int.
   The Alpha/Digital Unix platform is an example of the latter case.
   
   While other data types may be used internally in CSPICE, no other
   types appear in the API.
      
      
   CSPICE enumerated types
   =======================
   
   These are provided to enhance readability of the code.
      
      Type name                Value set
      ---------                ---------
      
      _Spicestatus             { SPICEFAILURE = -1,   SPICESUCCESS = 0 }
      
      
-Literature_References

   None.

-Author_and_Institution

   N.J. Bachman       (JPL)
   E.D. Wright        (JPL)
   
-Restrictions

   None.
      
-Version

   -CSPICE Version 3.0.0 18-SEP-1999 (NJB) 
   
      SpiceBoolean implementation changed from enumerated type to 
      typedef mapping to int.
   
   -CSPICE Version 2.0.0 29-JAN-1999 (NJB) 
   
      Made definition of SpiceInt and ConstSpiceInt platform 
      dependent to accommodate the Alpha/Digital Unix platform.
      
      Removed definitions of SpiceVoid and ConstSpiceVoid.
   
   -CSPICE Version 1.0.0 25-OCT-1997 (KRG) (NJB) (EDW)
*/

   #ifndef HAVE_SPICEDEFS_H
   #define HAVE_SPICEDEFS_H
   
   /*
   Include platform definitions, if they haven't been executed already.
   */
   #ifndef HAVE_PLATFORM_MACROS_H
      #include "SpiceZpl.h"
   #endif
   
   /*
   Basic data types. These are defined to be compatible with the
   types used by f2c, and so they follow the Fortran notion of what
   these things are. See the f2c documentation for the details
   about the choices for the sizes of these types.
   */
   typedef char           SpiceChar;
   typedef double         SpiceDouble;
   typedef float          SpiceFloat;
   

   #ifdef CSPICE_ALPHA_DIGITAL_UNIX
   
      typedef int         SpiceInt;
   #else
      typedef long        SpiceInt;
   #endif

   
   typedef const char     ConstSpiceChar;
   typedef const double   ConstSpiceDouble;
   typedef const float    ConstSpiceFloat;


   #ifdef CSPICE_ALPHA_DIGITAL_UNIX
   
      typedef const int   ConstSpiceInt;
   #else
      typedef const long  ConstSpiceInt;
   #endif
   
   
   /*
   More basic data types. These give mnemonics for some other data
   types in C that are not used in Fortran written by NAIF or
   supported by ANSI Fortran 77. These are for use in C functions
   but should not be passed to any C SPICE wrappers, ``*_c.c''
   since they are not Fortran compatible.
   */
   typedef long           SpiceLong;
   typedef short          SpiceShort;

   /*
   Unsigned data types
   */
   typedef unsigned char  SpiceUChar;
   typedef unsigned int   SpiceUInt;
   typedef unsigned long  SpiceULong;
   typedef unsigned short SpiceUShort;

   /*
   Signed data types
   */
   typedef signed char    SpiceSChar;

   /*
   Other basic types
   */
   typedef int            SpiceBoolean;
   typedef const int      ConstSpiceBoolean;
   
   #define SPICETRUE      1
   #define SPICEFALSE     0
      
   
   enum _Spicestatus { SPICEFAILURE = -1, SPICESUCCESS = 0 };
   
   typedef enum _Spicestatus SpiceStatus;


#endif
