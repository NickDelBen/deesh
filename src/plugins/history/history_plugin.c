//A shell plugin to keep track of history

#include "history_plugin.h"

//Checks if the input command is the exit command
int is_exit_command(char * command_in, int * result_location) {
  syslog(LOG_DEBUG, "Checking if string is exit command");
  int command_iterator;   //Used to iterate over string
  char current_character;
  
  //Initially assume it is exit command
  *result_location = 1;

  //Read the character following the input command
  current_character = *(command_in + EXIT_COMMAND_LENGTH);
  //The location after the word history should be the end of the command or a space
  if (current_character != COMMAND_TOKEN_DELIMETER && current_character != STRING_TERMINATOR) {
    *result_location = 0;
    return 0;
  }

  //Check to ensure the command is the exit command
  for(command_iterator = 0; command_iterator < EXIT_COMMAND_LENGTH; ++command_iterator) {
    //Read the next character of the input command
    current_character = *(command_in + command_iterator);
    //Check that the current character matches
    if (current_character != COMMAND_EXIT_LOWER[command_iterator] && current_character != COMMAND_EXIT_UPPER[command_iterator]) {
      //If character does not match this is not exit command
      *result_location = 0;
      break;
    }
  }
  
  return 0;
}

//Checks if the input command is a history command
int is_history_command(char * command_in, int * result_location) {
  syslog(LOG_DEBUG, "Checking if string is history command");
  int command_iterator;   //Used to iterate over string
  char current_character;
  
  //Initially assume it is a history command
  *result_location = 1;

  //Read the character following the input command
  current_character = *(command_in + COMMAND_PREFIX_LENGTH);
  //The location after the word history should be the end of the command or a space
  if (current_character != COMMAND_TOKEN_DELIMETER && current_character != STRING_TERMINATOR) {
    *result_location = 0;
    return 0;
  }

  //Check to ensure the command is the history command
  for(command_iterator = 0; command_iterator < COMMAND_PREFIX_LENGTH; ++command_iterator) {
    //Read the next character of the input command
    current_character = *(command_in + command_iterator);
    //Check that the current character matches
    if (current_character != COMMAND_PREFIX_UPPER[command_iterator] && current_character != COMMAND_PREFIX_LOWER[command_iterator]) {
      //If character does not match this is not a history command
      *result_location = 0;
      break;
    }
  }
  
  return 0;
}

//Checks the amount of history commands to print
int parse_history_command(char * command_in, int * result_location) {
  syslog(LOG_DEBUG, "Parsing history command");

  char trailing_character;

  //Check if the default history is requested
  if (command_in[COMMAND_PREFIX_LENGTH] == STRING_TERMINATOR) {
    *result_location = DEFAULT_HISTORY_COMMANDS;
    return 0;
  }
  //Extract the number of commands from the input string
  *result_location = atoi(command_in + COMMAND_PREFIX_LENGTH + 1);
  //Check if there was bo valid integer found
  if (*result_location == 0) {
    return 1;
  }
  //Check if ther were trailing characters after the integer parameter
  trailing_character = *(command_in + COMMAND_PREFIX_LENGTH + 1 + num_digits(*result_location));
  if (trailing_character != NEW_LINE && trailing_character != STRING_TERMINATOR) {
    return 1;
  }
  return 0;
}

//Listens for commands on stdin
int listen_for_command(int descriptor_in, char ** location_in) {
  syslog(LOG_DEBUG, "Listening for command");

  int message_size;   //The amount of characters in the received message.
  char * new_message; //The message that was received

  char readbuffer[LISTENER_BUFFER_SIZE];

  //Receive a command on the specified descriptor
  message_size = read(descriptor_in, readbuffer, sizeof(readbuffer));
  //If the second last character is a newline trim it
  if (readbuffer[message_size - 2] == NEW_LINE) {
    syslog(LOG_DEBUG, "Trimming newline character from received command.");
    readbuffer[message_size - 2] = STRING_TERMINATOR;
  }

  //Allocate memory to store the received message
  new_message = (char *) malloc(message_size);

  //Copy the received message into the space allocated for the command
  strcpy(new_message, readbuffer);

  syslog(LOG_DEBUG, "Received new command: '%s'", new_message);

  //Store the received command in the specified location
  *location_in = new_message;

  return 0;
}

//Helper function: Counts the digits in the specified integer up to 10 digits
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