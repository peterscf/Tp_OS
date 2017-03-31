#include <unistd.h> /* sysconf */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

/*
#ifdef malloc
#undef malloc
#endif

#ifdef free
#undef free
#endif
*/
typedef unsigned char byte; 

void* my_malloc( size_t size )
{
  byte *ptr = NULL;
  byte *ptr_M = NULL;
  size_t pgsz = sysconf( _SC_PAGE_SIZE );
  size_t M = (size + sizeof(size_t))/pgsz+2;
  
  printf("M: %u\n",(unsigned int)M); 
  
  
  ptr_M = mmap(NULL,M*pgsz,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE , -1,0);
  
  if ( MAP_FAILED == ptr_M ) {
    fprintf( stderr, "MMAP: %s\n", strerror( errno ) );
  }
  
  printf("Address page : %p\n",ptr_M);
  ptr = ptr_M + M*pgsz - pgsz - size; 
  if ( -1 == mprotect(ptr_M + M*pgsz-pgsz,pgsz,PROT_NONE) ) {
    fprintf( stderr, "MPROTECT: %s\n", strerror( errno ) );
  }
  
  *(((size_t*)ptr)-1) = M;
  
  return ptr; 
}

void my_free( void* ptr )
{
  size_t pgsz = sysconf( _SC_PAGE_SIZE );
  size_t M = pgsz * *(((size_t*)ptr)-1);
  void  *p = (void*)(pgsz*((size_t)ptr/pgsz));

  if ( -1 == munmap( p, M ) ) {
    fprintf( stderr, "MUNMAP: %s\n", strerror( errno ) );
  }
}

