
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <limits.h>

#include "mat.h"

mat mat_new( int height, int width )
{
  int i; 
  mat m;
  header h; 

  h.width = width;
  h.height = height;

  m = malloc( sizeof( header ) + height * sizeof( double* ) );
  
  ((header*)m)->width = width;
  ((header*)m)->height = height;
  
  m = (mat)( (header*)m+1 );
  
  for ( i= 0; i< height; i++ )
    {
      unsigned int j;
      
      m[i] = calloc( width,sizeof( double ) );

      for ( j= 0; j< width; j++ )
	m[i][j] = 0.;
    }

  return m;
}

void mat_delete( mat m )
{
  int i; 
  header *h = (header*)m-1;

  for ( i = 0; i< mat_height(m); i++ )
    free( m[i] );
  
  free( h );
}

int mat_height( mat m )
{
  header *h = (header*)m-1;

  return h->height;
}

int mat_width( mat m )
{
  header *h = (header*)m-1;

  return h->width;
}

mat mat_clone( mat m )
{
  unsigned int i, j; 
  mat clone = mat_new( mat_height(m), mat_width(m) );

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      clone[i][j] = m[i][j];

  return clone;
}

/* X = x + d with proper bound checking and mirroring */
#define mirror_add(X, x, d, min, max) do {      \
    (X) = (x) + (d);                            \
    if((X) < (min) || (X) >= (max))             \
      (X) = (x) - (d);                          \
} while(0)

mat mat_filter_fir (mat input, mat filter, int px, int py)
{
  int X, Y, x, y, dx, dy, wf, hf, w, h;
  mat  output;

  w = mat_width (input);
  h = mat_height (input);
  wf = mat_width (filter);
  hf = mat_height (filter);

  assert (px >= 0 && px < wf);
  assert (py >= 0 && py < hf);

  output = mat_new (h, w);

  for (x = 0; x < w; x++) {
    for (y = 0; y < h; y++) {
      /* filter around position x,y */
      for (dx = -px; dx < wf - px; dx++) {
        for (dy = -py; dy < hf - py; dy++) {
          /* compute the input sample location */
          mirror_add (X, x, dx, 0, w);
          mirror_add (Y, y, dy, 0, h);
          output[y][x] += filter[dy + py][dx + px] * input[Y][X];
        }
      }
    }
  }

  return (output);
}

double mat_min( mat m )
{
  unsigned int i, j;

  double min = INT_MAX;

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      if ( min > m[i][j] )
	min = m[i][j];

  return min; 
}


double mat_max( mat m )
{
  unsigned int i, j;

  double max = -INT_MAX;

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      if ( max < m[i][j] )
	max = m[i][j];

  return max; 
}


