#include <stdio.h>

#include "tab.h"

unsigned int max( unsigned int * tab, unsigned int sz ) 
{
  unsigned int i; 
  unsigned int m = tab[0]; 

  for ( i = 1; i < sz; i++ )
    if ( tab[i] > m ) 
      m = tab[i]; 

  return m; 
}

void print_tab( unsigned int * tab, unsigned int sz ) 
{
  unsigned int i; 

  for ( i = 0; i < sz; i++ )
    fprintf( stderr, "%d ", tab[i] ); 
  
  fprintf( stderr, "\n" ); 
  
  return; 
}
