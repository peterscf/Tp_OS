
#ifndef _CORE_H_
#define _CORE_H_

#include "tricklist.h"

void plugin_scan( char* dir, list *plugins );
void plugin_term( list *plugins );
int  plugin_lookup( char *, list, plugin_handler* );

#endif

