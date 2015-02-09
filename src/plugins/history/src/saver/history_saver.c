//Contains functions for storing a history item in deesh history

#include "history_saver.h"

//Stores the specified command in the specified history file
int store_history(char * filepath_in, char * command_in) {
  syslog(LOG_DEBUG, "Storing history command '%s'", command_in);

  FILE * file_stream;
  char * history_buffer;
  int command_length;

  //Find the size of the input command
  command_length = strlen(command_in);
  syslog(LOG_DEBUG, "Command Length: '%d'", command_length);
  //Allocate memory for the new command to be saved
  history_buffer = (char *) malloc(command_length + 2);
  //Copy the command to the created buffer
  memcpy(history_buffer, command_in, command_length);
  //Add newline and terminate the new command
  if (*(history_buffer + command_length - 1) != NEW_LINE) {
    syslog(LOG_DEBUG, "Appending newline to end of history item");
    *(history_buffer + command_length) = NEW_LINE;
    *(history_buffer + command_length + 1) = STRING_TERMINATOR;
  } else {
    *(history_buffer + command_length) = STRING_TERMINATOR;
  }  
  syslog(LOG_DEBUG, "New Command: '%s'", history_buffer);

  //Open the history file to write to
  if ((file_stream = fopen(filepath_in, "ab")) == NULL) {
    syslog(LOG_ERR, "Write History: %s", strerror(errno));
    return 1;
  }

  //Write the specified history item to the file
  if (fprintf(file_stream, history_buffer) < 0) {
    syslog(LOG_ERR, "Write History: %s", strerror(errno));
    fclose(file_stream);
    return 2;
  }

  //Free emory allocated for the command
  free(history_buffer);

  //Close the history file
  fclose(file_stream);

  syslog(LOG_DEBUG, "Successfully write history item to file '%s'", filepath_in);
  return 0;
}