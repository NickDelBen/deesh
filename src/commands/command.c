//Contains functions and structures that allow for control and handling of shell commands

#include "command.h"

//Frees the memory alocated for a command
int command_free(command_t * command_in) {
  syslog(LOG_DEBUG, "Freeing command");

  free(command_in);

  syslog(LOG_DEBUG, "Command free successful");

  return 0;
}