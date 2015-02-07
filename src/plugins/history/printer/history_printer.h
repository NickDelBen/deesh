/***********************************************************
 * Contains functions for printing the stored history of dshell
 *
 * Created By: Nick DelBen
 * Created On: February 6, 2015
 *
 * Last Modified: February 6, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _H_HISTORY_PRINTER
#define _H_HISTORY_PRINTER

#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

#include "stream_printer.h"

//The default number of history items to print
#define DEFAULT_HISTORY_ITEMS 10

#define NEW_LINE '\n'
#define STRING_TERMINATOR '\0'

//Prints the specified number of history items from the history
//Parameters
//  filepath_in       path to the history file
//  items_in          number of items to print
//  command_length_in maximum amount of characters in a shell command
//Return
//  0 success
int print_history(char * filepath_in, int items_in, int command_length_in);

//Attempts to convert a string to an integer and store it in a specified location
//Parameters
//  items_in        the string to be converted
//  result_location location to store the extracted integer
//Return
//  0 success
//  1 string is invalid integer
int parse_number_items(char * items_in, int * result_location);

//Checks the amount of digits in an integer
//Parameters
//  number_in the integer to count the digits of
//Return
//  number of digits in the integer
int num_digits(int number_in);

#endif