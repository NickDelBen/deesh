//A plugin for deesh

#include "plugin.h"

//Creates a new plugin from the input plugin file
int create_plugin(char * filepath_in, plugin_t ** location_in) {
  syslog(LOG_DEBUG, "Creating new plugin");

  FILE * file_pointer;
  plugin_t * new_plugin;
  char * line;
  size_t length;
  ssize_t read;
  int setting_iterator;
  char * setting_value;
  char current_char;
  int result_status;
  int function_result;

  //The default lineread length is 0
  length = 0;
  line = NULL;

  //Check that there was no problem opening the plugin file
  if ((file_pointer = fopen(filepath_in, "r")) == NULL) {
    return 1;
  }

  //Allocate space for the new plugin
  new_plugin = (plugin_t *) malloc(sizeof(plugin_t));
  new_plugin->command = NULL;
  new_plugin->plugin_path = NULL;
  new_plugin->persist = DEFAULT_PERSIST;
  new_plugin->parsed = DEFAULT_PARSED;

  //Assume success initially
  result_status = 0;

  //Read the information from the file
  while ((read = getline(&line, &length, file_pointer)) != -1) {
    syslog(LOG_DEBUG, "Found line '%s' in plugin file", line);
    //Extract the information from the line
    setting_iterator = 0;
    while (setting_iterator < read) {
      current_char = *(line + setting_iterator);
      //If we find the delimeter for key value pair we can stop reading
      if (current_char == PLUGIN_DELIMETER) {
        break;
      }
      ++setting_iterator;
    }
    syslog(LOG_DEBUG, "Plugin line parsed ended in character: '%c'", current_char);
    //If we did not read any characters, the setting is invalid
    if (setting_iterator == 0) {
      result_status = 2;
      break;
    }
    //If the character we stopped on is the newline character this is an invalid setting file
    if (current_char == '\n') {
      result_status = 2;
      break;
    }
    //If the character we stopped on is not the plugin delimeter the plugin file is invalid
    if (current_char != PLUGIN_DELIMETER) {
      result_status = 2;
      break;
    }
    syslog(LOG_DEBUG, "Last character of line: '%c'", *(line + read - 1));
    //Terminate the variable name
    *(line + setting_iterator) = STRING_TERMINATOR;
    //Terminate the variable value
    if (*(line + read - 1) == NEW_LINE) {
      *(line + read - 1) = STRING_TERMINATOR;
    }    
    syslog(LOG_DEBUG, "Parsing plugin setting setting: '%s' value: '%s' ", line, line + setting_iterator + 1);
    //Attempt to set a plugin setting from the read line
    if ((function_result = set_value(new_plugin, line, line + setting_iterator + 1)) != 0) {
      syslog(LOG_ERR, "Incorrectly formatted plugin file. Result: '%d'", function_result);
      //If the setting was invalid the file is not properly formatted
      result_status = 2;
      break;
    }
  }

  //Close the file we are reading from
  fclose(file_pointer);
  //If there is data in the line buffer free it
  if (line) {
    free(line);
  }
  //If the file was invalid free the memory for the plugin
  if (result_status != 0) {
    free_plugin(new_plugin);
  } else {
    *location_in = new_plugin;
  }

  return result_status;
}

//Sets a member for the input command to the specified value
int set_value(plugin_t * plugin_in, char * variable_in, char * value_in) {
  syslog(LOG_DEBUG, "setting value for plugin");

  if (strcmp(variable_in, PARSED_VARIABLE) == 0) {
    syslog(LOG_DEBUG, "Found parse variable");
    //Make sure the field isnt already set
    if (plugin_in->parsed != DEFAULT_PARSED) {
      return 3;
    }
    switch(*value_in) {
      case '0':
        plugin_in->parsed = 0;
        return 0;
      case '1':
        plugin_in->parsed = 1;
        return 0;
      default:
        return 2;
    }
  } else if (strcmp(variable_in, COMMAND_VARIABLE) == 0) {
    syslog(LOG_DEBUG, "Found command variable");
    //Plugin only can have associated command if it requires parsing
    if (plugin_in->parsed != 1) {
      return 2;
    }
    //Make sure the field is not already set
    if (plugin_in->command != NULL) {
      return 3;
    }
    //Copy the string into the command space
    plugin_in->command = (char *) malloc(strlen(value_in) + 1);
    strcpy(plugin_in->command, value_in);
    return 0;
  } else if (strcmp(variable_in, PERSIST_VARIABLE) == 0) {
    syslog(LOG_DEBUG, "Found persistence variable");
    //Persistance can only work if the command is unparsed
    if (plugin_in->parsed != 0) {
      return 2;
    }
    //Make sure the persistance field is not already set
    if (plugin_in->persist != DEFAULT_PERSIST) {
      return 3;
    }
    //Handle the result of the plugin value
    switch(*value_in) {
      case '0':
        plugin_in->parsed = 0;
        return 0;
      case '1':
        plugin_in->parsed = 1;
        return 0;
      default:
        return 2;
    }
  } else if (strcmp(variable_in, PATH_VARIABLE) == 0) {
    syslog(LOG_DEBUG, "Found path variable");
    //Ensure the field is not set already
    if (plugin_in->plugin_path != NULL) {
      return 3;
    }
    //Copy the path to the plugin object
    plugin_in->plugin_path = (char *) malloc(strlen(value_in) + 1);
    strcpy(plugin_in->plugin_path, value_in);
    return 0;
  } else {
    //Reaching here means there is no variable associated with the specified value
    return 1;
  }
  return 0;
}

//Frees the specified command from memory
int free_plugin(plugin_t * plugin_in) {
  syslog(LOG_DEBUG, "Freeing plugin from memory");
  free(plugin_in);
  return 0;
}

