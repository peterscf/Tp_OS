
#ifndef _MAT_H_
#define _MAT_H_

typedef struct header_t
{
  int height;
  int width;
} header;

typedef double ** mat;

mat mat_new( int height, int width );
void mat_delete( mat m );
mat mat_clone( mat m );
int mat_height( mat m );
int mat_width( mat m );
mat mat_pgm_read (const char *filename);
int mat_pgm_write (const char *filename, mat m);
mat mat_filter_fir (mat input, mat filter, int px, int py);
double mat_max( mat m );
double mat_min( mat m );

#endif

