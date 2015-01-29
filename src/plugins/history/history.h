/***********************************************************
 * Contains functions and structures that allow for 
 * keeping the history of delshell.
 *
 * Created By: Nick DelBen
 * Created On: January 14, 2015
 *
 * Last Modified: January 14, 2014
 *   - Created Initially
 **********************************************************/

#ifndef _HISTORY_H
#define _HISTORY_H

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list/linked_list.h"

//Structure containing the information for the history tracker
typedef struct {
  linked_list_t * list;
  int max_history;
} command_history_t;

//Creates a returns a new history tracker.
//DO NOT FORGET TO FREE THE MEMORY USING free_history() WHEN DONE
//Parameters
//  max_history_in the maximum amout of commands to store the history for
//  location_in    the memory location to store a pointer to th history tracker in
//Return
//  0 successful
//  1 failed to create history tracker
//  2 failed to create list for command history
int new_tracker(int max_history_in, command_history_t ** location_in);

//Adds an item to the history tracker
//Parameters
//  command_in the command to be added to history
//Return
//  0 successful
int add_history(command_history_t * history_in, char * command_in);

//Prints the last input number of history items
//If there are less commands than the input number
//the function will print all history
//Parameters
//  history_in the history tracker to print from
//  items_in   the amount of history items to print
//Return
//  0 successful
int print_history(command_history_t * history_in, int items_in);

//Frees the history to avoid memory leaks
//Parameters
//  history_in the history tracker to be freed from memory
//Return
//  0 if successful
int free_history(command_history_t * history_in);

#endif