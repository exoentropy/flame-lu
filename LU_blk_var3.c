
/* Copyright 2013 The University of Texas at Austin  
 
   For licensing information see
                  http://www.cs.utexas.edu/users/flame/license.html 

   Programmed by: Scott Enriquez
                  scottie@scott-enriquez.net
                                                                     */

#include "FLAME.h"

int LU_blk_var3( FLA_Obj A, int nb_alg )
{
  FLA_Obj ATL,   ATR,      A00, A01, A02, 
          ABL,   ABR,      A10, A11, A12,
                           A20, A21, A22;

  int b;

  FLA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, FLA_TL );

  while ( FLA_Obj_length( ATL ) < FLA_Obj_length( A ) ){

    b = min( FLA_Obj_length( ABR ), nb_alg );

    FLA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00, /**/ &A01, &A02,
                        /* ************* */   /* ******************** */
                                                &A10, /**/ &A11, &A12,
                           ABL, /**/ ABR,       &A20, /**/ &A21, &A22,
                           b, b, FLA_BR );

    /*------------------------------------------------------------*/

    // A01 := inv(L00) * A01
    FLA_Trsm( FLA_LEFT, FLA_LOWER_TRIANGULAR,
	      FLA_NO_TRANSPOSE, FLA_UNIT_DIAG,
	      FLA_ONE, A00, A01 );

    // A11 := LU(A11 - A10 * A01)
    FLA_Gemm(FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE,
                FLA_MINUS_ONE, A10, A01, FLA_ONE, A11);
    LU_unb_var3(A11);

    // A21 := (A21 - A20 * A01) * inv(U11)
    FLA_Gemm(FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE,
	      FLA_MINUS_ONE, A20, A01, FLA_ONE, A21);
    FLA_Trsm( FLA_RIGHT, FLA_UPPER_TRIANGULAR,
	      FLA_NO_TRANSPOSE, FLA_NONUNIT_DIAG,
	      FLA_ONE, A11, A21);

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00, A01, /**/ A02,
                                                     A10, A11, /**/ A12,
                            /* ************** */  /* ****************** */
                              &ABL, /**/ &ABR,       A20, A21, /**/ A22,
                              FLA_TL );

  }

  return FLA_SUCCESS;
}



