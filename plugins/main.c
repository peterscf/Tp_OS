#define _XOPEN_SOURCE 600 /* isblank */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <strings.h>
#include <sys/time.h>

#include "handlers.h"
#include "mat.h"
#include "core.h"

/* Les variables globales, c'est mal */

/* Repertoire ou se trouvent les plugins */
static char plugdir[] = "plugins";

/* Liste des plugins */
static list plugins   = NULL;

/* Image courante */
static mat  img       = NULL;

int verbose = 1; 


/* Huhu... fgets reagit a SIGALRM aussi...
   Meme bug sous Solaris : "so it really
   really looks like fgets() would have
   problems with a SIGALRM coming in and
   doesn't just re-try on EINTR. Can
   Solaris stdio _really_ be that broken?
   (Yeah, yeah, it may be "conforming".
   It's also so incredibly
   programmer-unfriendly that it's not
   even funny)" -- Linus Torvalds.
   Faut croire que la GlibC aussi a qques
   soucis :)
   En fait, il suffit d'appliquer ce petit
   patch en degageant les erreurs indues
   de fgets avec un bon gros clearerr sur
   le flux, et... 
   (And then we can just do

	while (safe_fgets(..))

   like normal people again, and not be
   afraid of strange stdio implementations)
   -- Linus Torvalds.
*/
char *safe_fgets(char *s, int size, FILE *stream)
{
  for (;;)
    {
      if (fgets(s, size, stream))
	return s;
      if (feof(stream))
	return NULL;

      clearerr(stream);
    }
}


void timer_reset(int n)
{
  struct itimerval timer;

  timer.it_interval.tv_sec  = 2;
  timer.it_interval.tv_usec = 0;
  
  timer.it_value.tv_sec  = 2;
  timer.it_value.tv_usec = 0;
  
  plugin_scan( plugdir, &plugins );

  setitimer( ITIMER_REAL, &timer, NULL );
  signal( SIGALRM, timer_reset );
  
}

void total_reset( int signum )
{
  fprintf( stderr, "\nPerforming reset!\n" );
  if ( img )
    mat_delete( img );
  img = NULL;
  timer_reset( 0 );
  
  signal( SIGINT, total_reset );
}

int main ( int argc, char ** argv )
{
  signal( SIGINT, total_reset );
  
  timer_reset( 0 );

  fprintf( stderr, "\nStarting Quick'n'Dirty SHell\n");
  fprintf( stderr, "-- type Ctrl-D to exit\n-- type Ctrl-C to reset\n\n" );
  
  do
    {
      plugin_handler h;
      int found = 0;
      char cmd[ LEN<<4 ];
      
      fprintf( stderr, "$ " );
      
      memset( cmd, '\0', LEN<<4 );
      
      if( NULL == safe_fgets( cmd, LEN<<4, stdin ) )
	{
	  fprintf( stderr, "Exiting shell\n\n" );
	  verbose = 1;
	  plugin_term( &plugins );
	  fprintf( stderr, "\n" );
	  break;
	}

      /* On vire le '\n' final */
      cmd[ strlen( cmd ) - 1 ] = '\0';

      if ( !strcasecmp( cmd, "pluglist" ) )
	{
	  if ( 0 < list_get_count( plugins ) )
	    list_apply_function( plugins, print_plugin_handler, plugin_handler );
	  else
	    fprintf( stderr, "No plugins!\n" );
	}
      else if ( !strncasecmp( cmd, "help", 4 ) )
	{
	  char *p = cmd+4;
	  while ( isblank(*p++) );

	  p--;

	  if ( *p == '\0' )
	    {
	      fprintf( stderr, "help <plugname>\npluglist\n" );
	    }
	  else
	    {
	      found = plugin_lookup( p, plugins, &h );
	      
	      if ( found )
		{
		  void (*f) (void) = h.help; 
		  f();
		}
	      else
		{
		  fprintf( stderr, "Plugin %s not found\n", p );
		}
	    }
	}
      else if ( strlen( cmd ) > 0 ) /* Skip void commands */
	{
	  char cmd_save[ LEN<<4 ];
	  char cmd_name[ LEN<<4 ];
	  char cmd_args[ LEN<<4 ];
	  char *ptr, *save_ptr;
	  char blanks[] = " \t\0";

	  memset( cmd_save, '\0', LEN<<4 );
	  memset( cmd_name, '\0', LEN<<4 );
	  memset( cmd_args, '\0', LEN<<4 );

	  memcpy( cmd_save, cmd, LEN<<4 );

	  save_ptr = cmd;
	  
	  ptr = strtok( cmd, blanks );

	  memcpy( cmd_name, ptr, strlen( ptr ) );

	  ptr = strtok( NULL, blanks );

	  if ( ptr )
	    memcpy( cmd_args, &cmd_save[ptr-save_ptr], strlen( &cmd_save[ptr-save_ptr] ) );

	  found = plugin_lookup( cmd_name, plugins, &h );
	  
	  if ( found )
	    {
	      /*
		A ce stade, h contient les handlers du plugin a lancer.
		cmd_name contient le nom du plugin a lancer
		cmd_args contient les arguments a passer au plugin
		TODO : Ecrivez le code de lancement de la routine
		principale du plugin. 
	       */
	      
	    }
	  else
	    {
	      fprintf( stderr, "Plugin %s not found\n", cmd );
	    }
	}

    } while ( 1 );
  
  exit( EXIT_SUCCESS );
}

