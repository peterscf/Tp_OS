#include <stdlib.h>
#include <stdio.h>

#include "tricklist.h"


#include "daemon.h"

#define syntaxe(x) "Syntaxe:\n%s\t lance\n",x

/* list of events */
list l = NULL;

int main ( int argc, char ** argv )
{

  switch( argc )
    {
    case 1 : 
      daemonize( );
      break;
    default :
      fprintf( stderr, syntaxe(argv[0]) );
      break;
    }
  
  exit( EXIT_SUCCESS ); 

}
