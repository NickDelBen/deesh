/***********************************************************
 * Contains functions and structures that allow for control 
 * and handling of shell commands
 *
 * Created By: Nick DelBen
 * Created On: January 22, 2015
 *
 * Last Modified: January 22, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _PARSED_COMMAND_H
#define _PARSED_COMMAND_H

#include <syslog.h>
#include <string.h>
#include <stdlib.h>

#include "command.h"

#define MAX_COMMAND_PIPES 32   //The maximum amout of pipes in one command
#define MAX_OUTPUT_REDIRECTS 1 //The maximum amount of output redirects

//Command Special Characters
#define STRING_TERMINATOR '\0'
#define COMMAND_PIPE '|'
#define INPUT_REDIRECT '<'
#define OUTPUT_REDIRECT '>'
#define CARRIAGE_RETURN '\r'
#define NEW_LINE '\n'

//The structure to hold the result of parsing an input command
typedef struct {
  int num_commands;                            //The number of commands
  char * string;                               //Points to string containing command
  size_t command_length;                       //The amounf of characters in the input command
  command_t * commands[MAX_COMMAND_PIPES + 1]; //The parsed commands
  int pipes;                                   //The amount of pips found in the command
  int num_output_files;                        //The number of output files we are redirecting stdout to
  char * output_files[MAX_OUTPUT_REDIRECTS];   //Files to read input to stdin from
  int has_input_file;                          //Flag to see if command ahs input files
  char * input_file;                           //File to write stdout to
} parsed_command_t;

//Creates a new command and stores the pointer to it in the specified location
//Runs in O(1n) where n is the characters in the command
//WARNING: input is destroyed
//DO NOT FORGET TO command_free() the command when you are no longer using it.
//Parameters
//  input_in        pointer to string containing the command
//  location_in     pointer to the memory location to store the new command
//  output_file     pointer to a filename to redirect stdio to
//  input_file      pointer to a filename to redirect stdin from
//  parser_location pointer to location to store where parsing stopped
//Return
//  0 successful
//  1 too many input redirects
//  2 too many output redirects
//  3 invalid source for input redirection
//  4 invalid destination for output redirection
//  5 Invalid command
int read_command(char * input_in, parsed_command_t * command_in, char ** parser_location);

//Parses an input command from the user
//DO NOT FORGET TO parsed_free()
//Parameters
//  input_in    the string to tokenize and turn into a command
//  location_in the memory location to store the command in
//Return
//  0 successful
//  1 too many input redirects
//  2 too many output redirects
//  3 invalid source for input redirection
//  4 invalid destination for output redirection
//  5 invalid command
//  6 too many pipes
//  7 no command entered
//  8 pipes and redirection in same command
int process_command(char * input_in, parsed_command_t ** location_in);

//Frees the memory allocated for a parsed command
//Parameters
//  command_in the command to be freed
//Return
//  0 successful
int parsed_free(parsed_command_t * command_in);

//Checks if a character is a special character
//Parameters
//  character_in the character to check
//Return
//  0 if character is not special
//  1 if character is special
int is_special_character(char character_in);

#endif