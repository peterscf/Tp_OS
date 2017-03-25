#include <stdio.h>

#include "handlers.h"

void print_plugin_handler( plugin_handler h )
{
  fprintf( stderr, "Plugin %12s : helper @ %p \troutine @ %p\n", h.name, h.help, h.apply );
}

