  //deesh - The multipurpose extensible shell

#include "deesh.h"

//Listens for user input commands
int listen_for_commands(char * command_prefix, plugin_controller_t * plugins_in) {
  syslog(LOG_DEBUG, "Listen for commands: Listening for command input");

  char input_buffer[INPUT_BUFFER_SIZE];

  while(1) {
    //Print the command prefix
    printf(command_prefix);
    //Read an input line
    fgets(input_buffer, INPUT_BUFFER_SIZE, stdin);
    syslog(LOG_DEBUG, "Listen for commands: input command '%s'", (char *) input_buffer);
    //Run the preprocess plugins on the specified command
    if (! is_exit_string((char *) input_buffer)) {
      if (run_preprocess_command(plugins_in, input_buffer) == 2) {
        syslog(LOG_DEBUG, "Listen For Commands: Error running preprocess commands");
        return 0;
      }
    }    
    //Push the command to the command handler
    switch(handle_command((char *) input_buffer, plugins_in)) {
      //If there is an exit command return and free
      case 2:
        syslog(LOG_DEBUG, "Listen For Commands: Command was exit");
        return 0;
    }
  }
  return 0;
}

//Handle an input command
int handle_command(char * command_in, plugin_controller_t * plugins_in) {
  syslog(LOG_DEBUG, "Handle Command: User input '%s' attempting to parse command", command_in);

  parsed_command_t * new_command;
  int valid_command;
  int command_iterator;
  int exit_flag;
  stream_t stdin_stream;
  stream_t stdout_stream;

  plugin_t * plugin;

  //Initially assume command is valud
  valid_command = 1;

  //Attempt to parse the input command
  switch(process_command(command_in, &new_command)) {
    case 0:
      break;
    case 8:
      syslog(LOG_DEBUG, "Handle Command: Pipes and redirects found in same command");
      parsed_free(new_command);
      return 0;
    case 1:
      syslog(LOG_DEBUG, "Can have maximum of 1 input redirect in the first command");
      valid_command = 0;
      break;
    case 2:
      syslog(LOG_DEBUG, "Can have maxmimum of 1 output redirect in the last command");
      valid_command = 0;
      break;
    case 3:
      syslog(LOG_DEBUG, "The source parameter for input redirection is invalid");
      valid_command = 0;
      break;
    case 4:
      syslog(LOG_DEBUG, "The destination parameter for output redirection is invalid. Output redirection must come in the last command");
      valid_command = 0;
      break;
    case 5:
      syslog(LOG_DEBUG, "The input command is invalid");
      valid_command = 0;
      break;
    case 6:
      syslog(LOG_DEBUG, "Input command exceeds maximum (%d) amount of pipes", MAX_COMMAND_PIPES);
      valid_command = 0;
      break;
    case 7:
      break;
    default:
      syslog(LOG_DEBUG, "An unknown error occured when processing the command");
  }

  //If the command was invalid free the memory for the command and return unsuccessful
  if (! valid_command) {
    parsed_free(new_command);
    return 1;
  }

  //Check to see if any commands are plugin commands
  for (command_iterator = 0; command_iterator < new_command->num_commands; command_iterator++) {
    if (check_if_plugin(new_command->commands[command_iterator]->tokens[0], plugins_in, &plugin)) {
      //Create a new string for the modified plugin
      new_command->commands[command_iterator]->tokens[0] = plugin->plugin_path;
    }
  }

  //Check to see if there was an exit command received
  if (has_exit_command(new_command)) {
    parsed_free(new_command);
    return 2;
  }

  //Check for input redirection
  if (new_command->has_input_file) {
    syslog(LOG_DEBUG, "Handle Command: Found input file");
    //If the file does not exist stop execution
    if (! file_exists(new_command->input_file)) {
      syslog(LOG_WARNING, "Handle Command: Specified input file does not exist");
      parsed_free(new_command);
      return 1;
    }
    backup_descriptor_in(new_command->input_file, &stdin_stream, stdin);
  }

  //Check for output redirection
  if (new_command->num_output_files != 0) {
    syslog(LOG_DEBUG, "Handle Command: Found output file");
    backup_descriptor_out(new_command->output_files[0], &stdout_stream, stdout);
  }

  //Execute the specified command
  if(run_command(new_command) != 0) {
    syslog(LOG_DEBUG, "Handle Command: There was an error executing the command");
    parsed_free(new_command);
    return 2;
  }

  syslog(LOG_DEBUG, "Handle Command: Command run successfully");

  //Free the memory allocated for the command
  parsed_free(new_command);

  //Check if have to reset input
  if (new_command->has_input_file) {
    syslog(LOG_DEBUG, "Handle Command: Restoring stdin");
    restore_descriptor(&stdin_stream);
  }

  //Check if have to reset output
  if (new_command->num_output_files != 0) {
    syslog(LOG_DEBUG, "Handle Command: Restoring stdout");
    restore_descriptor(&stdout_stream);
  }

  return 0;
}

//Checks to see if an exit command was received
int has_exit_command(parsed_command_t * command_in) {
  syslog(LOG_DEBUG, "Has Exit Command: Checking for exit command");
  
  int command_iterator;
  
  //Check each command in the set of parsed commands
  for (command_iterator = 0; command_iterator < command_in->num_commands; command_iterator++) {
    if (is_exit_command(command_in->commands[command_iterator])) {
      return 1;
    }
  }

  syslog(LOG_DEBUG, "Has Exit Command: Command contains no exit commands");
  return 0;
}

//Checks if the specified command is an exit command
int is_exit_command(command_t * command_in) {
  syslog(LOG_DEBUG, "Is Exit Command: Checking if command is exit");

  int character_iterator;
  char * command;
  int target_length;

  target_length = strlen(EXIT_COMMAND_UPPER);
  command = command_in->tokens[0];

  for (character_iterator = 0; character_iterator < target_length; character_iterator++) {
    if (command[character_iterator] != EXIT_COMMAND_UPPER[character_iterator] && command[character_iterator] != EXIT_COMMAND_LOWER[character_iterator]) {
      return 0;
    }
  }

  return command[target_length] == STRING_TERMINATOR ? 1 : 0;
}

//Checks if the input string is an exit command
int is_exit_string(char * string_in) {
  syslog(LOG_DEBUG, "Checking if '%s' is exit string", string_in);
  
  int target_length;
  int character_iterator;
  char current_character;

  target_length = strlen(EXIT_COMMAND_UPPER);

  //If the word is not the same length it is not exit command
  if (string_in[target_length] != NEW_LINE && string_in[target_length] != STRING_TERMINATOR) {
    return 0;
  }

  for (character_iterator = 0; character_iterator < target_length; character_iterator++) {
    current_character = string_in[character_iterator];
    if(current_character != EXIT_COMMAND_UPPER[character_iterator] && current_character != EXIT_COMMAND_LOWER[character_iterator]) {
      return 0;
    }
  }

  return 1;
}

//Checks if a file exists
int file_exists(char * filename_in) {
  syslog(LOG_DEBUG, "Checking if file '%s' exists");
  FILE * test_file;

  //Try to open the file
  if (test_file = fopen(filename_in, "r")) {
    //If we open the file it exists, close it and we are in the clear
    fclose(test_file);
    return 1;
  }
  //If we dont open it, shit went wrong
  return 0;
}