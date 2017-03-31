#ifndef _PMAT_H_
#define _PMAT_H_

#include <pthread.h>

#include "matrix.h"

/* Structure contenant les arguments avec lesquels lancer une tache */
typedef struct thread_args_t {

  mat m1, m2;   /* (pointeurs vers les) matrices m1 et m2 */
  mat r;        /* (pointeur vers la) matrice resultat */
  unsigned int i, j, k; /* ou en est-on dans le calcul ? */
  pthread_mutex_t * mutex; /* synchro entre les threads*/

} * thread_arg; 

/* Effectue la multiplication de deux matrices en 
   parallele en lancant plusieurs threads.  */
mat mat_pmul( mat m1, mat m2 ); 

/* Section critique ou la synchronisation est necessaire. */
void * criticalSection( void * arg );

#endif
