/***********************************************************
 * deesh - The multipurpose extensible shell
 *
 * Created By: Nick DelBen
 * Created On: February 3, 2015
 *
 * Last Modified: February 3, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _H_DEESH
#define _H_DEESH

#include <syslog.h>
#include <stdio.h>
#include <string.h>

#include "executor.h"
#include "commands/parsed_command.h"
#include "commands/command.h"
#include "plugin.h"
#include "plugin_controller.h"
#include "descriptors.h"

//The command to exit the shell
#define EXIT_COMMAND_UPPER "EXIT\0"
#define EXIT_COMMAND_LOWER "exit\0"

#define STRING_TERMINATOR '\0'

#define INPUT_BUFFER_SIZE 256

//Handle an input command
//Parameters
//  command_in command the user input
//  plugins_in plugins loaded for thuis shell session
//Return
//  0 success
//  1 invalid command
//  2 received exit command
int handle_command(char * command_in, plugin_controller_t * plugins_in);

//Checks to see if an exit command was received
//Parameters
//  commands_in the commands to check for exit
//Return
//  0 no exit command found
//  1 exit command found
int has_exit_command(parsed_command_t * command_in);

//Checks if the specified command is an exit command
//Parameters
//  command_in command to check
//Return
//  0 - is not exit command
//  1 - is exit command
int is_exit_command(command_t * command_in);

//Listens for user input commands
//Parameters
//  command_prefix prefix to put before each command
//  plugins_in     plugins loaded for thuis shell session
int listen_for_commands(char * command_prefix, plugin_controller_t * plugins_in);

//Checks if a file exists
//Parameters
//  filename_in path to the file to test
//Return
//  0 file does not exst
//  1 file exists
int file_exists(char * filename_in);

//Checks if the specified string is the exit string
//Parameters
//  string_in the string to check
//Return
//  0 is not exit string
//  1 is exit string
int is_exit_string(char * string_in);

#endif