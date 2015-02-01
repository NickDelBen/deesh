#include <stdio.h>

#include "plugin_controller.h"

#define PLUGIN_DIRECTORY "plugins/\0"

int main() {
  //Initilize the logger
  openlog("deesh", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
  setlogmask(LOG_UPTO(LOG_WARNING));

  plugin_controller_t * plugins;

  //Create the plugin controller
  switch(init_plugin_controller(PLUGIN_DIRECTORY, &plugins)) {
    case 0:
      syslog(LOG_DEBUG, "Plugin Controller sucessfully initiated");
    case 1:
      syslog(LOG_DEBUG, "Unable to scan for plugins");
    default:
      syslog(LOG_DEBUG, "An unknown error occured while initiating plugin controller");
  }




  //Free the memory allocated for plugins
  free_plugin_controller(plugins);

  return 0;
}