/*

-Header_File SpicePln.h ( CSPICE Plane definitions )

-Abstract

   Perform CSPICE definitions for the SpicePlane data type.
            
-Copyright

   Copyright (1999), California Institute of Technology.
   U.S. Government sponsorship acknowledged.

-Required_Reading

   None.
   
-Particulars

   This header defines structures and typedefs that may be referenced in 
   application code that calls CSPICE Plane functions.
   

      Structures
      ==========
   
         Name                  Description
         ----                  ----------
   
         SpicePlane            Structure representing a plane in 3-
                               dimensional space.
         
                               The members are:
 
                                  normal:     Vector normal to plane.

                                  constant:   Constant of plane equation
                                       
                                              Plane =  
                                              
                                              {X: <normal,X> = constant}



         ConstSpicePlane       A const SpicePlane.
         
         
-Literature_References

   None.

-Author_and_Institution

   N.J. Bachman       (JPL)
   
-Restrictions

   None.
      
-Version

   -CSPICE Version 1.0.0, 04-MAR-1999 (NJB)  

*/

#ifndef HAVE_SPICE_PLANES

   #define HAVE_SPICE_PLANES
   
   
   
   /*
   Plane structure:
   */
   
   struct _SpicePlane 
   
      { SpiceDouble      normal   [3];
        SpiceDouble      constant;     };
          
   typedef struct _SpicePlane  SpicePlane;

   typedef const SpicePlane    ConstSpicePlane;
 
#endif

