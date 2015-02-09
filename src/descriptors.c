//Functions for manipulating file descriptors

#include "descriptors.h"

//Copies one specified descriptor to another specified descriptor
int redirect_descriptor(int previous_descriptor_in, int new_descriptor_in) {
  syslog(LOG_DEBUG, "Redirecting discriptor '%d' to descriptor '%d'", previous_descriptor_in, new_descriptor_in);
  
  //Check that the descriptors are not already duplicated
  if (previous_descriptor_in == new_descriptor_in) {
    return 1;
  }

  //Redirect the descriptor
  if (dup2(previous_descriptor_in, new_descriptor_in) == -1) {
    //If duplication failed this could break the whole pipeline so we must stop execution
    syslog(LOG_DEBUG, "Failed to redirect descriptor");
    terminate_execution();
  }

  //Close the old descriptor
  close(previous_descriptor_in);

  return 0;
}

//Saves an input descriptor
int backup_descriptor_out(char * filename_out, stream_t * stream_in, FILE * file_in) {
  backup_descriptor(filename_out, stream_in, file_in, "w");
  return 0;
}

//Saves an input descriptor
int backup_descriptor_in(char * filename_out, stream_t * stream_in, FILE * file_in) {
  backup_descriptor(filename_out, stream_in, file_in, "r");
  return 0;
}

//Saves a descriptor to the specified location
int backup_descriptor(char * filename_out, stream_t * stream_in, FILE * file_in, char * open_mode_in) {
  syslog(LOG_DEBUG, "Backing up stream");
  stream_in->descriptor = file_in;
  fflush(file_in);
  fgetpos(file_in, stream_in->reader_location);
  stream_in->fd_location = dup(fileno(file_in));
  freopen(filename_out, open_mode_in, file_in); 
  return 0;
}

//Restores a descriptor from the specified location
int restore_descriptor(stream_t * stream_in) {
  syslog(LOG_DEBUG, "Restoring stream");
  //Flush the buffer for the stream
  fflush(stream_in->descriptor);
  dup2(stream_in->fd_location, fileno(stream_in->descriptor));
  close(stream_in->fd_location);
  clearerr(stream_in->descriptor);
  fsetpos(stream_in->descriptor, stream_in->reader_location);
  return 0;
}