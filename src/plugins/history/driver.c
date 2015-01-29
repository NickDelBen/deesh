//Driver for the shell history plugin

#include <syslog.h>

#include "history_plugin.h"
#include "history.h"

#define INPUT_DESCRIPTOR 0

#define COMMAND_TYPE_UNKNOWN 0
#define COMMAND_TYPE_EXIT    1
#define COMMAND_TYPE_HISTORY 2

//Return
//  0 if terminates nicely
//  1 if terminates with error
int main(int argc, char *argv[]) {
  //Initilize the logger
  openlog("shell-history", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
  setlogmask(LOG_UPTO(LOG_INFO));

  int command_result;
  char * received_command;
  int command_type;
  int requested_commands;
  int command_check;

  //The tracker to store all the history
  command_history_t * tracker;

  //Initilize the history tracker
  if ((command_result = new_tracker(MAX_HISTORY_ITEMS, &tracker)) != 0){
    syslog(LOG_ERR, "Encountered error %d when initilizing history tracker", command_result);
    return 1;
  }

  do {
    //Assume the type of command is unknown
    command_type = COMMAND_TYPE_UNKNOWN;
    //Listen for a command
    if ((command_result = listen_for_command(INPUT_DESCRIPTOR, &received_command)) != 0) {
      syslog(LOG_ERR, "Encountered error %d when listening for command", command_result);
      return 1;
    }
    //Check if the received command is the exit command
    if ((command_result = is_exit_command(received_command, &command_check)) != 0) {
      syslog(LOG_ERR, "Encountered error %d when checking for exit command. Skipping command.", command_result);
      continue;
    }
    //If we found an exit command set the command type
    if (command_check) {
      command_type = COMMAND_TYPE_EXIT;
    }
    //If the command is still unidentified check if the received command is a history command
    if (command_type == COMMAND_TYPE_UNKNOWN) {
      if ((command_result = is_history_command(received_command, &command_check)) != 0) {
        syslog(LOG_ERR, "Encountered error %d when checking for hist command. Skipping command.", command_result);
        continue;
      }
      //If we found a history command set the command type
      if (command_check) {
        command_type = COMMAND_TYPE_HISTORY;
      }
    }
    //Handle a command to be added to history
    if (command_type == COMMAND_TYPE_UNKNOWN) {
      if ((command_result = add_history(tracker, received_command)) != 0) {
        syslog(LOG_WARNING, "Encountered error %d when adding command to history. Skipping command.", command_result);
      }
      continue;
    }
    //Handle a command to display history items
    if (command_type == COMMAND_TYPE_HISTORY) {
      syslog(LOG_DEBUG, "Found history command. Attempting to handle.");
      //Find the amount of history items to print
      if ((command_result = parse_history_command(received_command, &requested_commands)) != 0) {
        syslog(LOG_WARNING, "Encountered error %d when finding amount of commands to display. Skipping command.", command_result);
        continue;
      }
      //Print the specified number of history items
      if ((command_result = print_history(tracker, requested_commands)) != 0) {
        syslog(LOG_WARNING, "Encountered error %d when printing last %d commands. Terminating print job.", command_result, requested_commands);
      }
      continue;
    }
    syslog(LOG_DEBUG, "Command has been handled successfully. Continuing to listen.");
  } while (command_type != COMMAND_TYPE_EXIT);

  syslog(LOG_DEBUG, "Received exit command. History plugin terminating.");

  //Free the history from memory
  if ((command_result = free_history(tracker)) != 0) {
    syslog(LOG_ERR, "Encountered error %d when freeing command history. Terminating history plugin.");
    return 1;
  }
  syslog(LOG_DEBUG, "History plugin terminated smoothly.");
  
  return 0;
}