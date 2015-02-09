//Contains functions for printing from a file stream

#include "stream_printer.h"

//Prints the last lines from a text file
int print_last_lines(FILE * stream_in, int lines_in, int max_length) {
  syslog(LOG_DEBUG, "Printing %d lines from specified stream");

  char stream_buffer[lines_in * max_length + 1];
  int characters_read;
  char * result_beginning;
  
  //Read characters from the file stream
  if (read_stream_end(stream_in, lines_in * max_length, &characters_read, &stream_buffer) != 0) {
    syslog(LOG_ERR, "Print File: %s", strerror(errno));
    return 2;
  }

  //Terminate the read characters
  stream_buffer[characters_read] = STRING_TERMINATOR;
  //Find the specified lines from the specified file
  find_last_lines(lines_in, (char *) &stream_buffer, characters_read - 1, &result_beginning);
  //Print the found lines
  printf("%s\n", result_beginning);

  syslog(LOG_DEBUG, "Print File: Successfully printed lines from stream");
  return 0;
}

//Reads a specified number of bytes from the end an input stream and stores them in the specified location
int read_stream_end(FILE * stream_in, int bytes_in, int * result_read, void * location_in) {
  syslog(LOG_DEBUG, "Read Stream: Reading %d bytes from stream", bytes_in);

  int bytes_read;
  int file_size;

  //Get the byte size of the file
  if (fseek(stream_in, 0, SEEK_END) == -1) {
    return 1;
  }
  if ((file_size = ftell(stream_in)) == -1) {
    return 1;
  }
  //Set the read start location
  if (fseek(stream_in, file_size <= bytes_in ? 0 : file_size - bytes_in, SEEK_SET) == -1) {
    return 1;
  }
  
  //Read the data in from the specified stream
  bytes_read = fread(location_in, 1, bytes_in, stream_in);
  //The number of bytes read is one less to remove the EOF
  *result_read = bytes_read - 1;
  
  syslog(LOG_DEBUG, "Read Stream: Sucessfully read %d bytes from stream", *result_read);  
  return 0;
}

//Finds the first character of the forst of a specified number of lines
int find_last_lines(int lines_in, char * string_in, int string_length, char ** result_in) {
  syslog(LOG_DEBUG, "Find Lines: Searching for last %d lines", lines_in);

  char * char_iterator;
  int lines_read;

  //Initally have found no lines
  lines_read = 0;
  //Start reading from the last character
  char_iterator = string_in + string_length;
  //Trim the trailing newlines
  while (*char_iterator == NEW_LINE) {
    *(char_iterator--) = STRING_TERMINATOR;
  }
  //Keep checking for newlines until we have found the specified amount of lines or read the whole string
  while (lines_read < lines_in && char_iterator > string_in) {
    //If we find a newline character it means we have finished reading a line
    if (*(--char_iterator) == NEW_LINE) {
      ++lines_read;
    }
  }
  //Skip ahead because of over-decrementation of character pointer
  *result_in = char_iterator + 1;
  
  syslog(LOG_DEBUG, "Find Lines: Sucessfully found %d lines", lines_read);
  return 0;
}