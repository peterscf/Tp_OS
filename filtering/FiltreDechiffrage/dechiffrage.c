#include <stdlib.h>
#include <stdio.h>

int main ( int argc, char ** argv )
{
	int key= atoi(argv[1]);
	int c;
	
	while (EOF != (c=fgetc(stdin))){
		printf("%c", c-key);
	}
    exit( EXIT_SUCCESS );
}
