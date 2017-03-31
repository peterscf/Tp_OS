#include <stdlib.h>
#include <stdio.h>
#include "alloc.c"
#define SZ 5

#define OVERFLOW 1

int main ( int argc, char ** argv )
{
  unsigned int i;

  double *p = my_malloc( SZ * sizeof( *p ) );

  for ( i= 0; i< SZ; i++ )
    p[i] = 0.;

  for ( i= 0; i< SZ+OVERFLOW; i++ )
    fprintf( stderr, "p[%d] = %f @ %p\n", i, p[i], (void*)&p[i] );
  
  my_free( p );
  
  exit( EXIT_SUCCESS );
}
