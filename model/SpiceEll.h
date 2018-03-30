/*

-Header_File SpiceEll.h ( CSPICE Ellipse definitions )

-Abstract

   Perform CSPICE definitions for the SpiceEllipse data type.
            
-Copyright

   Copyright (1999), California Institute of Technology.
   U.S. Government sponsorship acknowledged.

-Required_Reading

   None.
   
-Particulars

   This header defines structures and typedefs that may be referenced in 
   application code that calls CSPICE Ellipse functions.
   

      Structures
      ==========
   
         Name                  Description
         ----                  ----------
   
         SpiceEllipse          Structure representing an ellipse in 3-
                               dimensional space.
         
                               The members are:
 
                                  center:     Vector defining ellipse's
                                              center.

                                  semiMajor:  Vector defining ellipse's
                                              semi-major axis.
                                       
                                  semiMinor:  Vector defining ellipse's
                                              semi-minor axis.
                                       
                               The ellipse is the set of points
                               
                                 {X:  X =                  center 
                                            + cos(theta) * semiMajor
                                            + sin(theta) * semiMinor,
                                            
                                  theta in [0, 2*Pi) }


         ConstSpiceEllipse     A const SpiceEllipse.
         
         
-Literature_References

   None.

-Author_and_Institution

   N.J. Bachman       (JPL)
   
-Restrictions

   None.
      
-Version

   -CSPICE Version 1.0.0, 04-MAR-1999 (NJB)  

*/

#ifndef HAVE_SPICE_ELLIPSES

   #define HAVE_SPICE_ELLIPSES
   
   
   
   /*
   Ellipse structure:
   */
   
   struct _SpiceEllipse 
   
      { SpiceDouble      center    [3];
        SpiceDouble      semiMajor [3];     
        SpiceDouble      semiMinor [3];  };
          
   typedef struct _SpiceEllipse  SpiceEllipse;

   typedef const SpiceEllipse    ConstSpiceEllipse;
 
#endif

