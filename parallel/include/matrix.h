#ifndef _MATRIX_H_
#define _MATRIX_H_

/* Bon ben voila... une matrice quoi... */
typedef struct mat_t {
  unsigned int l, c; 
  int ** data; 
} * mat; 


/* Cree une matrice et la remplit de zeros */
mat mat_zeros( unsigned int l, unsigned int c );

/* Cree une matrice remplie de valeurs aleatoires */
mat mat_random( unsigned int l, unsigned int c ); 

/* Detruit une matrice */
void mat_delete( mat m ); 

/* Test l'égalité entre les deux matrices */
int are_mat_equal( mat m1, mat m2 );

/* Affiche le contenu de la matrice passee en parametre */
void mat_print( mat m ); 

/* Effectue la multiplication de deux matrices -- version non parallele, 
   pour verification du calcul parallele */
mat mat_mul( mat m1, mat m2 ); 

#endif
