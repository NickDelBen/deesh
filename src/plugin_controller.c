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
        break;
      default:
        syslog(LOG_DEBUG, "An unknown error has occured");
    }
    //Handle memory management
    if (temp_plugin->command != NULL) {
      syslog(LOG_DEBUG, "New plugin has command");
      add_back(new_controller->allocations, temp_plugin->command);
    }
    if (temp_plugin->plugin_path != NULL) {
      syslog(LOG_DEBUG, "New plugin has path");
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