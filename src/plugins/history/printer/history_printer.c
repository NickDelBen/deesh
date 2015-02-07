//Contains functions for printing the stored history of dshell

#include "history_printer.h"

//Prints the specified number of history items from the history
int print_history(char * filepath_in, int items_in, int command_length_in) {
  syslog(LOG_DEBUG, "Printing history from file %s", filepath_in);

  FILE * file_stream;

  //Open the history file to read from
  if ((file_stream = fopen(filepath_in, "r")) == NULL) {
    syslog(LOG_ERR, "Print History: %s", strerror(errno));
    return 1;
  }

  //Print the specified number of history items
  if (print_last_lines(file_stream, items_in, command_length_in) != 0) {
    syslog(LOG_ERR, "Print History: Error printing lines");
    fclose(file_stream);
    return 1;
  }
  
  //Close the history file
  fclose(file_stream);

  syslog(LOG_DEBUG, "Successully printed requested history items");

  return 0;
}

//Checks the amount of history commands to print
int parse_number_items(char * items_in, int * result_location) {
  syslog(LOG_DEBUG, "Extracting number of history items to be printed from %s", items_in);

  char trailing_character;

  //Extract the number of commands from the input string
  *result_location = atoi(items_in);
  //Check if there was bo valid integer found
  if (*result_location == 0) {
    return 1;
  }
  //Check if ther were trailing characters after the integer parameter
  trailing_character = *(items_in + num_digits(*result_location));
  if (trailing_character != NEW_LINE && trailing_character != STRING_TERMINATOR) {
    return 1;
  }
  
  syslog(LOG_DEBUG, "Successfully found %d history items were requested", *result_location);
  return 0;
}

//Checks the amount of digits in an integer
int num_digits(int number_in) {
    if (number_in < 0) number_in = -1 * number_in;
    if (number_in < 10) return 1;
    if (number_in < 100) return 2;
    if (number_in < 1000) return 3;
    if (number_in < 10000) return 4;
    if (number_in < 100000) return 5;
    if (number_in < 1000000) return 6;
    if (number_in < 10000000) return 7;
    if (number_in < 100000000) return 8;
    if (number_in < 1000000000) return 9;
    return 10;
}