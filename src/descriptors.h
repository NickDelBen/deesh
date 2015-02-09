/***********************************************************
 * Handles the
 *
 * Created By: Nick DelBen
 * Created On: February 4, 2015
 *
 * Last Modified: February 4, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _H_DESCRIPTORS
#define _H_DESCRIPTORS

#include <stdio.h>
#include <syslog.h>

//Data item for holding stream infiormation
typedef struct {
  FILE * descriptor;        //The descriptor this stream is associated with
  int fd_location;          //The current file descriptor
  fpos_t * reader_location; //The index of the stream reader
} stream_t;

//Copies one specified descriptor to another specified descriptor
//Parameters
//  previous_descriptor_in descriptor to redirect from
//  new_descriptor_in      descriptor to redirect to
//Return
//  0 - success
//  1 - descriptors already match
//  2 - could not redirect
int redirect_descriptor(int previous_descriptor_in, int new_descriptor_in);

//Saves a descriptor to the specified location
//Parameters
//  filename_out filename to read the new stream from
//  stream_in    stream to store the information in
//  file_in      open descriptor we are backing up
//  open_mode_in mode to open the file with
//Return
//  0 succes
int backup_descriptor(char * filename_out, stream_t * stream_in, FILE * file_in, char * open_mode_in);
int backup_descriptor_in(char * filename_out, stream_t * stream_in, FILE * file_in);
int backup_descriptor_out(char * filename_out, stream_t * stream_in, FILE * file_in);

//Restores a descriptor from the specified location
//Parameters
//  stream_in stream to restore the information from
//Return
//  0 success
int restore_descriptor(stream_t * stream_in);

#endif