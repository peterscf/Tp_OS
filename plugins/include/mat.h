
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
int mat_height( mat m );
int mat_width( mat m );
mat mat_pgm_read (const char *filename);
int mat_pgm_write (const char *filename, mat m);

#endif

