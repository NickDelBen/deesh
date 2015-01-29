/***********************************************************
 * A shell plugin to keep track of history
 *
 * Created By: Nick DelBen
 * Created On: January 28, 2015
 *
 * Last Modified: January 29, 2014
 *   - Added listener function
 **********************************************************/

#ifndef _HISTORY_PLUGIN_H
#define _HISTORY_PLUGIN_H

#include <syslog.h>
#include <string.h>
#include <stdlib.h>

//The default number of commands to show if no parameter is given
#define DEFAULT_HISTORY_COMMANDS 10

//The maximum number of history items to keep track of
#define MAX_HISTORY_ITEMS 256

//The command prefix for history
#define COMMAND_PREFIX_LENGTH 7
#define COMMAND_PREFIX_UPPER "HISTORY"
#define COMMAND_PREFIX_LOWER "history"

//The command to exit the shell
#define EXIT_COMMAND_LENGTH 4
#define COMMAND_EXIT_LOWER "exit"
#define COMMAND_EXIT_UPPER "EXIT"

//Special command characters
#define COMMAND_TOKEN_DELIMETER ' '
#define STRING_TERMINATOR '\0'
#define NEW_LINE '\n'

//Amount of characters for buffer to store incoming commands
#define LISTENER_BUFFER_SIZE 1024

//Listens for commands on stdin
//Parameters
//  descriptor_in the file descriptor to listen on
//  location_in   location to store received command
//Return
//  0 successful
int listen_for_command(int descriptor_in, char ** location_in);

//Checks if the input command is the exit command
//Parameters
//  command_in      the command to check
//  result_location the location to store result of check
//                  stores 1 if is exit command / 0 if is not
//Return
//  0 successful
int is_exit_command(char * command_in, int * result_location);

//Checks if the input command is a history command
//Parameters
//  command_in      the command to check
//  result_location the location to store result of check
//                  stores 1 if is history command / 0 if is not
//Return
//  0 successful
int is_history_command(char * command_in, int * result_location);

//Checks the amount of history commands to print
//Parameters
//  command_in
//  result_location
//Return
//  0 successful
//  1 invalid amount of commands
int parse_history_command(char * command_in, int * result_location);

//Checks the amount of digits in an integer
//Parameters
//  number_in the number to count the digits of
//Return
//  number of digits in specified number
int num_digits(int number_in);

#endif