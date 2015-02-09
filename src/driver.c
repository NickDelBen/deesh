//deesh shell application

#include "plugin_controller.h"
#include "deesh.h"

#include <syslog.h>
#include <stdlib.h>

#define PREFIX_TRAIL " %% \0"

int main() {
  //Initilize the logger
  openlog("deesh", LOG_PERROR | LOG_NDELAY | LOG_NOWAIT, LOG_USER);
  setlogmask(LOG_UPTO(LOG_WARNING));

  plugin_controller_t * plugins;
  char * username;
  char * prefix;
  int userlen;

  username = getenv("USER");
  prefix = (char*) malloc(strlen(username) + strlen(PREFIX_TRAIL) + 1);
  strcpy(prefix, username);
  strcat(prefix, PREFIX_TRAIL);

  //Create the plugin controller
  switch(init_plugin_controller(PLUGIN_DIRECTORY, &plugins)) {
    case 0:
      syslog(LOG_DEBUG, "Plugin Controller sucessfully initiated");
    case 1:
      syslog(LOG_DEBUG, "Unable to scan for plugins");
    default:
      syslog(LOG_DEBUG, "An unknown error occured while initiating plugin controller");
  }

  //Begin listening for commands
  listen_for_commands(prefix, plugins);  

  //Free the prefix
  free(prefix);
 
  //Free the memory allocated for plugins
  free_plugin_controller(plugins);

  return 0;
}