
#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#define LEN 48

typedef struct plugin_handler_t
{
  char name[ LEN ];
  void* handler;
  void* apply;
  void* help;
} plugin_handler;

void print_plugin_handler( plugin_handler h );

#endif

