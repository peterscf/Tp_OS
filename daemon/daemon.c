#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "tricklist.h"

#include "general.h"
#include "event.h"
#include "utils.h"
#include "daemon.h"

extern list l; 

void sigint_handler( int signum )
{

  int empty;
  
  /* Une fois le signal intercepte, le gestionnaire
     associe est decharge. Il faut donc le rearmer au
     besoin. Ici, il le faut. 
   */
  signal( SIGINT, sigint_handler );

  fprintf( stderr, "Reloading cronfile\n" );

  list_delete( l ); 

  parse_file( CRONDFLT );

  list_check_if_empty( l, empty );
  
  while ( l->content && (int)(((event*)l->content)->t) - time( NULL ) < 0 && !empty )
    {
      list_del_first( l );
      list_check_if_empty( l, empty );
    }
  
  list_check_if_empty( l, empty );
  
  if ( !empty )
    {
      list_apply_function(l,printevent); 

      alarm( (int)(((event*)l->content)->t) - time( NULL ) );
  
      fprintf( stderr, "Next event in %d sec\n",  (int)((((event*)l->content)->t) - time( NULL )) ); 
    }
  else
    {
      fprintf( stderr, "Nothing to do yet!\n" );
      pause();
    }

  return; 
  
}

void sigquit_handler( int signum )
{
	exit(1);	
}

void sigalarm_handler( int signum )
{
	signal(SIGALRM,sigalarm_handler);
	/*verifier si liste event vide 
	si vide alors exe pause
	sinon retire l'event de la liste (list_pop()) puis exe system() puis position nouvelle alarme sur next event si EOJ alors fin:
 	*/
}

int code_demon( char * cronfile )
{

  char dir[STRLEN];
  int i, empty; 





  /* on enregistre les fonctions suivantes pour qu'elles soient executees
     lors de l'appel a exit()
   */
	atexit(finish);
	atexit(delete_lock_file);
	atexit(delete_event_list);
  /* on installe les gestionnaires de signaux */
	signal(SIGALRM,sigalarm_handler);
	signal(SIGINT,sigint_handler);
	signal(SIGQUIT,sigquit_handler);



  /* Being paranoid is not (yet) a crime */
  for ( i= 0; i< STRLEN; i++ ) dir[i] = '\0'; 

  /* on change le repertoire racine du demon */
  sprintf( dir, "%s/%s", HOME, getlogin() ); 
  
  if ( chdir( dir ) )
    {
      perror("Failed to change working dir");
      exit( EXIT_FAILURE ); 
    }
  
  fprintf( stdout, "Cron daemon launched (PID %u)\n", getpid() );

  if ( check_if_launched() == EXIT_SUCCESS )
    {
      fprintf( stderr, "Daemon already running\n" );
      return EXIT_FAILURE; 
    }

  /* on cree le fichier de verrou */
  create_lock_file();

  /* on charge la liste des evenements */
  parse_file( CRONDFLT );   

  /* on vire tous les elements du passe */
  list_check_if_empty(l,empty);
  
  while ( l->content && (int)(((event*)l->content)->t) - time( NULL ) < 0 && !empty )
    {
      list_del_first( l );
      list_check_if_empty( l, empty );
    }

  /* on regarde si la liste est vide */
  list_check_if_empty( l, empty );

  /* s'il y a au moins un evenement, on colle une alarme dessus */
  if ( !empty )
    {
      alarm( (int)(((event*)l->content)->t) - time( NULL ) );
  
      fprintf( stderr, "Next event in %d sec\n",  (int)((((event*)l->content)->t) - time( NULL )) );
    }
  else
    {
      fprintf( stderr, "Nothing to do yet!\n" );
    }
  
  /* Notre demon ne fait rien d'autre que gerer des signaux et des alarmes. 
     Il passe donc le plus clair de son temps -Aà roupiller comme un bienheureux. -b
   */
  while( 1 )
    {
      sleep( 1 );
    }
  
  exit( EXIT_SUCCESS );
}



int daemonize( ) 
{

  pid_t launch_pid = 0, daemon_pid = 0;
  launch_pid = fork();
	if (launch_pid > 0 ){/*pere*/
		exit(1);	
	}
	else{
		setsid();
		daemon_pid = fork();
		if (daemon_pid > 0 ){/*fils*/
			exit(1);
		}
		else{/*petit_fils*/
			code_demon(CRONDFLT);
		}
	} 	

	return EXIT_SUCCESS;
  
}

