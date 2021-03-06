
/* Copyright 2013 The University of Texas at Austin  
 
   For licensing information see
                  http://www.cs.utexas.edu/users/flame/license.html 

   Programmed by: Scott Enriquez
                  scottie@scott-enriquez.net
                                                                     */

#include "FLAME.h"

int LU_unb_var4( FLA_Obj A )
{
  FLA_Obj ATL,   ATR,      A00,  a01,     A02, 
          ABL,   ABR,      a10t, alpha11, a12t,
                           A20,  a21,     A22;

  FLA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, FLA_TL );

  while ( FLA_Obj_length( ATL ) < FLA_Obj_length( A ) ){

    FLA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00,  /**/ &a01,     &A02,
                        /* ************* */   /* ************************** */
                                                &a10t, /**/ &alpha11, &a12t,
                           ABL, /**/ ABR,       &A20,  /**/ &a21,     &A22,
                           1, 1, FLA_BR );

    /*------------------------------------------------------------*/

    // alpha11 := alpha11 - a10t * a01
    FLA_Dots( FLA_MINUS_ONE, a10t, a01, FLA_ONE, alpha11 );

    // a21 := (a21 - A20 * a01) / alpha11
    FLA_Gemv( FLA_NO_TRANSPOSE, FLA_MINUS_ONE, A20, a01, FLA_ONE, a21 );

    FLA_Inv_scal( alpha11, a21 );

    // a12t := a12t - a10t * A02
    FLA_Gemv( FLA_TRANSPOSE, FLA_MINUS_ONE, A02, a10t, FLA_ONE, a12t );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00,  a01,     /**/ A02,
                                                     a10t, alpha11, /**/ a12t,
                            /* ************** */  /* ************************ */
                              &ABL, /**/ &ABR,       A20,  a21,     /**/ A22,
                              FLA_TL );

  }

  return FLA_SUCCESS;
}
