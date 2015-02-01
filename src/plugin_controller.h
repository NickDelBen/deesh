/***********************************************************
 * A manager to handle the plugins for deesh
 *
 * Created By: Nick DelBen
 * Created On: January 29, 2015
 *
 * Last Modified: January 29, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _PLUGIN_CONTROLLER_H
#define _PLUGIN_CONTROLLER_H

#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <syslog.h>

#include "plugin.h"
#include "linked_list/linked_list.h"

#define MAX_PLUGIN_SIZE 128

//Deals with the plugins
typedef struct {
  char * plugin_directory;     //The directory plugins are stored in
  linked_list_t * preparse;    //The plugins that run before an input command is parsed will just be a list of
  linked_list_t * postparse;   //The plugins that run after an input command is parsed
  linked_list_t * allocations; //Allocated strings that need to be frees
} plugin_controller_t;

//Creates a new plugin manager
//Parameters
//  directory_in the directory where plugins are stored
//  location_in  the memory location to store the new plugin parser
//Return
//  0 successful
//  1 failed to scan for plugins
int init_plugin_controller(char * directory_in, plugin_controller_t ** location_in);

//Frees the specified plugin controller from memory
//Parameters
//  controller_in the plugin to be freed
//Return
//  0 success
int free_plugin_controller(plugin_controller_t * controller_in);

#endif