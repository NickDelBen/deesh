/***********************************************************
 * A plugin for deesh
 *
 * Created By: Nick DelBen
 * Created On: January 29, 2015
 *
 * Last Modified: January 31, 2015
 *   - Added function to parse a setting file line
 **********************************************************/

#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//The file extension for plugins 
#define PLUGIN_MATCH "*.dshp\0"

//Character that delemits plug files name from value
#define PLUGIN_DELIMETER '='
#define NEW_LINE '\n'

//Names of plugin variables
#define PARSED_VARIABLE "parsed\0"
#define COMMAND_VARIABLE "command\0"
#define PERSIST_VARIABLE "persist\0"
#define PATH_VARIABLE "path\0"

//Default value for plugin variables
#define DEFAULT_PERSIST -1
#define DEFAULT_PARSED -1

#define STRING_TERMINATOR '\0'
#define NEW_LINE '\n'

//A plugin that needs to persist data between runs
typedef struct {
  int parsed;         //Whether the plugin runs before or after plugin
  int persist;        //Whether or not a plugin has to be persistance between runs
  int fd_out;         //The file descriptor to push the commands to (only for non persistent commands)
  char * command;     //The command string to match against for this plugin
  char * plugin_path; //Path to the plugin, relative to the plugin directory
} plugin_t;

//Creates a new plugin from the input plugin file
//DO NOT FORGET TO free_plugin() when you are done
//Parameters
//  filepath_in         path to the information file for the plugin
//  location_in         location to store the newly created command
//Return
//  0 success
//  1 error opening plugin file
//  2 plugin file is not formed corectly
int create_plugin(char * filepath_in, plugin_t ** location_in);

//Sets a member for the input command to the specified value
//Parameters
//  plugin_in plugin tos et the value for
//  member_in variable of the plugin to be set
//  value_in  value to set the specified member to
//Return
//  0 value set successfully
//  1 invalid field
//  2 invalid value
//  3 field already set
int set_value(plugin_t * plugin_in, char * variable_in, char * value_in);

//Frees the specified plugin from memory
//Parameters
//  plugin_in the plugin to be freed
//Return
//  0 success
int free_plugin(plugin_t * plugin_in);

#endif