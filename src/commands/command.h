/***********************************************************
 * Contains functions and structures for a specific input
 * command
 *
 * Created By: Nick DelBen
 * Created On: January 15, 2015
 *
 * Last Modified: January 15, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _COMMAND_H
#define _COMMAND_H

#include <syslog.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_TOKENS 32  //The maximum amount of tokens in a command
#define MAX_OUTPUT_REDIRECTS 1 //The maximum amount of output redirects

//The structure to hold a command
typedef struct {
  char * tokens[MAX_COMMAND_TOKENS]; //The tokens associated with this command
  int num_tokens;                    //The amount of tokens in this command
} command_t;

//Frees the memory allocated for a command
//Parameters
//  command_in the command to be freed
//Return
//  0 successful
int command_free(command_t * command_in);

#endif