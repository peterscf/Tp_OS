#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <sys/time.h>

#define STRLEN   128
#define HOME     "/users/phelma/phelma2015/"
#define CRONDFLT "./cron.dat"
#define LOCKFILE "cron.pid"

#define EOJ      "End of jobs"

#define DEBUG(x) fprintf( stderr, "file %s, line %d: "#x"=%d (dec) - %x (hex)\n", __FILE__, __LINE__, (int)(x), (int)(x) )

typedef struct event_t
{

  time_t t; /* on trie la liste des evenements sur le temps */
  char command[STRLEN];

} event; 



#endif
