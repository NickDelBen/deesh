//Driver for the printer modiule of the deesh history plugin.

#include <syslog.h>

#include "history_common.h"
#include "printer/history_printer.h"
#include "printer/stream_printer.h"

#define SYSLOG_LOG_LEVEL LOG_WARNING

int main(int argc, char **argv) {
  int requested_items;

  //Initilize the logger
  openlog("deesh-history-printer", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
  setlogmask(LOG_UPTO(SYSLOG_LOG_LEVEL));

  //Check the program call
  switch (argc) {
    case 1:
      syslog(LOG_DEBUG, "No item number specified. Defaulting to %d items", DEFAULT_HISTORY_ITEMS);
      requested_items = SYSLOG_LOG_LEVEL;
      break;
    case 2:
      syslog(LOG_DEBUG, "Specified %s history items to be printed", argv[1]);
      if (parse_number_items(argv[1], &requested_items) == 0) {        
        break;
      }
      syslog(LOG_DEBUG, "Invalid amount of items %s specified for printing");
    default:
      syslog(LOG_ERR, "Invalid program call. Correct syntax is <executable> <number_of_items>");
      return 1;
  }

  //Print the specified number of items
  print_history(HISTORY_FILEPATH, requested_items, MAX_COMMAND_LENGTH);

  return 0;
}