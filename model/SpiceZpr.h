/*

-Header_File SpiceZpr.h ( CSPICE prototypes )

-Abstract

   Define prototypes for CSPICE user-interface-level functions.

-Copyright

   Copyright (1997), California Institute of Technology.
   U.S. Government sponsorship acknowledged.

-Required_Reading

   None.

-Literature_References

   None.

-Particulars

   This is the header file containing prototypes for CSPICE user-level
   C routines.  Prototypes for the underlying f2c'd SPICELIB routines
   are contained in the separate header file SpiceZfc.  However, those
   routines are not part of the official CSPICE API.

-Author_and_Institution

   N.J. Bachman       (JPL)
   K.R. Gehringer     (JPL)
   W.L. Taber         (JPL)
   F.S. Turner        (JPL)
   E.D. Wright        (JPL)

-Version


   -CSPICE Version 12.2.0, 23-OCT-2001 (NJB)

      Added prototypes for

         badkpv_c
         dcyldr_c
         dgeodr_c
         dlatdr_c
         drdcyl_c
         drdgeo_c
         drdlat_c
         drdsph_c
         dsphdr_c
         ekacec_c
         ekaced_c
         ekacei_c
         ekappr_c
         ekbseg_c
         ekccnt_c
         ekcii_c
         ekdelr_c
         ekinsr_c
         ekntab_c
         ekrcec_c
         ekrced_c
         ekrcei_c
         ektnam_c
         ekucec_c
         ekuced_c
         ekucei_c
         inelpl_c
         invort_c
         kxtrct_c

      Added const qualifier to input array arguments of

         conics_c
         illum_c
         pdpool_c
         prop2b_c
         q2m_c
         spkuds_c
         xposeg_c

      Added const qualifier to the return value of

         tkvrsn_c
    
   -CSPICE Version 12.1.0, 12-APR-2000 (FST)

      Added prototype for

         getfov_c

   -CSPICE Version 12.0.0, 22-MAR-2000 (NJB)
   
      Added prototypes for
         
         lparse_c
         lparsm_c      
         spkw12_c
         spkw13_c
         

   -CSPICE Version 11.1.0, 17-DEC-1999 (WLT)
   
      Added prototype for
               
         dafrda_c

   -CSPICE Version 11.0.0, 07-OCT-1999 (NJB) (EDW) 

      Changed ekaclc_c, ekacld_c, ekacli_c prototypes to make input
      pointers const-qualified where appropriate.
      
      Changed prompt_c prototype to accommodate memory leak bug fix.
      
      Changed ekpsel_c prototype to be consistent with other interfaces
      having string array outputs.
      
      Added prototypes for
               
         axisar_c
         brcktd_c
         brckti_c
         cidfrm_c
         cgv2el_c
         clpool_c
         cmprss_c
         cnmfrm_c
         convrt_c
         cvpool_c
         dafbbs_c
         dafbfs_c
         dafcls_c
         dafcs_c
         daffna_c
         daffpa_c
         dafgh_c
         dafgn_c
         dafgs_c
         dafopr_c
         dafps_c
         dafus_c
         diags2_c
         dtpool_c
         dvdot_c
         dvhat_c
         dvpool_c
         edlimb_c
         ekops_c
         ekopw_c
         eul2xf_c
         ftncls_c
         furnsh_c
         getmsg_c
         getelm_c
         gnpool_c
         ident_c
         illum_c
         inedpl_c
         kdata_c
         kinfo_c
         ktotal_c
         lmpool_c
         matchi_c
         matchw_c
         maxd_c
         maxi_c
         mequ_c
         mind_c
         mini_c
         moved_
         npedln_c
         npelpt_c
         nplnpt_c
         pcpool_c
         pdpool_c
         pipool_c
         pjelpl_c
         pxform_c
         rav2xf_c
         raxisa_c
         rquad_c
         saelgv_c
         spk14a_c
         spk14b_c
         spk14e_c
         spkapp_c
         spkapo_c
         spkcls_c
         spkezp_c
         spkgps_c
         spkopn_c
         spkpds_c
         spkpos_c
         spkssb_c
         spksub_c
         spkuds_c
         spkw02_c
         spkw03_c
         spkw05_c
         spkw08_c
         spkw09_c
         spkw10_c
         spkw15_c
         spkw17_c
         stpool_c
         subpt_c
         subsol_c
         swpool_c
         szpool_c
         tparse_c
         trace_c
         unload_c
         vaddg_c
         vhatg_c
         vlcomg_c
         vminug_c
         vrel_c
         vrelg_c
         vsepg_c
         vtmv_c
         vtmvg_c
         vzerog_c
         xf2eul_c
         xf2rav_c
         xposeg_c
         
          
   -CSPICE Version 10.0.0, 09-MAR-1999 (NJB)  

      Added prototypes for
      
         frame_c
         inrypl_c
         nvc2pl_c
         nvp2pl_c
         pl2nvc_c
         pl2nvp_c
         pl2psv_c
         psv2pl_c
         sce2c_c
         vprjp_c
         vprjpi_c

      Now conditionally includes SpiceEll.h and SpicePln.h.
      
      
   -CSPICE Version 9.0.0, 25-FEB-1999 (NJB)

      Added prototypes for

         eknseg_c
         eknelt_c
         ekpsel_c
         ekssum_c
         
      Now conditionally includes SpiceEK.h.
      
      
   -CSPICE Version 8.0.0, 20-OCT-1998 (NJB)

      Added const qualifier to all input matrix and vector arguments.
      
      Added prototypes for

         det_c
         dpmax_c
         dpmax_
         dpmin_c
         dpmin_
         frinfo_c
         frmnam_c
         getfat_c
         intmax_c
         intmax_
         intmin_c
         intmin_
         invert_c
         namfrm_c
         vrotv_c
         vsclg_c
         
         
   -CSPICE Version 7.0.0, 02-APR-1998 (EDW)

      Added prototypes for

         mequg_c
         unormg_g
         vdistg_c
         vdotg_c
         vequg_c
         vnormg_c

   -CSPICE Version 6.0.0, 31-MAR-1998 (NJB)

      Added prototypes for

         ekaclc_c
         ekacld_c
         ekacli_c
         ekcls_c
         ekffld_c
         ekfind_c
         ekgc_c
         ekgd_c
         ekgi_c
         ekifld_c
         eklef_c
         ekopr_c
         ekopn_c
         ekuef_c

   -CSPICE Version 5.0.1, 05-MAR-1998 (EDW)

      Remove some non printing characters.

   -CSPICE Version 5.0.0, 03-MAR-1998 (NJB)

      Added prototypes for

         etcal_c
         ltime_c
         stelab_c
         tpictr_c
         twovec_c
         vsubg_c

   -CSPICE Version 4.0.0, 11-FEB-1998 (EDW)

      Added prototypes for

         timdef_c
         tsetyr_c


   -CSPICE Version 3.0.0, 02-FEB-1998 (NJB)

      Added prototypes for

         pckuof_c
         tipbod_c

      Type SpiceVoid was replaced with void.

   -CSPICE Version 2.0.0, 06-JAN-1998 (NJB)

      Changed all input-only character pointers to type ConstSpiceChar.

   -CSPICE Version 1.0.0, 25-OCT-1997 (NJB) (KRG) (EDW)

-Index_Entries

   protoypes of CSPICE functions

*/


/*
Include Files:
*/


#ifndef  HAVE_SPICEDEFS_H
#include "SpiceZdf.h"
#endif

#ifndef  HAVE_SPICE_EK_H
#include "SpiceEK.h"
#endif

#ifndef  HAVE_SPICE_PLANES_H
#include "SpicePln.h"
#endif

#ifndef  HAVE_SPICE_ELLIPSES_H
#include "SpiceEll.h"
#endif

#ifndef HAVE_SPICEWRAPPERS_H
#define HAVE_SPICEWRAPPERS_H




/*
   Function prototypes for CSPICE functions are listed below.
   Each prototype is accompanied by a function abstract and brief I/O
   description.

   See the headers of the C wrappers for detailed descriptions of the
   routines' interfaces.

   The list below should be maintained in alphabetical order.
*/


   void              axisar_c ( ConstSpiceDouble   axis   [3],
                                SpiceDouble        angle,
                                SpiceDouble        r      [3][3]  );
                                
                                 
   SpiceBoolean      badkpv_c ( ConstSpiceChar    *caller,
                                ConstSpiceChar    *name,
                                ConstSpiceChar    *comp,
                                SpiceInt           size,
                                SpiceInt           divby,
                                SpiceChar          type   );
                                

   void              bodc2n_c ( SpiceInt           code,
                                SpiceInt           namelen,
                                SpiceChar        * name,
                                SpiceBoolean     * found   );


   void              boddef_c ( ConstSpiceChar   * name,
                                SpiceInt           code );


   SpiceBoolean      bodfnd_c ( SpiceInt           body,
                                ConstSpiceChar   * item );


   void              bodn2c_c ( ConstSpiceChar   * name,
                                SpiceInt         * code,
                                SpiceBoolean     * found );


   void              bodvar_c ( SpiceInt           body,
                                ConstSpiceChar   * item,
                                SpiceInt         * dim ,
                                SpiceDouble      * values );


   SpiceDouble       brcktd_c ( SpiceDouble        number,
                                SpiceDouble        end1,
                                SpiceDouble        end2    );
   
   
   SpiceInt          brckti_c ( SpiceInt           number,
                                SpiceInt           end1,
                                SpiceInt           end2    );
   
   
   SpiceDouble       b1900_c  ( void );


   SpiceDouble       b1950_c  ( void );


   void              cgv2el_c ( ConstSpiceDouble    center[3],
                                ConstSpiceDouble    vec1  [3],
                                ConstSpiceDouble    vec2  [3],
                                SpiceEllipse      * ellipse   ); 


   void              chkin_c  ( ConstSpiceChar    * module );


   void              chkout_c ( ConstSpiceChar    * module );


   void              cidfrm_c ( SpiceInt            cent,
                                SpiceInt            lenout,
                                SpiceInt          * frcode,
                                SpiceChar         * frname,
                                SpiceBoolean      * found  );
                                
                                 
   void              ckcls_c  ( SpiceInt            handle );


   void              ckgp_c   ( SpiceInt            inst,
                                SpiceDouble         sclkdp,
                                SpiceDouble         tol,
                                ConstSpiceChar    * ref,
                                SpiceDouble         cmat[3][3],
                                SpiceDouble       * clkout,
                                SpiceBoolean      * found      );


   void              ckgpav_c ( SpiceInt            inst,
                                SpiceDouble         sclkdp,
                                SpiceDouble         tol,
                                ConstSpiceChar    * ref,
                                SpiceDouble         cmat[3][3],
                                SpiceDouble         av[3],
                                SpiceDouble       * clkout,
                                SpiceBoolean      * found      );


   void              cklpf_c  ( ConstSpiceChar    * fname,
                                SpiceInt          * handle    );


   void              ckopn_c  ( ConstSpiceChar    * name,
                                ConstSpiceChar    * ifname,
                                SpiceInt            ncomch,
                                SpiceInt          * handle  );


   void              ckupf_c  ( SpiceInt            handle );


   void              ckw01_c  ( SpiceInt            handle,
                                SpiceDouble         begtime,
                                SpiceDouble         endtime,
                                SpiceInt            inst,
                                ConstSpiceChar    * ref,
                                SpiceBoolean        avflag,
                                ConstSpiceChar    * segid,
                                SpiceInt            nrec,
                                ConstSpiceDouble    sclkdp [],
                                ConstSpiceDouble    quats  [][4],
                                ConstSpiceDouble    avvs   [][3]  );


   void              ckw02_c  ( SpiceInt            handle,
                                SpiceDouble         begtim,
                                SpiceDouble         endtim,
                                SpiceInt            inst,
                                ConstSpiceChar    * ref,
                                ConstSpiceChar    * segid,
                                SpiceInt            nrec,
                                ConstSpiceDouble    start  [],
                                ConstSpiceDouble    stop   [],
                                ConstSpiceDouble    quats  [][4],
                                ConstSpiceDouble    avvs   [][3],
                                ConstSpiceDouble    rates  []    );


   void              ckw03_c  ( SpiceInt            handle,
                                SpiceDouble         begtim,
                                SpiceDouble         endtim,
                                SpiceInt            inst,
                                ConstSpiceChar    * ref,
                                SpiceBoolean        avflag,
                                ConstSpiceChar    * segid,
                                SpiceInt            nrec,
                                ConstSpiceDouble    sclkdp [],
                                ConstSpiceDouble    quats  [][4],
                                ConstSpiceDouble    avvs   [][3],
                                SpiceInt            nints,
                                ConstSpiceDouble    starts []     );


   SpiceDouble       clight_c ( void );


   void              clpool_c ( void );
    

   void              cmprss_c ( SpiceChar           delim,
                                SpiceInt            n,
                                ConstSpiceChar    * input,
                                SpiceInt            lenout,
                                SpiceChar         * output  );
                                
                                
   void              cnmfrm_c ( ConstSpiceChar    * cname,
                                SpiceInt            lenout,
                                SpiceInt          * frcode,
                                SpiceChar         * frname,
                                SpiceBoolean      * found   );
                                
                                 
   void              conics_c ( ConstSpiceDouble    elts[8],
                                SpiceDouble         et,
                                SpiceDouble         state[6] );


   void              convrt_c ( SpiceDouble         x,
                                ConstSpiceChar    * in,
                                ConstSpiceChar    * out,
                                SpiceDouble       * y    );
                                
                                 
   void              cvpool_c ( ConstSpiceChar    * agent,
                                SpiceBoolean      * update );
                                
                                 
   void              cyllat_c ( SpiceDouble         r,
                                SpiceDouble         lonc,
                                SpiceDouble         z,
                                SpiceDouble       * radius,
                                SpiceDouble       * lon,
                                SpiceDouble       * lat );


   void              cylrec_c ( SpiceDouble         r,
                                SpiceDouble         lon,
                                SpiceDouble         z,
                                SpiceDouble         rectan[3] );


   void              cylsph_c ( SpiceDouble         r,
                                SpiceDouble         lonc,
                                SpiceDouble         z,
                                SpiceDouble       * radius,
                                SpiceDouble       * colat,
                                SpiceDouble       * lon );

   void              dafbbs_c ( SpiceInt            handle );
   
   
   void              dafbfs_c ( SpiceInt            handle );
   
   
   void              dafcls_c ( SpiceInt            handle );
   
   
   void              dafcs_c  ( SpiceInt            handle );
   
   
   void              daffna_c ( SpiceBoolean      * found );
   
   
   void              daffpa_c ( SpiceBoolean      * found );
   
   
   void              dafgh_c  ( SpiceInt          * handle );
   
   
   void              dafgn_c  ( SpiceInt            lenout,
                                SpiceChar         * name   );
   
   
   void              dafgs_c  ( SpiceDouble         sum[] );
   
   
   void              dafopr_c ( ConstSpiceChar    * fname,
                                SpiceInt          * handle  );
                                
                                
   void              dafps_c  ( SpiceInt            nd,
                                SpiceInt            ni,
                                ConstSpiceDouble    dc  [],
                                ConstSpiceInt       ic  [],
                                SpiceDouble         sum []  );

                                
   void              dafrda_c ( SpiceInt            handle,
                                SpiceInt            begin,
                                SpiceInt            end,
                                SpiceDouble       * data );


   void              dafus_c  ( ConstSpiceDouble    sum [],
                                SpiceInt            nd,
                                SpiceInt            ni,
                                SpiceDouble         dc  [],
                                SpiceInt            ic  []  );
   
   
   SpiceDouble       det_c    ( ConstSpiceDouble    m1[3][3] );


   void              dcyldr_c ( SpiceDouble         x,
                                SpiceDouble         y,
                                SpiceDouble         z,
                                SpiceDouble         jacobi[3][3] );


   void              dgeodr_c ( SpiceDouble         x,
                                SpiceDouble         y,
                                SpiceDouble         z,
                                SpiceDouble         re,
                                SpiceDouble         f,
                                SpiceDouble         jacobi[3][3] );


   void              dlatdr_c ( SpiceDouble         x,
                                SpiceDouble         y,
                                SpiceDouble         z,
                                SpiceDouble         jacobi[3][3] );
   

   void              drdcyl_c ( SpiceDouble         r,
                                SpiceDouble         lon,
                                SpiceDouble         z,
                      SpiceDouble         jacobi[3][3] );


   void              drdgeo_c ( SpiceDouble         lon,
                                SpiceDouble         lat,
                                SpiceDouble         alt,
                                SpiceDouble         re,
                                SpiceDouble         f,
                                SpiceDouble         jacobi[3][3] );


   void              drdlat_c ( SpiceDouble         r,
                                SpiceDouble         lon,
                                SpiceDouble         lat,
                                SpiceDouble         jacobi[3][3] );


   void              drdsph_c ( SpiceDouble         r,
                                SpiceDouble         colat,
                                SpiceDouble         lon,
                                SpiceDouble         jacobi[3][3] ); 


   void              dsphdr_c ( SpiceDouble         x,
                                SpiceDouble         y,
                                SpiceDouble         z,
                                SpiceDouble         jacobi[3][3] );
  

   void              diags2_c ( ConstSpiceDouble    symmat [2][2],
                                SpiceDouble         diag   [2][2],
                                SpiceDouble         rotate [2][2]  );
                                
                                 
   SpiceDouble       dpmax_c  ( void );


   SpiceDouble       dpmax_   ( void );


   SpiceDouble       dpmin_c  ( void );


   SpiceDouble       dpmin_   ( void );


   SpiceDouble       dpr_c    ( void );


   void              dtpool_c ( ConstSpiceChar      * name,
                                SpiceBoolean        * found,
                                SpiceInt            * n,
                                SpiceChar             type [1] );
                                
                                
   SpiceDouble       dvdot_c  ( ConstSpiceDouble      s1[6], 
                                ConstSpiceDouble      s2[6] );
                                

   void              dvhat_c  ( ConstSpiceDouble      s1[6], 
                                SpiceDouble           sout[6] );
                                
                                
   void              dvpool_c ( ConstSpiceChar      * name );
   
   
   void              edlimb_c ( SpiceDouble           a,
                                SpiceDouble           b,
                                SpiceDouble           c,
                                ConstSpiceDouble      viewpt[3],
                                SpiceEllipse        * limb      );
                                

   void              ekacec_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt              recno,
                                ConstSpiceChar      * column,
                                SpiceInt              nvals,
                                SpiceInt              vallen,
                                const void          * cvals,
                                SpiceBoolean          isnull );


   void              ekaced_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt              recno,
                                ConstSpiceChar      * column,
                                SpiceInt              nvals,
                                ConstSpiceDouble    * dvals,
                                SpiceBoolean          isnull );


   void              ekacei_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt              recno,
                                ConstSpiceChar      * column,
                                SpiceInt              nvals,
                                ConstSpiceInt       * ivals,
                                SpiceBoolean          isnull );

                                
   void              ekaclc_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                ConstSpiceChar      * column,
                                SpiceInt              vallen,
                                const void          * cvals,
                                ConstSpiceInt       * entszs,
                                ConstSpiceBoolean   * nlflgs,
                                ConstSpiceInt       * rcptrs,
                                SpiceInt            * wkindx  );


   void              ekacld_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                ConstSpiceChar      * column,
                                ConstSpiceDouble    * dvals,
                                ConstSpiceInt       * entszs,
                                ConstSpiceBoolean   * nlflgs,
                                ConstSpiceInt       * rcptrs,
                                SpiceInt            * wkindx  );


   void              ekacli_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                ConstSpiceChar      * column,
                                ConstSpiceInt       * ivals,
                                ConstSpiceInt       * entszs,
                                ConstSpiceBoolean   * nlflgs,
                                ConstSpiceInt       * rcptrs,
                                SpiceInt            * wkindx  );


   void              ekappr_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt            * recno  );


   void              ekbseg_c ( SpiceInt              handle,
                                ConstSpiceChar      * tabnam,
                                SpiceInt              ncols,
                                SpiceInt              cnmlen,
                                const void          * cnames,
                                SpiceInt              declen,
                                const void          * decls,
                                SpiceInt            * segno  );


   void              ekccnt_c ( ConstSpiceChar      * table,
                                SpiceInt            * ccount );


   void              ekcii_c  ( ConstSpiceChar      * table,
                                SpiceInt              cindex,
                                SpiceInt              lenout,
                                SpiceChar           * column,
                                SpiceEKAttDsc       * attdsc  );
 

   void              ekcls_c  ( SpiceInt              handle );


   void              ekdelr_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt              recno );


   void              ekffld_c ( SpiceInt            handle,
                                SpiceInt            segno,
                                SpiceInt          * rcptrs );


   void              ekfind_c ( ConstSpiceChar    * query,
                                SpiceInt            lenout,
                                SpiceInt          * nmrows,
                                SpiceBoolean      * error,
                                SpiceChar         * errmsg );


   void              ekgc_c   ( SpiceInt            selidx,
                                SpiceInt            row,
                                SpiceInt            elment,
                                SpiceInt            lenout,
                                SpiceChar         * cdata,
                                SpiceBoolean      * null,
                                SpiceBoolean      * found  );


   void              ekgd_c   ( SpiceInt            selidx,
                                SpiceInt            row,
                                SpiceInt            elment,
                                SpiceDouble       * ddata,
                                SpiceBoolean      * null,
                                SpiceBoolean      * found  );


   void              ekgi_c   ( SpiceInt            selidx,
                                SpiceInt            row,
                                SpiceInt            elment,
                                SpiceInt          * idata,
                                SpiceBoolean      * null,
                                SpiceBoolean      * found  );


   void              ekifld_c ( SpiceInt            handle,
                                ConstSpiceChar    * tabnam,
                                SpiceInt            ncols,
                                SpiceInt            nrows,
                                SpiceInt            cnmlen,
                                const void        * cnames,
                                SpiceInt            declen,
                                const void        * decls,
                                SpiceInt          * segno,
                                SpiceInt          * rcptrs );


   void              ekinsr_c ( SpiceInt            handle,
                                SpiceInt            segno,
                                SpiceInt            recno );


   void              eklef_c  ( ConstSpiceChar    * fname,
                                SpiceInt          * handle );


   SpiceInt          eknelt_c ( SpiceInt            selidx,
                                SpiceInt            row     );
                                
                                 
   SpiceInt          eknseg_c ( SpiceInt            handle );


   void              ekntab_c ( SpiceInt            * n );


   void              ekopn_c  ( ConstSpiceChar    * fname,
                                ConstSpiceChar    * ifname,
                                SpiceInt            ncomch,
                                SpiceInt          * handle );


   void              ekopr_c  ( ConstSpiceChar    * fname,
                                SpiceInt          * handle );


   void              ekops_c  ( SpiceInt          * handle );


   void              ekopw_c  ( ConstSpiceChar    * fname,
                                SpiceInt          * handle );
                                
                                
   void              ekpsel_c ( ConstSpiceChar    * query,
                                SpiceInt            msglen,
                                SpiceInt            tablen,
                                SpiceInt            collen,
                                SpiceInt          * n,
                                SpiceInt          * xbegs,
                                SpiceInt          * xends,
                                SpiceEKDataType   * xtypes,
                                SpiceEKExprClass  * xclass,
                                void              * tabs,
                                void              * cols,
                                SpiceBoolean      * error,
                                SpiceChar         * errmsg ); 
                   

   void              ekrcec_c ( SpiceInt           handle,
                                SpiceInt           segno,
                                SpiceInt           recno,
                                ConstSpiceChar   * column,
                                SpiceInt           lenout,
                                SpiceInt         * nvals,
                                void             * cvals,
                                SpiceBoolean     * isnull );


   void              ekrced_c ( SpiceInt           handle,
                                SpiceInt           segno,
                                SpiceInt           recno,
                                ConstSpiceChar   * column,
                                SpiceInt         * nvals,
                                SpiceDouble      * dvals,
                                SpiceBoolean     * isnull );


   void              ekrcei_c ( SpiceInt           handle,
                                SpiceInt           segno,
                                SpiceInt           recno,
                                ConstSpiceChar   * column,
                                SpiceInt         * nvals,
                                SpiceInt         * ivals,
                                SpiceBoolean     * isnull );

                   
   void              ekssum_c ( SpiceInt            handle,
                                SpiceInt            segno,
                                SpiceEKSegSum     * segsum );
                                

   void              ektnam_c ( SpiceInt            n,
                                SpiceInt            lenout,
                                SpiceChar         * table  );


   void              ekucec_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt              recno,
                                ConstSpiceChar      * column,
                                SpiceInt              nvals,
                                SpiceInt              vallen,
                                const void          * cvals,
                                SpiceBoolean          isnull );


   void              ekuced_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt              recno,
                                ConstSpiceChar      * column,
                                SpiceInt              nvals,
                                ConstSpiceDouble    * dvals,
                                SpiceBoolean          isnull );


   void              ekucei_c ( SpiceInt              handle,
                                SpiceInt              segno,
                                SpiceInt              recno,
                                ConstSpiceChar      * column,
                                SpiceInt              nvals,
                                ConstSpiceInt       * ivals,
                                SpiceBoolean          isnull );

                                
   void              ekuef_c  ( SpiceInt            handle );


   SpiceBoolean      eqstr_c  ( ConstSpiceChar    * a,
                                ConstSpiceChar    * b  );


   void              el2cgv_c ( ConstSpiceEllipse * ellipse,
                                SpiceDouble         center[3],
                                SpiceDouble         smajor[3],
                                SpiceDouble         sminor[3]  );
                                
                                 
   void              erract_c ( ConstSpiceChar    * operation,
                                SpiceInt            lenout,
                                SpiceChar         * action    );


   void              errch_c  ( ConstSpiceChar    * marker,
                                ConstSpiceChar    * string );


   void              errdev_c ( ConstSpiceChar    * operation,
                                SpiceInt            lenout,
                                SpiceChar         * device    );


   void              errdp_c  ( ConstSpiceChar    * marker,
                                SpiceDouble         number  );


   void              errint_c ( ConstSpiceChar    * marker,
                                SpiceInt            number );


   void              errprt_c ( ConstSpiceChar    * operation,
                                SpiceInt            lenout,
                                SpiceChar         * list     );


   void              etcal_c  ( SpiceDouble         et,
                                SpiceInt            lenout,
                                SpiceChar         * string  );


   void              et2utc_c ( SpiceDouble         et ,
                                ConstSpiceChar    * format,
                                SpiceInt            prec,
                                SpiceInt            lenout,
                                SpiceChar         * utcstr );


   void              eul2m_c  ( SpiceDouble         angle3,
                                SpiceDouble         angle2,
                                SpiceDouble         angle1,
                                SpiceInt            axis3,
                                SpiceInt            axis2,
                                SpiceInt            axis1,
                                SpiceDouble         r [3][3] );


   void              eul2xf_c ( ConstSpiceDouble    eulang[6],
                                SpiceInt            axisa,
                                SpiceInt            axisb,
                                SpiceInt            axisc,
                                SpiceDouble         xform [6][6] );
                                
                   
   SpiceBoolean      exists_c ( ConstSpiceChar    * name );


   void              expool_c ( ConstSpiceChar    * name,
                                SpiceBoolean      * found );


   SpiceBoolean      failed_c ( void );


   void              frame_c  ( SpiceDouble         x[3],
                                SpiceDouble         y[3],
                                SpiceDouble         z[3] );


   void              frinfo_c ( SpiceInt            frcode,
                                SpiceInt          * cent,
                                SpiceInt          * clss,
                                SpiceInt          * clssid,
                                SpiceBoolean      * found );
                  
                   
   void              frmnam_c ( SpiceInt            frcode,
                                SpiceInt            lenout,
                                SpiceChar         * frname  );
                                

   void              ftncls_c ( SpiceInt            unit );
   

   void              furnsh_c ( ConstSpiceChar    * file );


   void              gcpool_c ( ConstSpiceChar    * name,
                                SpiceInt            start,
                                SpiceInt            room,
                                SpiceInt            lenout,
                                SpiceInt          * n,
                                void              * cvals,
                                SpiceBoolean      * found );


   void              gdpool_c ( ConstSpiceChar    * name,
                                SpiceInt            start,
                                SpiceInt            room,
                                SpiceInt          * n,
                                SpiceDouble       * values,
                                SpiceBoolean      * found );


   void              georec_c ( SpiceDouble         lon,
                                SpiceDouble         lat,
                                SpiceDouble         alt,
                                SpiceDouble         re,
                                SpiceDouble         f,
                                SpiceDouble         rectan[3] );


   void              getcml_c ( SpiceInt          * argc,
                                SpiceChar       *** argv );


   SpiceInt          getcml_  ( SpiceChar         * outline,
                                SpiceInt            len     );


   void              getelm_c ( SpiceInt            frstyr,
                                SpiceInt            lineln,
                                const void        * lines,
                                SpiceDouble       * epoch,
                                SpiceDouble       * elems   );
                                
                                
   void              getfat_c ( ConstSpiceChar    * file,
                                SpiceInt            arclen,
                                SpiceInt            typlen,
                                SpiceChar         * arch,
                                SpiceChar         * type   ); 


   void              getfov_c ( SpiceInt            instid,
                                SpiceInt            room,
                                SpiceInt            shapelen,
                                SpiceInt            framelen,
                                SpiceChar         * shape,
                                SpiceChar         * frame,
                                SpiceDouble         bsight [3],
                                SpiceInt          * n,
                                SpiceDouble         bounds [][3] ); 


   void              getmsg_c ( ConstSpiceChar    * option,
                                SpiceInt            lenout,
                                SpiceChar         * msg     );
                                
                                 
   void              gipool_c ( ConstSpiceChar    * name,
                                SpiceInt            start,
                                SpiceInt            room,
                                SpiceInt          * n,
                                SpiceInt          * ivals,
                                SpiceBoolean      * found );


   void              gnpool_c ( ConstSpiceChar    * name,
                                SpiceInt            start,
                                SpiceInt            room,
                                SpiceInt            lenout,
                                SpiceInt          * n,
                                void              * kvars,
                                SpiceBoolean      * found  );
                                
                                 
   SpiceDouble       halfpi_c ( void );

   
   void              ident_c  ( SpiceDouble         matrix[3][3] );
   
   
   void              illum_c  ( ConstSpiceChar    * target,
                                SpiceDouble         et,
                                ConstSpiceChar    * abcorr, 
                                ConstSpiceChar    * obsrvr, 
                                ConstSpiceDouble    spoint [3],
                                SpiceDouble       * phase,
                                SpiceDouble       * solar,
                                SpiceDouble       * emissn     );
                                
                                
   void              inedpl_c ( SpiceDouble         a,
                                SpiceDouble         b,
                                SpiceDouble         c,
                                ConstSpicePlane   * plane,
                                SpiceEllipse      * ellipse,
                                SpiceBoolean      * found    );
       
                         
   void              inelpl_c ( ConstSpiceEllipse * ellips,
                                ConstSpicePlane   * plane,
                                SpiceInt          * nxpts,
                                SpiceDouble         xpt1[3],
                                SpiceDouble         xpt2[3] );
                                
                                
   void              inrypl_c ( ConstSpiceDouble    vertex [3],
                                ConstSpiceDouble    dir    [3],
                                ConstSpicePlane   * plane,
                                SpiceInt          * nxpts,
                                SpiceDouble         xpt    [3] );
                                

   SpiceInt          intmax_c ( void );


   SpiceInt          intmax_  ( void );


   SpiceInt          intmin_c ( void );


   SpiceInt          intmin_  ( void );


   void              invert_c ( ConstSpiceDouble    m1[3][3],
                                SpiceDouble         m2[3][3]  );
                                

   void              invort_c ( ConstSpiceDouble    m  [3][3],
                                SpiceDouble         mit[3][3] );

                                
   SpiceBoolean      isrot_c  ( ConstSpiceDouble    m   [3][3],
                                SpiceDouble         ntol,
                                SpiceDouble         dtol       );
                                
                                
   SpiceBoolean      iswhsp_c ( ConstSpiceChar    * string );


   SpiceDouble       j1900_c  ( void );


   SpiceDouble       j1950_c  ( void );


   SpiceDouble       j2000_c  ( void );


   SpiceDouble       j2100_c  ( void );


   SpiceDouble       jyear_c  ( void );


   void              kdata_c  ( SpiceInt          which,
                                ConstSpiceChar  * kind,
                                SpiceInt          fillen,
                                SpiceInt          typlen,
                                SpiceInt          srclen,
                                SpiceChar       * file,
                                SpiceChar       * filtyp,
                                SpiceChar       * source,
                                SpiceInt        * handle,
                                SpiceBoolean    * found   );


   void              kinfo_c  ( ConstSpiceChar  * file,
                                SpiceInt          typlen,
                                SpiceInt          srclen,
                                SpiceChar       * filtyp,
                                SpiceChar       * source,
                                SpiceInt        * handle,
                                SpiceBoolean    * found  );
                  
                  
   void              ktotal_c ( ConstSpiceChar   * kind,
                                SpiceInt         * count ); 


   void              kxtrct_c ( ConstSpiceChar       * keywd,
                                SpiceInt               termlen,
                                const void           * terms,
                                SpiceInt               nterms,
                                SpiceInt               stringlen,
                                SpiceInt               substrlen,
                                SpiceChar            * string,
                                SpiceBoolean         * found,
                                SpiceChar            * substr  );


   SpiceInt          lastnb_c ( ConstSpiceChar    * string );


   void              latcyl_c ( SpiceDouble         radius,
                                SpiceDouble         lon,
                                SpiceDouble         lat,
                                SpiceDouble       * r,
                                SpiceDouble       * lonc,
                                SpiceDouble       * z      );


   void              latrec_c ( SpiceDouble         radius,
                                SpiceDouble         longitude,
                                SpiceDouble         latitude,
                                SpiceDouble         rectan [3] );


   void              latsph_c ( SpiceDouble         radius,
                                SpiceDouble         lon,
                                SpiceDouble         lat,
                                SpiceDouble       * rho,
                                SpiceDouble       * colat,
                                SpiceDouble       * lons   );


   void              lcase_c  ( SpiceChar         * in,
                                SpiceInt            lenout,
                                SpiceChar         * out     );


   void              ldpool_c ( ConstSpiceChar    * filename );


   void              lmpool_c ( const void        * cvals,
                                SpiceInt            lenvals,
                                SpiceInt             n       );
                                
                                
   void              lparse_c ( ConstSpiceChar    * list,
                                ConstSpiceChar    * delim,
                                SpiceInt            nmax,
                                SpiceInt            lenout,
                                SpiceInt          * n,
                                void              * items   );
               
                                
   void              lparsm_c ( ConstSpiceChar    * list,
                                ConstSpiceChar    * delims,
                                SpiceInt            nmax,
                                SpiceInt            lenout,
                                SpiceInt          * n,
                                void              * items   );
               
                                
   void              ltime_c  ( SpiceDouble         etobs,
                                SpiceInt            obs,
                                ConstSpiceChar    * dir,
                                SpiceInt            targ,
                                SpiceDouble       * ettarg,
                                SpiceDouble       * elapsd  );


   void              m2eul_c  ( ConstSpiceDouble    r[3][3],
                                SpiceInt            axis3,
                                SpiceInt            axis2,
                                SpiceInt            axis1,
                                SpiceDouble       * angle3,
                                SpiceDouble       * angle2,
                                SpiceDouble       * angle1  );


   void              m2q_c    ( ConstSpiceDouble    r[3][3],
                                SpiceDouble         q[4]     );



   SpiceBoolean      matchi_c ( ConstSpiceChar    * string,
                                ConstSpiceChar    * templ,
                                SpiceChar           wstr,
                                SpiceChar           wchr   );


   SpiceBoolean      matchw_c ( ConstSpiceChar    * string,
                                ConstSpiceChar    * templ,
                                SpiceChar           wstr,
                                SpiceChar           wchr   );


   SpiceDouble       maxd_c  ( SpiceInt             n, 
                                                    ... );


   SpiceInt          maxi_c  ( SpiceInt             n, 
                                                    ... );


   void              mequ_c   ( ConstSpiceDouble    m1  [3][3],
                                SpiceDouble         mout[3][3] );


   void              mequg_c  ( const void        * m1,
                                SpiceInt            nr,
                                SpiceInt            nc,
                                void              * mout );


   SpiceDouble       mind_c   ( SpiceInt            n, 
                                                    ... );


   SpiceInt          mini_c   ( SpiceInt            n, 
                                                    ... );


   int               moved_   ( SpiceDouble       * arrfrm,
                                SpiceInt          * ndim,
                                SpiceDouble       * arrto  );
                                 

   void              mtxm_c   ( ConstSpiceDouble    m1  [3][3],
                                ConstSpiceDouble    m2  [3][3],
                                SpiceDouble         mout[3][3] );


   void              mtxmg_c  ( const void        * m1,
                                const void        * m2,
                                SpiceInt            row1,
                                SpiceInt            col1,
                                SpiceInt            col2,
                                void              * mout  );


   void              mtxv_c   ( ConstSpiceDouble    m1  [3][3],
                                ConstSpiceDouble    vin [3],
                                SpiceDouble         vout[3]    );


   void              mtxvg_c  ( const void        * m1,
                                const void        * v2,
                                SpiceInt            ncol1,
                                SpiceInt            nr1r2,
                                void              * vout   );


   void              mxm_c    ( ConstSpiceDouble    m1  [3][3],
                                ConstSpiceDouble    m2  [3][3],
                                SpiceDouble         mout[3][3] );


   void              mxmg_c   ( const void        * m1,
                                const void        * m2,
                                SpiceInt            row1,
                                SpiceInt            col1,
                                SpiceInt            col2,
                                void              * mout  );


   void              mxmt_c   ( ConstSpiceDouble    m1  [3][3],
                                ConstSpiceDouble    m2  [3][3],
                                SpiceDouble         mout[3][3] );


   void              mxmtg_c  ( const void        * m1,
                                const void        * m2,
                                SpiceInt            nrow1,
                                SpiceInt            nc1c2,
                                SpiceInt            nrow2,
                                void              * mout  );


   void              mxv_c    ( ConstSpiceDouble    m1[3][3],
                                ConstSpiceDouble    vin[3],
                                SpiceDouble         vout[3] );


   void              mxvg_c   ( const void        * m1,
                                const void        * v2,
                                SpiceInt            nrow1,
                                SpiceInt            nc1r2,
                                void              * vout   );


   void              namfrm_c ( ConstSpiceChar    * frname,
                                SpiceInt          * frcode );


   void              nearpt_c ( ConstSpiceDouble    positn[3],
                                SpiceDouble         a,
                                SpiceDouble         b,
                                SpiceDouble         c,
                                SpiceDouble         npoint[3],
                                SpiceDouble       * alt        );


   void              npedln_c ( SpiceDouble         a,
                                SpiceDouble         b,
                                SpiceDouble         c,
                                ConstSpiceDouble    linept[3],
                                ConstSpiceDouble    linedr[3],
                                SpiceDouble         pnear[3],
                                SpiceDouble       * dist      );


   void              npelpt_c ( ConstSpiceDouble    point[3],
                                ConstSpiceEllipse * ellips,
                                SpiceDouble         pnear[3],
                                SpiceDouble       * dist      );
                                
                                 
   void              nplnpt_c ( ConstSpiceDouble    linpt  [3],
                                ConstSpiceDouble    lindir [3],
                                ConstSpiceDouble    point  [3],
                                SpiceDouble         pnear  [3],
                                SpiceDouble       * dist       );
                                

   void              nvc2pl_c ( ConstSpiceDouble    normal[3],
                                SpiceDouble         constant,
                                SpicePlane        * plane     );


   void              nvp2pl_c ( ConstSpiceDouble    normal[3],
                                ConstSpiceDouble    point[3],
                                SpicePlane        * plane     );


   void              oscelt_c ( ConstSpiceDouble    state[6],
                                SpiceDouble         et      ,
                                SpiceDouble         mu      ,
                                SpiceDouble         elts[8]  );


   void              pcklof_c ( ConstSpiceChar    * fname,
                                SpiceInt          * handle );


   void              pckuof_c ( SpiceInt            handle );


   void              pcpool_c ( ConstSpiceChar   * name,
                                SpiceInt           n,
                                SpiceInt           lenvals,
                                const void       * cvals    );
                                
                                 
   void              pdpool_c ( ConstSpiceChar    * name,
                                SpiceInt            n,
                                ConstSpiceDouble  * dvals );
                                
                                 
   SpiceDouble       pi_c     ( void );

   
   void              pipool_c ( ConstSpiceChar    * name,
                                SpiceInt            n,
                                ConstSpiceInt     * ivals ); 
                                
         
   void              pjelpl_c ( ConstSpiceEllipse * elin,
                                ConstSpicePlane   * plane,
                                SpiceEllipse      * elout  );
                                
                                
   void              pl2nvc_c ( ConstSpicePlane   * plane,
                                SpiceDouble         normal[3],
                                SpiceDouble       * constant  );
                   
                   
   void              pl2nvp_c ( ConstSpicePlane   * plane,
                                SpiceDouble         normal[3],
                                SpiceDouble         point[3]  );
                   
                   
   void              pl2psv_c ( ConstSpicePlane   * plane,
                                SpiceDouble         point[3],
                                SpiceDouble         span1[3],
                                SpiceDouble         span2[3]  );
                                 
                   
   SpiceChar       * prompt_c ( ConstSpiceChar    * prmptStr,
                                SpiceInt            lenout,
                                SpiceChar         * buffer   );


   void              prop2b_c ( SpiceDouble         gm,
                                ConstSpiceDouble    pvinit[6],
                                SpiceDouble         dt,
                                SpiceDouble         pvprop[6] );


   void              prsdp_c  ( ConstSpiceChar    * string,
                                SpiceDouble       * dpval  );


   void              prsint_c ( ConstSpiceChar    * string,
                                SpiceInt          * intval );


   void              psv2pl_c ( ConstSpiceDouble    point[3],
                                ConstSpiceDouble    span1[3],
                                ConstSpiceDouble    span2[3],
                                SpicePlane        * plane     );


   void              putcml_c ( SpiceInt            argc ,
                                SpiceChar        ** argv  );


   void              pxform_c ( ConstSpiceChar    * from,
                                ConstSpiceChar    * to,
                                SpiceDouble         et,
                                SpiceDouble         rotate[3][3] );
                                
 
   void              q2m_c    ( ConstSpiceDouble    q[4],
                                SpiceDouble         r[3][3] );


   void              radrec_c ( SpiceDouble         range,
                                SpiceDouble         ra,
                                SpiceDouble         dec,
                                SpiceDouble         rectan[3] );


   void              rav2xf_c ( ConstSpiceDouble     rot   [3][3],
                                ConstSpiceDouble     av    [3],
                                SpiceDouble          xform [6][6]  );
                                
                                 
   void              raxisa_c ( ConstSpiceDouble     matrix[3][3],
                                SpiceDouble          axis  [3],
                                SpiceDouble        * angle       );
                                
 
   void              rdtext_c ( ConstSpiceChar     * file,
                                SpiceInt             lenout,
                                SpiceChar          * line,
                                SpiceBoolean       * eof    );
                                
                                
   void              reccyl_c ( ConstSpiceDouble    rectan[3],
                                SpiceDouble       * r,
                                SpiceDouble       * lon,
                                SpiceDouble       * z         );


   void              recgeo_c ( ConstSpiceDouble    rectan[3],
                                SpiceDouble         re,
                                SpiceDouble         f,
                                SpiceDouble       * lon,
                                SpiceDouble       * lat,
                                SpiceDouble       * alt );


   void              reclat_c ( ConstSpiceDouble    rectan[3],
                                SpiceDouble       * radius,
                                SpiceDouble       * longitude,
                                SpiceDouble       * latitude  );


   void              recrad_c ( ConstSpiceDouble    rectan[3],
                                SpiceDouble       * radius,
                                SpiceDouble       * ra,
                                SpiceDouble       * dec  );


   void              reset_c  ( void );


   SpiceBoolean      return_c ( void );


   void              recsph_c ( ConstSpiceDouble    rectan[3],
                                SpiceDouble       * r,
                                SpiceDouble       * colat,
                                SpiceDouble       * lon );


   void              rotate_c ( SpiceDouble         angle,
                                SpiceInt            iaxis,
                                SpiceDouble         mout[3][3] );


   void              rotmat_c ( ConstSpiceDouble    m1[3][3],
                                SpiceDouble         angle,
                                SpiceInt            iaxis,
                                SpiceDouble         mout[3][3] );


   void              rotvec_c ( ConstSpiceDouble    v1[3],
                                SpiceDouble         angle,
                                SpiceInt            iaxis,
                                SpiceDouble         vout[3] );


   SpiceDouble       rpd_c    ( void );


   void              rquad_c  ( SpiceDouble         a,
                                SpiceDouble         b,
                                SpiceDouble         c,
                                SpiceDouble         root1[2],
                                SpiceDouble         root2[2] );
                                
                                 
   void              saelgv_c ( ConstSpiceDouble    vec1  [3],
                                ConstSpiceDouble    vec2  [3],
                                SpiceDouble         smajor[3],
                                SpiceDouble         sminor[3]  );
                                
                                 
   void              scdecd_c ( SpiceInt            sc,
                                SpiceDouble         sclkdp,
                                SpiceInt            sclklen,
                                SpiceChar         * sclkch   );


   void              sce2s_c  ( SpiceInt            sc,
                                SpiceDouble         et,
                                SpiceInt            sclklen,
                                SpiceChar         * sclkch   );


   void              sce2c_c  ( SpiceInt            sc,
                                SpiceDouble         et,
                                SpiceDouble       * sclkdp   );


   void              sce2t_c  ( SpiceInt            sc,
                                SpiceDouble         et,
                                SpiceDouble       * sclkdp   );


   void              scencd_c ( SpiceInt            sc,
                                ConstSpiceChar    * sclkch,
                                SpiceDouble       * sclkdp   );


   void              scfmt_c  ( SpiceInt            sc,
                                SpiceDouble         ticks,
                                SpiceInt            clkstrlen,
                                SpiceChar         * clkstr    );


   void              scpart_c ( SpiceInt            sc,
                                SpiceInt          * nparts,
                                SpiceDouble       * pstart,
                                SpiceDouble       * pstop   );


   void              scs2e_c  ( SpiceInt            sc,
                                ConstSpiceChar    * sclkch,
                                SpiceDouble       * et      );


   void              sct2e_c  ( SpiceInt            sc,
                                SpiceDouble         sclkdp,
                                SpiceDouble       * et      );


   void              sctiks_c ( SpiceInt            sc,
                                ConstSpiceChar    * clkstr,
                                SpiceDouble       * ticks   );


   void              setmsg_c ( ConstSpiceChar    * msg );


   void              sigerr_c ( ConstSpiceChar    * message );


   SpiceDouble       spd_c    ( void );


   void              sphcyl_c ( SpiceDouble         radius,
                                SpiceDouble         colat,
                                SpiceDouble         slon,
                                SpiceDouble       * r,
                                SpiceDouble       * lon,
                                SpiceDouble       * z );


   void              sphlat_c ( SpiceDouble         r,
                                SpiceDouble         colat,
                                SpiceDouble         lons,
                                SpiceDouble       * radius,
                                SpiceDouble       * lon,
                                SpiceDouble       * lat   );


   void              sphrec_c ( SpiceDouble         r,
                                SpiceDouble         colat,
                                SpiceDouble         lon,
                                SpiceDouble         rectan[3] );


   void              spk14a_c ( SpiceInt            handle,
                                SpiceInt            ncsets,
                                ConstSpiceDouble    coeffs [],
                                ConstSpiceDouble    epochs []  );
                                
                                
   void              spk14b_c ( SpiceInt            handle,
                                ConstSpiceChar    * segid,
                                SpiceInt            body,
                                SpiceInt            center,
                                ConstSpiceChar    * frame,
                                SpiceDouble         first,
                                SpiceDouble         last,
                                SpiceInt            chbdeg  );


   void              spk14e_c ( SpiceInt            handle );


   void              spkapo_c ( SpiceInt            targ,
                                SpiceDouble         et,
                                ConstSpiceChar    * ref,
                                ConstSpiceDouble    sobs[6],
                                ConstSpiceChar    * abcorr,
                                SpiceDouble         ptarg[3],
                                SpiceDouble       * lt        );
                                
                                
   void              spkapp_c ( SpiceInt            targ,
                                SpiceDouble         et,
                                ConstSpiceChar    * ref,
                                ConstSpiceDouble    sobs   [6],
                                ConstSpiceChar    * abcorr,
                                SpiceDouble         starg  [6],
                                SpiceDouble       * lt         );
   
                                
   void              spkcls_c ( SpiceInt            handle );
   
                                
   void              spkez_c  ( SpiceInt            target,
                                SpiceDouble         epoch,
                                ConstSpiceChar    * frame,
                                ConstSpiceChar    * abcorr,
                                SpiceInt            observer,
                                SpiceDouble         state[6],
                                SpiceDouble       * lt       );


   void              spkezp_c ( SpiceInt            targ,
                                SpiceDouble         et,
                                ConstSpiceChar    * ref,
                                ConstSpiceChar    * abcorr,
                                SpiceInt            obs,
                                SpiceDouble         ptarg[3],
                                SpiceDouble       * lt        );
                                

   void              spkezr_c ( ConstSpiceChar    * target,
                                SpiceDouble         epoch,
                                ConstSpiceChar    * frame,
                                ConstSpiceChar    * abcorr,
                                ConstSpiceChar    * observer,
                                SpiceDouble         state[6],
                                SpiceDouble       * lt       );


   void              spkgeo_c ( SpiceInt            targ,
                                SpiceDouble         et,
                                ConstSpiceChar    * ref,
                                SpiceInt            obs,
                                SpiceDouble         state[6],
                                SpiceDouble       * lt       );


   void              spkgps_c ( SpiceInt            targ,
                                SpiceDouble         et,
                                ConstSpiceChar    * ref,
                                SpiceInt            obs,
                                SpiceDouble         pos[3],
                                SpiceDouble       * lt     );
                                
                                 
   void              spklef_c ( ConstSpiceChar    * filename,
                                SpiceInt          * handle   );


   void              spkopa_c ( ConstSpiceChar    * file,
                                SpiceInt          * handle );
                                
                                
   void              spkopn_c ( ConstSpiceChar    * name,
                                ConstSpiceChar    * ifname,
                                SpiceInt            ncomch,
                                SpiceInt          * handle  );
                                
                                
   void              spkpds_c ( SpiceInt            body,  
                                SpiceInt            center,
                                ConstSpiceChar    * frame,
                                SpiceInt            type,
                                SpiceDouble         first,
                                SpiceDouble         last,
                                SpiceDouble         descr[5] );
                                 
                                 
   void              spkpos_c ( ConstSpiceChar    * targ,
                                SpiceDouble         et,
                                ConstSpiceChar    * ref,
                                ConstSpiceChar    * abcorr,
                                ConstSpiceChar    * obs,
                                SpiceDouble         ptarg[3],
                                SpiceDouble       * lt        );
         
                                
   void              spkssb_c ( SpiceInt            targ,
                                SpiceDouble         et,
                                ConstSpiceChar    * ref,
                                SpiceDouble         starg[6] );
                                
                                 
   void              spksub_c ( SpiceInt            handle,
                                SpiceDouble         descr[5],
                                ConstSpiceChar    * ident,
                                SpiceDouble         begin,
                                SpiceDouble         end,
                                SpiceInt            newh    );
                                
                                 
   void              spkuds_c ( ConstSpiceDouble    descr [5],
                                SpiceInt          * body,
                                SpiceInt          * center,
                                SpiceInt          * frame,
                                SpiceInt          * type,
                                SpiceDouble       * first,
                                SpiceDouble       * last,
                                SpiceInt          * begin,
                                SpiceInt          * end     );
                                
                                
   void              spkuef_c ( SpiceInt             handle );


   void              spkw02_c ( SpiceInt             handle,
                                SpiceInt             body,
                                SpiceInt             center,
                                ConstSpiceChar     * frame,
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid,
                                SpiceDouble          intlen,
                                SpiceInt             n,
                                SpiceInt             polydg,
                                ConstSpiceDouble     cdata [],
                                SpiceDouble          btime     );


   void              spkw03_c ( SpiceInt             handle,
                                SpiceInt             body,
                                SpiceInt             center,
                                ConstSpiceChar     * frame,
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid,
                                SpiceDouble          intlen,
                                SpiceInt             n,
                                SpiceInt             polydg,
                                ConstSpiceDouble     cdata [],
                                SpiceDouble          btime     );
                                
                                
   void              spkw05_c ( SpiceInt             handle,
                                SpiceInt             body,
                                SpiceInt             center,
                                ConstSpiceChar     * frame,
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid,
                                SpiceDouble          gm,
                                SpiceInt             n,
                                ConstSpiceDouble     states [][6],
                                ConstSpiceDouble     epochs []     );
                                
                                
   void              spkw08_c ( SpiceInt             handle,
                                SpiceInt             body,
                                SpiceInt             center,
                                ConstSpiceChar     * frame,
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid,
                                SpiceInt             degree,
                                SpiceInt             n,
                                ConstSpiceDouble     states[][6],
                                SpiceDouble          epoch1,
                                SpiceDouble          step         );
                                
                                
   void              spkw09_c ( SpiceInt             handle,
                                SpiceInt             body,
                                SpiceInt             center, 
                                ConstSpiceChar     * frame,
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid,
                                SpiceInt             degree,
                                SpiceInt             n,
                                ConstSpiceDouble     states[][6],
                                ConstSpiceDouble     epochs[]     );
                                
                                
   void              spkw10_c ( SpiceInt             handle,
                                SpiceInt             body, 
                                SpiceInt             center, 
                                ConstSpiceChar     * frame, 
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid, 
                                ConstSpiceDouble     consts [8],
                                SpiceInt             n,
                                ConstSpiceDouble     elems  [],
                                ConstSpiceDouble     epochs []  );
      
                                
   void              spkw12_c ( SpiceInt             handle,
                                SpiceInt             body,
                                SpiceInt             center, 
                                ConstSpiceChar     * frame,
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid,
                                SpiceInt             degree,
                                SpiceInt             n,
                                ConstSpiceDouble     states[][6],
                                SpiceDouble          epoch0,
                                SpiceDouble          step        );
                                

   void              spkw13_c ( SpiceInt             handle,
                                SpiceInt             body,
                                SpiceInt             center, 
                                ConstSpiceChar     * frame,
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid,
                                SpiceInt             degree,
                                SpiceInt             n,
                                ConstSpiceDouble     states[][6],
                                ConstSpiceDouble     epochs[]     );
                                

   void              spkw15_c ( SpiceInt             handle,
                                SpiceInt             body, 
                                SpiceInt             center, 
                                ConstSpiceChar     * frame, 
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid, 
                                SpiceDouble          epoch,
                                ConstSpiceDouble     tp     [3],
                                ConstSpiceDouble     pa     [3],
                                SpiceDouble          p,
                                SpiceDouble          ecc,
                                SpiceDouble          j2flg,
                                ConstSpiceDouble     pv     [3],
                                SpiceDouble          gm,
                                SpiceDouble          j2,
                                SpiceDouble          radius     );
                                

   void              spkw17_c ( SpiceInt             handle,
                                SpiceInt             body, 
                                SpiceInt             center, 
                                ConstSpiceChar     * frame, 
                                SpiceDouble          first,
                                SpiceDouble          last,
                                ConstSpiceChar     * segid, 
                                SpiceDouble          epoch,
                                ConstSpiceDouble     eqel   [9],
                                SpiceDouble          rapol,
                                SpiceDouble          decpol      );

                                             
   void              stelab_c ( ConstSpiceDouble    pobj[3],
                                ConstSpiceDouble    vobs[3],
                                SpiceDouble         appobj[3] );


   void              stpool_c ( ConstSpiceChar    * item,
                                SpiceInt            nth,
                                ConstSpiceChar    * contin,
                                SpiceInt            lenout,
                                SpiceChar         * string,
                                SpiceInt          * size,
                                SpiceBoolean      * found  );
                                
                                 
   void              str2et_c ( ConstSpiceChar    * date,
                                SpiceDouble       * et   );


   void              subpt_c  ( ConstSpiceChar    * method,
                                ConstSpiceChar    * target,
                                SpiceDouble         et,
                                ConstSpiceChar    * abcorr,
                                ConstSpiceChar    * obsrvr,
                                SpiceDouble         spoint [3],
                                SpiceDouble       * alt         );
                                
                                
   void              subsol_c ( ConstSpiceChar    * method,
                                ConstSpiceChar    * target,
                                SpiceDouble         et,
                                ConstSpiceChar    * abcorr,
                                ConstSpiceChar    * obsrvr,
                                SpiceDouble         spoint[3] );
                                

   SpiceDouble       sumad_c  ( ConstSpiceDouble    array[],
                                SpiceInt            n        );


   SpiceInt          sumai_c  ( ConstSpiceInt       array[],
                                SpiceInt            n        );


   void              surfnm_c ( SpiceDouble         a,
                                SpiceDouble         b,
                                SpiceDouble         c,
                                ConstSpiceDouble    point[3],
                                SpiceDouble         normal[3] );


   void              surfpt_c ( ConstSpiceDouble    positn[3],
                                ConstSpiceDouble    u[3],
                                SpiceDouble         a,
                                SpiceDouble         b,
                                SpiceDouble         c,
                                SpiceDouble         point[3],
                                SpiceBoolean      * found     );


   void              swpool_c ( ConstSpiceChar    * agent,
                                SpiceInt            nnames,
                                SpiceInt            lenvals,
                                const void        * names   );
                                
                                 
   void              sxform_c ( ConstSpiceChar    * from,
                                ConstSpiceChar    * to,
                                SpiceDouble         et,
                                SpiceDouble         xform[6][6] );


   void              szpool_c ( ConstSpiceChar    * name,
                                SpiceInt          * n,
                                SpiceBoolean      * found );
                                
                                 
   void              timdef_c ( ConstSpiceChar    * action,
                                ConstSpiceChar    * item,
                                SpiceInt            lenout,
                                SpiceChar         * value );


   void              timout_c ( SpiceDouble         et,
                                ConstSpiceChar    * pictur,
                                SpiceInt            lenout,
                                SpiceChar         * output );


   void              tipbod_c ( ConstSpiceChar    * ref,
                                SpiceInt            body,
                                SpiceDouble         et,
                                SpiceDouble         tipm[3][3] );


   void              tisbod_c ( ConstSpiceChar    * ref,
                                SpiceInt            body,
                                SpiceDouble         et,
                                SpiceDouble         tsipm[6][6] );


   ConstSpiceChar  * tkvrsn_c ( ConstSpiceChar    * item );


   void              tparse_c ( ConstSpiceChar    * string,
                                SpiceInt            lenout,
                                SpiceDouble       * sp2000,
                                SpiceChar         * errmsg  ); 
                   
                   
   void              tpictr_c ( ConstSpiceChar    * sample,
                                SpiceInt            lenpictur,
                                SpiceInt            lenerror,
                                SpiceChar         * pictur,
                                SpiceBoolean      * ok,
                                SpiceChar         * error     );


   SpiceDouble       trace_c  ( ConstSpiceDouble    matrix[3][3] );


   void              trcoff_c ( void );


   void              tsetyr_c ( SpiceInt            year );


   SpiceDouble       twopi_c  ( void );


   void              twovec_c ( ConstSpiceDouble    axdef  [3],
                                SpiceInt            indexa,
                                ConstSpiceDouble    plndef [3],
                                SpiceInt            indexp,
                                SpiceDouble         mout   [3][3] );


   SpiceDouble       tyear_c  ( void );


   void              ucase_c  ( SpiceChar         * in,
                                SpiceInt            lenout,
                                SpiceChar         * out    );


   void              ucrss_c  ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3],
                                SpiceDouble         vout[3] );


   SpiceDouble       unitim_c ( SpiceDouble         epoch,
                                ConstSpiceChar    * insys,
                                ConstSpiceChar    * outsys );


   void              unload_c ( ConstSpiceChar   * file ); 


   void              unorm_c  ( ConstSpiceDouble    v1[3],
                                SpiceDouble         vout[3],
                                SpiceDouble       * vmag    );


   void              unormg_c ( ConstSpiceDouble  * v1,
                                SpiceInt            ndim,
                                SpiceDouble       * vout,
                                SpiceDouble       * vmag );


   void              utc2et_c ( ConstSpiceChar    * utcstr,
                                SpiceDouble       * et   );


   void              vadd_c   ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3],
                                SpiceDouble         vout[3] ) ;


   void              vaddg_c  ( ConstSpiceDouble  * v1,
                                ConstSpiceDouble  * v2,
                                SpiceInt            ndim,
                                SpiceDouble       * vout );


   void              vcrss_c  ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3],
                                SpiceDouble         vout[3] );


   SpiceDouble       vdist_c  ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3] );


   SpiceDouble       vdistg_c ( ConstSpiceDouble  * v1,
                                ConstSpiceDouble  * v2,
                                SpiceInt            ndim );


   SpiceDouble       vdot_c   ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3] );

   SpiceDouble       vdotg_c  ( ConstSpiceDouble  * v1,
                                ConstSpiceDouble  * v2,
                                SpiceInt            ndim );

   void              vequ_c   ( ConstSpiceDouble    vin[3],
                                SpiceDouble         vout[3] );


   void              vequg_c  ( ConstSpiceDouble  * vin,
                                SpiceInt            ndim,
                                SpiceDouble       * vout );


   void              vhat_c   ( ConstSpiceDouble    v1  [3],
                                SpiceDouble         vout[3] );


   void              vhatg_c  ( ConstSpiceDouble    v1  [3],
                                SpiceInt            ndim,
                                SpiceDouble         vout[3] );


   void              vlcom_c  ( SpiceDouble         a,
                                ConstSpiceDouble    v1[3],
                                SpiceDouble         b,
                                ConstSpiceDouble    v2[3],
                                SpiceDouble         sum[3] );


   void              vlcom3_c ( SpiceDouble         a,
                                ConstSpiceDouble    v1[3],
                                SpiceDouble         b,
                                ConstSpiceDouble    v2[3],
                                SpiceDouble         c,
                                ConstSpiceDouble    v3[3],
                                SpiceDouble         sum[3] );


   void              vlcomg_c ( SpiceInt            n,
                                SpiceDouble         a,
                                ConstSpiceDouble  * v1,
                                SpiceDouble         b,
                                ConstSpiceDouble  * v2,
                                SpiceDouble       * sum );


   void              vminug_c ( ConstSpiceDouble  * vin,
                                SpiceInt            ndim,
                                SpiceDouble       * vout );
 

   void              vminus_c ( ConstSpiceDouble    v1[3],
                                SpiceDouble         vout[3] );


   SpiceDouble       vnorm_c  ( ConstSpiceDouble    v1[3] );


   SpiceDouble       vnormg_c ( ConstSpiceDouble  * v1,
                                SpiceInt            ndim );


   void              vpack_c  ( SpiceDouble         x,
                                SpiceDouble         y,
                                SpiceDouble         z,
                                SpiceDouble         v[3] );
                                
                                 
   void              vperp_c  ( ConstSpiceDouble    a[3],
                                ConstSpiceDouble    b[3],
                                SpiceDouble         p[3] );


   void              vprjp_c  ( ConstSpiceDouble    vin   [3],
                                ConstSpicePlane   * plane,
                                SpiceDouble         vout  [3] );


   void              vprjpi_c ( ConstSpiceDouble    vin    [3],
                                ConstSpicePlane   * projpl,
                                ConstSpicePlane   * invpl,
                                SpiceDouble         vout   [3],
                                SpiceBoolean      * found       );
                                
                                
   void              vproj_c  ( ConstSpiceDouble    a[3],
                                ConstSpiceDouble    b[3],
                                SpiceDouble         p[3] );


   SpiceDouble       vrel_c   ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3]  );


   SpiceDouble       vrelg_c  ( ConstSpiceDouble  * v1, 
                                ConstSpiceDouble  * v2,
                                SpiceInt            ndim  );


   void              vrotv_c  ( ConstSpiceDouble    v[3],
                                ConstSpiceDouble    axis[3],
                                SpiceDouble         theta,
                                SpiceDouble         r[3] );


   void              vscl_c   ( SpiceDouble         s,
                                ConstSpiceDouble    v1[3],
                                SpiceDouble         vout[3] );


   void              vsclg_c  ( SpiceDouble         s,
                                ConstSpiceDouble  * v1,
                                SpiceInt            ndim,
                                SpiceDouble       * vout  );


   SpiceDouble       vsep_c   ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3] );


   void              vsub_c   ( ConstSpiceDouble    v1[3],
                                ConstSpiceDouble    v2[3],
                                SpiceDouble         vout[3] );


   void              vsubg_c  ( ConstSpiceDouble  * v1,
                                ConstSpiceDouble  * v2,
                                SpiceInt            ndim,
                                SpiceDouble       * vout  );


   SpiceDouble       vsepg_c  ( ConstSpiceDouble  * v1,
                                ConstSpiceDouble  * v2,
                                SpiceInt            ndim );


   SpiceDouble       vtmv_c   ( ConstSpiceDouble    v1     [3],
                                ConstSpiceDouble    matrix [3][3],
                                ConstSpiceDouble    v2     [3]    );


   SpiceDouble       vtmvg_c  ( const void        * v1,
                                const void        * matrix,
                                const void        * v2,
                                SpiceInt            nrow,
                                SpiceInt            ncol          );


   void              vupack_c ( ConstSpiceDouble    v[3],
                                SpiceDouble       * x,
                                SpiceDouble       * y,
                                SpiceDouble       * z     ); 

   SpiceBoolean      vzero_c  ( ConstSpiceDouble     v[3] );


   SpiceBoolean      vzerog_c ( ConstSpiceDouble  * v, 
                                SpiceInt            ndim );


 
   void              xf2eul_c ( ConstSpiceDouble     xform  [6][6],
                                SpiceInt             axisa,
                                SpiceInt             axisb,
                                SpiceInt             axisc,
                                SpiceDouble          eulang [6],
                                SpiceBoolean       * unique         );
     
                                
   void              xf2rav_c ( ConstSpiceDouble     xform [6][6],
                                SpiceDouble          rot   [3][3],
                                SpiceDouble          av    [3]     );
                                 
                                 
   void              xpose_c  ( ConstSpiceDouble    m1  [3][3],
                                SpiceDouble         mout[3][3] );


   void              xpose6_c ( ConstSpiceDouble    m1  [6][6],
                                SpiceDouble         mout[6][6] );


   void              xposeg_c ( const void        * matrix, 
                                SpiceInt            nrow, 
                                SpiceInt            ncol, 
                                void              * xposem );


   void              zzgetcml_c( SpiceInt          * argc,
                                 SpiceChar       *** argv,
                                 SpiceBoolean        init );


#endif 

