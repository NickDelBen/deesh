//A manager to handle the plugins for deesh

#include "plugin_controller.h"

//Creates a new plugin manager
int init_plugin_controller(char * directory_in, plugin_controller_t ** location_in) {
  syslog(LOG_DEBUG, "Initilizing plugin controller");

  plugin_controller_t * new_controller;
  glob_t search_result;
  char * plugin_matcher;
  int glob_iterator;
  int folder_len;
  int plugin_path_len;
  int plugin_dir_len;

  //Allocate space for the new controller
  new_controller = (plugin_controller_t *) malloc(sizeof(plugin_controller_t));
  new_controller->plugin_directory = directory_in;
  //Initilize the lsit of preparse plugins
  new_list(&(new_controller->preparse));
  //Initilize the list of postparse commands
  new_list(&(new_controller->postparse));
  new_list(&(new_controller->allocations));

  //Determine the matching string for plugin files
  folder_len = strlen(directory_in);
  plugin_matcher = malloc(folder_len + strlen(PLUGIN_MATCH) + 1);
  strcpy(plugin_matcher, directory_in);
  strcpy(plugin_matcher + folder_len, PLUGIN_MATCH);

  syslog(LOG_DEBUG, "Searching for plugins that match path '%s'", plugin_matcher);

  //Search the plugin directory for plugin files    
  switch(glob(plugin_matcher, 0, NULL, &search_result)) {
    case 0:
      break;
    case GLOB_NOSPACE:
      return 1;
      break;
    case GLOB_ABORTED:
      return 1;
      break;
    case GLOB_NOMATCH:
    default:
      break;
  }

  syslog(LOG_DEBUG, "Found '%d' plugins", search_result.gl_pathc);

  //Parse each plugin file and create the necessary plugin objects
  for(glob_iterator = 0; glob_iterator < search_result.gl_pathc; glob_iterator++) {
    syslog(LOG_DEBUG, "Found plugin file '%s'", search_result.gl_pathv[glob_iterator]);
    plugin_t * temp_plugin;
    char * temp_path;
    //Create a plugin for the plugin editor
    switch (create_plugin(search_result.gl_pathv[glob_iterator], &temp_plugin)) {
      case 1:
        syslog(LOG_ERR, "There was an error opening the plugin file '%s'", search_result.gl_pathv[glob_iterator]);
        break;
      case 2:
        syslog(LOG_ERR, "The plugin file '%s' was not formatted corectly", search_result.gl_pathv[glob_iterator]);
        break;
      case 0:
        syslog(LOG_DEBUG, "Successfully created plugin from '%s'", search_result.gl_pathv[glob_iterator]);
        //Check to see if any commands are plugin commands
        plugin_path_len = strlen(temp_plugin->plugin_path);
        plugin_dir_len = strlen(directory_in);
        //Create a new string for the modified plugin
        temp_path = (char *) malloc(plugin_dir_len + plugin_path_len + 2);
        memcpy(temp_path, PLUGIN_DIRECTORY, plugin_dir_len + 1);
        temp_path[plugin_dir_len] = STRING_TERMINATOR;
        strcat(temp_path, temp_plugin->plugin_path);
        free(temp_plugin->plugin_path);
        temp_plugin->plugin_path = temp_path;
        syslog(LOG_DEBUG, "Init Plugin Controller: Modified path to include plugin directory '%s'", temp_plugin->plugin_path);
        break;
      default:
        syslog(LOG_DEBUG, "An unknown error has occured");
    }
    //Handle memory management
    if (temp_plugin->command != NULL) {
      syslog(LOG_DEBUG, "Init Plugin Controller: New plugin has command");
      add_back(new_controller->allocations, temp_plugin->command);
    }
    if (temp_plugin->plugin_path != NULL) {
      syslog(LOG_DEBUG, "Init Plugin Controller: New plugin has path");
      add_back(new_controller->allocations, temp_plugin->plugin_path);
    }
    //Add the new plugin to the plugin manager
    if (temp_plugin->parsed == 0) {
      add_back(new_controller->preparse,temp_plugin);
      continue;
    }
    add_back(new_controller->postparse,temp_plugin);
  }
  //Free the memory for the plugin match string
  free(plugin_matcher);
  //Free the memory used for the search
  globfree(&search_result);

  *location_in = new_controller;

  return 0;
}

//Checks if a specified command is a plugin
int check_if_plugin(char * command_in, plugin_controller_t * plugins_in, plugin_t ** location_in) {
  syslog(LOG_DEBUG, "Checking if '%s' is a plugin command", command_in);
  
  plugin_t * current_plugin;
  struct list_node_t * current_node;

  current_node = plugins_in->postparse->first;

  //Check each postprocess command to see if it matches
  while (current_node != NULL) {
    //The current plugin is the data in the current node 
    current_plugin = (plugin_t *) current_node->data;
    if (strcmp(current_plugin->command, command_in) == 0) {
      syslog(LOG_DEBUG, "Found plugin command");
      *location_in = current_plugin;
      return 1;
    }
    current_node = current_node->next;
  }

  return 0;
}

//Frees the specified plugin controller from memory
int free_plugin_controller(plugin_controller_t * controller_in) {
  syslog(LOG_DEBUG, "Freeing controller location");

  int alloc_iterator;

  //Free the memory allocations
  free_list(controller_in->allocations);

  //Free the linked list for pre-parse commands
  syslog(LOG_DEBUG, "Freeing preparse plugin list");
  free_list(controller_in->preparse);
  syslog(LOG_DEBUG, "Freeing postparse plugin list");
  //Free the linked list for the binary commands
  free_list(controller_in->postparse);

  //Free the memory allocated for the controller
  free(controller_in);

  return 0;
}

//Executes the preprocess commands
int run_preprocess_command(plugin_controller_t * plugins_in, char * command_in) {
  syslog(LOG_DEBUG, "Executing plugins on command '%s'", command_in);

  int is_command;
  char * character_iterator;
  plugin_t * current_plugin;
  struct list_node_t * current_node;
  char * executable[3];
  char * fixed_command;
  int command_len;
  int run_result; 

  //Initially assume command is valid
  is_command = 0;

  //Check to see if the command is not just full of spaces
  for (character_iterator = command_in; *character_iterator != NEW_LINE && *character_iterator != STRING_TERMINATOR; character_iterator++) {
    if (*character_iterator != ' ') {
      is_command = 1;
      break;
    }
  }
  //If the command was just full of spaces return success
  if (is_command == 0) {
    return 0;
  }

  executable[2] = NULL;
  syslog(LOG_DEBUG, "Run Preprocess Plugins: Command is valid");
  current_node = plugins_in->preparse->first;

  //Run each preparse plugin with the 
  while (current_node != NULL) {
    //The current plugin is the data in the current node 
    current_plugin = (plugin_t *) current_node->data;
    executable[0] = current_plugin->plugin_path;
    executable[1] = command_in;
    syslog(LOG_DEBUG, "Run Preprocess Plugins: Running plugin '%s' with parameter '%s'", executable[0], executable[1]);
    //Attempt to fork for the command execution
    switch(fork()) {
      case -1:
        //Parent process
        syslog(LOG_DEBUG, "Run Preprocess Plugins: The fork failed.");
        return 1;
      case 0:
        //Child process
        run_result = execvp(executable[0], executable);
        syslog(LOG_ERR, "Run Preprocess Plugins: Error running preprocess plugin result '%d'", run_result);
        return 2;
        break;
      default:
        //Parent process
        wait(0);
        break;
    }
    current_node = current_node->next;
  }
  
  syslog(LOG_DEBUG, "Run Preprocess Plugins: All preprocess plugins run successfully");
  return 0;
}