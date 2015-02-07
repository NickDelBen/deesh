/***********************************************************
 * Contains functions for storing a history item in deesh history
 *
 * Created By: Nick DelBen
 * Created On: February 6, 2015
 *
 * Last Modified: February 6, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _H_HISTORY_SAVER
#define _H_HISTORY_SAVER

#include <syslog.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define NEW_LINE '\n'
#define STRING_TERMINATOR '\0'

//Stores the specified command in the specified history file
//Parameters
//  filepath_in path to the file history is stored in
//  command_in  command t0nbe stored
//Return
//  0 success
//  1 error accessing file
//  2 error writing to file
int store_history(char * filepath_in, char * command_in);

#endif