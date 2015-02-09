/***********************************************************
 * A manager to handle the plugins for deesh
 *
 * Created By: Nick DelBen
 * Created On: January 29, 2015
 *
 * Last Modified: February 1, 2015
 *   - Added allocations list to get rid of unreachable strings
 **********************************************************/

#ifndef _PLUGIN_CONTROLLER_H
#define _PLUGIN_CONTROLLER_H

#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <syslog.h>

#include "plugin.h"
#include "linked_list/list_node.h"
#include "linked_list/linked_list.h"

#define PLUGIN_DIRECTORY "./plugins/\0"

//Deals with the plugins
typedef struct {
  char * plugin_directory;     //The directory plugins are stored in
  linked_list_t * preparse;    //The plugins that run before an input command is parsed
  linked_list_t * postparse;   //The plugins that run after an input command is parsed
  linked_list_t * allocations; //Allocated strings that need to be freed
} plugin_controller_t;

//Creates a new plugin manager
//Parameters
//  directory_in the directory where plugins are stored
//  location_in  the memory location to store the new plugin parser
//Return
//  0 successful
//  1 failed to scan for plugins
int init_plugin_controller(char * directory_in, plugin_controller_t ** location_in);

//Checks if a specified command is a plugin
//Parameter
//  command_in  command to check
//  plugins_in  set of plugis to compare command against
//  location_in location to store pointer to the plugin associated
//Return
//  0 is not a plugin
//  1 is a plugin
int check_if_plugin(char * command_in, plugin_controller_t * plugins_in, plugin_t ** loation_in);

//Frees the specified plugin controller from memory
//Parameters
//  controller_in the plugin to be freed
//Return
//  0 success
int free_plugin_controller(plugin_controller_t * controller_in);

//Executes the preprocess commands
//Parameters
//  plugins_in the commands to run this on
//  command_in the command to runt he plugins on
//Return
//  0 success
//  1 failure
int run_preprocess_command(plugin_controller_t * plugins_in, char * command_in);

#endif