//Contains functions and structures that allow for keeping the history of deesh.

#include "history.h"

//Creates a returns a new history tracker.
int new_tracker(int max_history_in, command_history_t ** location_in) {
  syslog(LOG_DEBUG, "Creating history tracker");

  command_history_t * new_tracker; //The tracker to be created

  //Attempt to create the history tracker
  if ((new_tracker = (command_history_t *) malloc(sizeof(command_history_t))) == NULL) {
    return 1;
  }
  
  //Attempt to create the list to hold history commands
  if (new_list(&new_tracker->list) != 0) {
    return 2;
  }
  
  //Set the new trackers maximum history count
  new_tracker->max_history = max_history_in;
  
  //Store a pointer to the new tracker in the specified location
  *location_in = new_tracker;

  return 0;
}

//Frees the history to avoid memory leaks
int free_history(command_history_t * history_in) {
  syslog(LOG_DEBUG, "Freeing memory for history tracker");

  //Free the memory for the list of commands
  free_list(history_in->list);

  //Free the memory for the history tracker
  free(history_in);

  return 0;
}

//Adds an item to the history tracker
int add_history(command_history_t * history_in, char * command_in) {
  syslog(LOG_DEBUG, "Adding command to history: %s", command_in);

  //Add the history item to the end of the command list
  add_back(history_in->list, command_in);

  //If maximum history has been reached remove oldest history item
  while (history_in->list->count > history_in->max_history) {
    remove_front(history_in->list);
  }

  return 0;
}

//Prints the last input number of history items
int print_history(command_history_t * history_in, int items_in) {
  syslog(LOG_DEBUG, "Printing last %d history items", items_in);

  struct list_node_t * start; //The first node to start printing from
  int counter;                //The amount of history items to print
 
  //Find the oldest history item to print
  if (history_in->list->count <= items_in) {
    start = history_in->list->first;
  } else {
    start = history_in->list->last;
    counter = 1;
    while (counter < items_in) {
      start = start->previous;
      counter = counter + 1;
    }
  }

  syslog(LOG_DEBUG, "Found first history item to print");

  syslog(LOG_INFO, "");

  //Print each command from history starting from the found beginning command
  while (start != NULL) {
    syslog(LOG_INFO, (char *) start->data);
    start = start->next;
  }

  return 0;
}