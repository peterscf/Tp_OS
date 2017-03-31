/*##################################################################
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *#################################################################
*/

#include <stdio.h>
#include <string.h>

#include "./api/mat.h"

mat invert( mat m, char *args )
{

  double min;
  double max;

  double dynamic = 0.;
  
  unsigned int i, j;

  if ( m == NULL )
    {
      fprintf( stderr, "No image loaded!\n" );
      return m;
    }

  min = mat_min( m ); 
  max = mat_max( m );

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      m[i][j] = (255-m[i][j]);
  
  return m;
}


void invert_helper( void )
{

  fprintf( stderr, "\nThis is help for plugin invert\n" );

  fprintf( stderr, "\nThis plugin will invert the pixels\n" );
 
  return;
}
