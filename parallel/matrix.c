#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "matrix.h"
#include "general.h"

/* Rien a signaler pour les matrices, 
   on s'en fout c'est juste pour avoir 
   de quoi jouer avec. 
*/

mat mat_zeros( unsigned int l, unsigned int c )
{
  mat m = NULL; 
  unsigned int i, j; 

  if ( !(m=malloc( sizeof( *m ) )) )
    return NULL; 

  if ( !(m->data=malloc( l*sizeof( *(m->data) ) )) )
    return NULL; 

  for ( i = 0; i < l; i++ ) 
    if ( !(m->data[i] = malloc( c*sizeof( **(m->data) ) )) )
    {
	  for ( j = 0; j < i; j++ ) free( m->data[j] ); 
	    free( m->data );
	  return( NULL );
    }

  m->l = l;   
  m->c = c; 

  for ( i = 0; i < l; i++ )
    for ( j = 0; j < c; j++ )
      m->data[i][j] = 0;     

  return( m ); 

}


mat mat_random( unsigned int l, unsigned int c ) 
{
  mat m = mat_zeros( l, c ); 
  unsigned int i, j; 

  srand( getpid() ); 

  for ( i = 0; i < l; i++ )
    for ( j = 0; j < c; j++ )
      m->data[i][j] =  1 + (int) (NUM_MAX * (rand() / (RAND_MAX + 1.0)));

  return m; 
}


int are_mat_equal( mat m1, mat m2 )
{
  int i;
  int j;
  if( m1->l != m2->l || m1->c != m2->c ) return 0;
  for ( i = 0; i< m1->l; i++ )
    for ( j = 0; j < m1->c; j++ )
      if (m1->data[i][j] != m2->data[i][j]) return 0;
  
  return 1;
}


void mat_delete( mat m )
{
  unsigned int i; 

  for ( i = 0; i < m->l; i++ ) 
    free( m->data[i] ); 

  free( m->data ); 

  free( m );

  return;

}


void mat_print( mat m ) 
{
  unsigned int i, j; 

  fprintf( stderr, "Matrix %p: %d rows, %d cols\n", (void *)m, m->l, m->c );

  for ( i = 0; i < m->l; i++ ) 
  {
    for ( j = 0; j < m->c; j++ ) 
	  fprintf( stderr, "%d ", m->data[i][j] );
    fprintf( stderr, "\n" );
  }

  fprintf( stderr, "\n" );

  return;
}

mat mat_mul( mat m1, mat m2 ) 
{
  unsigned int i, j, k; 
  mat r; 

  assert( m1->c == m2->l );

  r = mat_zeros( m1->l, m2->c );

  for ( i = 0; i < r->l; i++ ) 
    for ( j = 0; j < r->c; j++ ) 
      for ( k = 0; k < m1->c; k++ ) 
		r->data[i][j] += m1->data[i][k] * m2->data[k][j];

  return( r );
}

