#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "tricklist.h"


#include "general.h"

/* la liste qu'on utilise est une variable definie ailleurs,
   donc on utilise le mot-clef 'extern' pour le compilo ne
   hurle pas. 
*/
extern list l; 

int check_if_launched( void )
{

  unsigned int p;
  pid_t pid;
  FILE * fp = NULL; 

  pid = getpid();

  if ( !(fp=fopen( LOCKFILE, "r" )) )
    {
      return EXIT_FAILURE;
    }

  
  if ( 1!=fscanf( fp, "%u", &p ) )
    {
      fprintf( stderr, "Someone's tryin' to fool me...\n" ); 
      fclose( fp );
      unlink( LOCKFILE ); 
      return EXIT_FAILURE;
    }

  fclose( fp );

  if ( p!=(unsigned int)pid )
    {
      fprintf( stderr, "Another cron daemon is already running, committing suicide.\n" );
      /*
	On utilise _exit() lorsqu'on ne veut pas executer les fonctions
	enregistrees avec atexit() lors de la sortie du programme.
	Dans ce cas, on ne veut pas effacer la liste des evenements et
	on ne veut pas non plus effacer le fichier de verrou. 
      */
      _exit( EXIT_FAILURE );
    }
  
  return EXIT_SUCCESS;
}

void create_lock_file( void )
{

  FILE * fp = NULL; 

  if ( !(fp=fopen( LOCKFILE, "w" )) )
    {
      perror( "create_lock_file" );
      exit( EXIT_FAILURE );
    }

  fprintf( fp, "%d\n", getpid() );

  fclose( fp ); 

  fprintf( stderr, "Lock file created\n" );
  
  return;
}

void delete_lock_file( void )
{

  if ( unlink( LOCKFILE ) )
    {
      perror( "delete_lock_file" );
      exit( EXIT_FAILURE );
    }

  fprintf( stderr, "Lock file deleted\n" );

  return; 
  
}

void delete_event_list( void )
{

  list_delete( l );

  fprintf( stderr, "Event list deleted\n"); 
  return; 

}

void finish( void )
{

  fprintf( stderr, "Finishing daemon\n" ); 

  return;
}

