#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h> 
#include <unistd.h>
#include <assert.h>

#include "general.h"
#include "matrix.h"
#include "pmul.h"

int main ( int argc, char ** argv ) 
{
  mat r1, r2; 
  mat m1, m2; 
  struct timeval t0,t1,t2;

  /* On cree des matrices au pif */
  m1 = mat_random( 5, DIM );
  m2 = mat_random( DIM, 4 );

  /* Paranoid coding... */
  assert( m1->c == m2->l );
	  
 /* On les affiche */
  mat_print( m1 );
  mat_print( m2 );

  /* On chronometre */
  gettimeofday( &t0, NULL );
  /* r1 contient le resultat avec le calcul parallele */
  r1 = mat_pmul( m1, m2 );
  gettimeofday( &t1, NULL );
  /* r2 contient le resultat avec le calcul sequentiel
     pour verifier la justesse du calcul parallele...  */
  r2 =  mat_mul( m1, m2 );
  gettimeofday( &t2, NULL );

  fprintf( stderr, "Algo parallel : %ld usec\nAlgo seq : %ld usec\n\n",(t1.tv_sec - t0.tv_sec)*1000000L + (t1.tv_usec - t0.tv_usec) ,(t2.tv_sec - t1.tv_sec)*1000000L  + (t2.tv_usec - t1.tv_usec) );
  
  /* on affiche les resultats pour comparer */
  mat_print( r2 );
  mat_print( r1 );

  /* Si le resultat est faux, on sort ici */
  assert( are_mat_equal( r2, r1 ) );
  
  fprintf( stderr, "Matrices are identical!\n" );

  /* On ne laisse pas de $%*!& en partant */
  mat_delete( m1 );
  mat_delete( m2 );
  mat_delete( r1 );
  mat_delete( r2 );
  
  exit( EXIT_SUCCESS );
}
