/***********************************************************
 * Contains functions for printing from a file stream
 *
 * Created By: Nick DelBen
 * Created On: February 6, 2015
 *
 * Last Modified: February 6, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _H_STREAM_PRINTER
#define _H_STREAM_PRINTER

#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

#define NEW_LINE '\n'
#define STRING_TERMINATOR '\0'

//Prints the last lines from a text file
//Parameters
//  stream_in  stream to read the lines from
//  lines_in   amount of lines to print
//  max_length maximum mount of characters contained in each line
//Return
//  0 success
//  1 error opening file
//  2 error reading from file
int print_last_lines(FILE * stream_in, int lines_in, int max_length);

//Reads a specified number of bytes from the end an input stream and stores them in the specified location
//If there are less bytes in the stream than requested, stores all the bytes
//Parameters
//  stream_in   stream to read the bytes from
//  bytes_in    number of bytes to be read
//  result_read location to store number of bytes read
//  result_data location to store the read bytes
//Return
//  0 success
//  1 error parsing file
int read_stream_end(FILE * stream_in, int bytes_in, int * result_read, void * location_in);

//Finds the first character of the forst of a specified number of lines
//and stores a pointer to the character in the specified location
//Parameters
//  lines_in      amount of lines to be read
//  string_in     string to search for the lines in
//  string_length length of the string to search
//  result_in     location to store the pointer to the beginning of the lines in
//Return
//  0 success
int find_last_lines(int lines_in, char * string_in, int string_length, char ** result_in);

#endif