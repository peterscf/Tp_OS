#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <dlfcn.h>  /* Use dynamic loader */
#include <dirent.h> /* Directory listing  */

#include "handlers.h"
#include "tricklist.h"

extern int verbose; 

void plugin_term( list *plugins )
{
  plugin_handler h;
  int i; 

  for ( i= list_get_count( *plugins ) - 1; i > -1; i-- )
    {
      list_del_first( *plugins, h );

      if ( verbose )
	fprintf( stderr, "Closing handler %p for plugin %s\n", h.handler, h.name );

      dlclose( h.handler );
    }

  list_delete( *plugins );
  *plugins = NULL;
  
  return;
}


void plugin_scan( char* dir, list *plugins )
{
  DIR* directory;
  struct dirent* direntry;
  uint n    = 0; 
  uint nold = 0; 

  if ( *plugins != NULL )
    {
      nold = list_get_count( *plugins );
      plugin_term( plugins );
    }
  
  
  *plugins = list_init( );
  list_set_name( *plugins, "of plugins" );

  directory = opendir( dir );

  if ( directory != NULL )
    {
      while ( ( direntry = readdir( directory ) ) != NULL )
	{
	  char *ptr = NULL;

	  ptr = strstr( direntry->d_name, ".so" );

	  if ( ptr != NULL )
	    {
	      char  sname[LEN];
	      plugin_handler h; 

	      memset( h.name, '\0', LEN );
	      memset(  sname, '\0', LEN );
	      
	      sprintf( sname, "%s/%s", dir, direntry->d_name );
	      memcpy( h.name, direntry->d_name, ptr - direntry->d_name );

	      /*
		A ce stade, sname, contient le nom du fichier contenant
		le plugin, et h.name contient le nom de la routine *
		principale du plugin. Vous stockerez dans h.name le nom
		de la routine d'affichage d'aide du plugin.
		Pour ajouter un descripteur de plugin a la liste des
		plugins, utilisez : list_add_last( *plugins, h );

		TODO : Remplissez le descripteur de plugin h et ne
		l'ajoutez a la liste des plugins que si vous avez ete
		capable de trouver les deux fonctions qui vous interessent
		a l'interieur. Pensez a incrementer la variable n en cas
		de succes !

		En cas d'erreur, vous afficherez les messages issus du
		chargeur sur stderr. 
	       */
	      


	      
	    } /* ptr != NULL */	  
	}

      if ( n!=nold )
	{
	  if ( n > nold )
	    {
	      fprintf( stderr, "\nDiscovered %d new plugin%s\n", n-nold, (n-nold)==1?"":"s" );
	    }
	  else
	    {
	      fprintf( stderr, "\n%d plugin%s got deleted\n", nold-n, (nold-n)==1?"":"s" );
	    }
	}
      
      closedir( directory );

      if ( verbose )
	{
	  list_apply_function( *plugins, print_plugin_handler, plugin_handler ); 
	  verbose = 0; 
	}

    }
  else
    {
      perror( "opendir" );
      return;
    }

  return; 
}

int plugin_lookup( char* plugname, list plugins, plugin_handler *h )
{
  int found = list_find_by_value( plugins, *h, name, strcasecmp, plugname, plugin_handler );

  return found;
}


