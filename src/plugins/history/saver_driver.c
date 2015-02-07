//Driver for the printer modiule of the deesh history plugin.

#include <syslog.h>

#include "history_common.h"
#include "saver/history_saver.h"

#define SYSLOG_LOG_LEVEL LOG_DEBUG

int main(int argc, char **argv) {
  //Initilize the logger
  openlog("deesh-history-printer", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
  setlogmask(LOG_UPTO(SYSLOG_LOG_LEVEL));
  
  //Check the program call
  switch (argc) {
    case 2:
      break;
    case 1:
      syslog(LOG_DEBUG, "No history item specified.");
    default:
      syslog(LOG_ERR, "Invalid program call. Correct syntax is <executable> <shell_command>");
      return 1;
  }

  //Store the command in the history
  if (store_history(HISTORY_FILEPATH, argv[1]) != 0) {
    syslog(LOG_DEBUG, "There was an error saving the command. History was not updated");
  }

  return 0;
}