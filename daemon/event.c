#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "tricklist.h"

#include "general.h"

/* localtime_r n'est pas une fonction ANSI C... */
struct tm *localtime_r (const time_t *timep, struct tm *result);

void printevent( void * content )
{

  struct tm d;
  event * e = (event *)content;

  localtime_r(&(e->t), &d);

  fprintf(stderr, "%02d:%02d:%02d :: %s\n", d.tm_hour, d.tm_min, d.tm_sec, e->command);
  
  return;
}

/*
  A line event is in the form:
  hh:mm:ss command
*/

static event parse_line( char * line )
{

  time_t tod; 
  struct tm tmtod; 
  unsigned int h, m, s, i;
  event e;
  char blanks[]={':','\t',' ','\0'};
  char * token = NULL;
  
  token = strtok(line, blanks);

  sscanf(token, "%u", &h);
  
  token=strtok(NULL, blanks);

  sscanf(token, "%u", &m);

  token=strtok(NULL, blanks);

  sscanf(token, "%u", &s);

  token=strtok(NULL, blanks);

  for (i= 0; i< STRLEN; i++) e.command[i]='\0';

  strncpy(e.command, token, STRLEN-1); 

  while ( (token=strtok( NULL, blanks ))!=NULL )
    {
      strcat( e.command, " " );
      strcat( e.command, token );      
    }

  time(&tod);
  
  localtime_r(&tod,&tmtod);

  tmtod.tm_sec=s; 
  tmtod.tm_min=m; 
  tmtod.tm_hour=h; 

  e.t=mktime(&tmtod);
  
  return e; 
}

list parse_file( char * cronfile )
{

  extern list l;
  
  FILE * fp = NULL; 
  char line[STRLEN];
  event e;
  unsigned int tmax = 0; 

  list_init( l );
  
  if ( !(fp=fopen( cronfile, "r" )) )
    {
      perror( "parse_file" );
      return NULL; 
    }

  while( fgets(line,STRLEN-1,fp) )
    {
      e=parse_line(line);
      list_add_by_index(l,e,t);

      if ( e.t > tmax ) tmax = e.t; 
      
    }

  fclose( fp );

  
  e.t = ++tmax;
  strcpy(e.command, EOJ);

  list_add_last(l,e);
  
  return l; 

}
