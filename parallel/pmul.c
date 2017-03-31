#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "pmul.h"
#include "matrix.h"
#include "tab.h"


/*
  Dans ce TD, on peut jouer avec trois primitives de
  synchronisation :
  - l'algorithme du boulanger de Leslie Lamport
  - l'utilisation d'un mutex Posix
  - le Compare And Swap (instruction atomique)

  Pour selectionner l'une ou l'autre des methodes de
  synchronisation, on utilisera un #define parmi : 
  #define LAMPORT  : algo du boulanger
  #define COMPARE  : compare and swap atomique
  #define PMUTEX   : mutex Posix

  On peut tester ce qu'il advient en l'absence de
  synchronisation : 
  #define NOSYNC   : aucune synchronisation

  NOTE : 
  Partout dans le code, les instructions relatives a
  l'implantation de l'une ou l'autre methode sont
  ecrites entre des directives pour le pre-processeur.
  Par exemple, tout ce qui a a voir avec l'algo du
  boulanger est ecrit entre :
  #ifdef LAMPORT
    ....
  #endif
  
*/

/* On commence SANS synchro, juste pour rire (jaune) */
#define NOSYNC 


/*
  La fonction My_compare implante le
  compare & swap atomique en assembleur,
  soit l'instruction cmpxchgl chez Intel.
  Voir le fichier cas.s pour les details,
  d'ou la declaration 'extern'.
*/
extern int My_compare( int*, int, int );


pthread_t * threads = NULL; /* tableau stockant les ID de chaque thread    */

/* Variables globales pour l'algo du boulanger */
#ifdef LAMPORT
unsigned int * choix;               /* tableau 'choix' dans l'algo du boulanger    */
unsigned int * numero;              /* tableau 'numero' dans l'algo du boulanger   */
#endif


/* Fonction qui effectue le calcul et qui contient la section critique     */
void * criticalSection( void * arg )
{
#ifdef LAMPORT
  unsigned int i; 
#endif
#ifdef COMPARE
  int old,new,mult;
  int* test;
#endif
#ifdef PMUTEX
  int mult;
#endif


  /*
    TODO : Convertir les arguments pour recuperer le bon type
   */
  thread_arg a; /*  = ... */

  /*****************************************************************/
  /*
    Premiere synchro : Algorithme du boulanger de LAMPORT
   */
#ifdef LAMPORT
  /*
    TODO : Implanter l'algorithme du boulanger de Leslie Lamport
   */
#endif
  
  /*****************************************************************/
  /*
    Deuxieme synchro : MUTEX POSIX
   */  
#ifdef PMUTEX
  /* TODO : Implanter un mutex Posix
     Lire le manuel au sujet de pthread_mutex_{lock,unlock}
   */
#endif

  /*****************************************************************/
  /*
    Troisieme synchro : COMPARE AND SWAP (ASM)
   */
#ifdef COMPARE
  test = &a->r->data[a->i][a->j];
  old = *test;
  mult = (a->m1->data[a->i][a->k]) * (a->m2->data[a->k][a->j]);
  new = mult + old;
  while( !My_compare( test, old, new ) )
  {
    old = *test;
    new = mult + old;
  }
#endif
  
  /*****************************************************************/
  /*
    Pas de synchro : resultat faux
   */
#ifdef NOSYNC
  a->r->data[a->i][a->j] += (a->m1->data[a->i][a->k]) * (a->m2->data[a->k][a->j]); 
#endif

  return NULL; 
}

mat mat_pmul( mat m1, mat m2 )
{
#ifdef PMUTEX
  /* Declaration et initialisation du mutex Posix */
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
  thread_arg targs = NULL; /* tableau des arguments des threads */
  mat r = NULL; 
  unsigned int i, j, k;
  int zorro = 0;

  /* Contract programming is not a crime */
  assert( m1->c == m2->l );

  /* On connait la taille de la matrice resultat, on alloue */
  r = mat_zeros( m1->l, m2->c );

  /* On alloue la place pour les tableaux */
  threads = malloc( m1->c*sizeof( *threads ) );
  targs   = malloc( m1->c*sizeof(  *targs  ) );

#ifdef LAMPORT
  choix   = malloc( m1->c*sizeof(  *choix  ) );
  numero  = malloc( m1->c*sizeof( *numero  ) );
#endif
  
  /* On initialise ce qui ne changera pas d'une execution sur l'autre */
  for ( k = 0; k < m1->c; k++ ) 
  {
    targs[k].m1 = m1; 
    targs[k].m2 = m2; 
    targs[k].r  =  r; 
#ifdef PMUTEX
    targs[k].mutex = &mutex;
#endif
  }

  /* on double-boucle sur la matrice entiere */
  for ( i = 0; i < r->l; i++ )
    for ( j = 0; j < r->c; j++ )
    {
      /* pour chaque appel de thread a l'iteration (i,j)... */
      for ( k= 0; k< m1->c; k++ )
      {
#ifdef LAMPORT
        choix[k] = 0; 
        numero[k] = 0;
#endif
        threads[k] = 0; 
        targs[k].i = i; 
        targs[k].j = j; 
      }

      zorro = 0;
	
      /* on lance les threads */
      for ( k = 0; k < m1->c; k++ )
      {
        targs[k].k = k; /* il ne manquait plus que de savoir quel calcul faire */

        /* Ahhh le bon vieux pthread_create...
           TODO : Completer l'interieur du if(...) avec pthread_create
         */
        if ( 1  /* remplacer le 1 par pthread_create */ ) 
        {
          /* Une cause d'erreur probable de pthread_create() est l'utilisation
             simultannée de trop nombreux threads. On va donc attendre qu'au
             moins un thread ait fini, parmi les premiers lancés.

            TODO : utiliser pthread_join pour attendre la fin du thread[zorro]. 
          */
          zorro++;
          k--;
        }
      }

	  /* Avant de passer au coefficient suivant, on verifie 
	     que tous les threads ont termine...
	     Ca s'appelle un rendez-vous. 
      
	     TODO : Utiliser pthread_join !
	     Solution standard, pratique et fiable
	  */
	  for ( k = zorro; k < m1->c; k++ )
	  {
	    /* pthread_join */
	  }
    }

  /* on fait le menage... */
  free( targs );
  free( threads );

#ifdef LAMPORT
  free( numero );
  free( choix );
#endif
  
  return r; 
}
