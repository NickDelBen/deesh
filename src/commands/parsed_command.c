//Contains functions and structures that allow for control and handling of shell commands

#include "parsed_command.h"

//Creates a new command from the input string
int read_command(char * input_in, parsed_command_t * command_in, char ** parser_location) {
  syslog(LOG_DEBUG, "Creating new command");

  char * string_iterator; //Iterator to hit each character in the input comand string
  int reading_token;      //Boolean to keep track of whether or not a token is currently being parsed
  command_t * result;     //The newly generated command
  int result_status;      //The result of reading the command

  //Allocate memory for the command
  result = (command_t *) malloc(sizeof(command_t));
  result->num_tokens = 0;

  string_iterator = input_in;
  reading_token = 0;

  //Assume success
  result_status = 0;

  //Iterate through the string and tokenize
  while (*string_iterator != STRING_TERMINATOR && *string_iterator != COMMAND_PIPE && result_status == 0) {
    switch (*string_iterator) {
      case NEW_LINE:
        syslog(LOG_DEBUG, "Read Command: Found newline character");
        *string_iterator = STRING_TERMINATOR;
        continue;
      case CARRIAGE_RETURN:
        syslog(LOG_DEBUG, "Read Command: Found carrage return character");
        *string_iterator = STRING_TERMINATOR;
        continue;
      //Check if we hit a blank space
      case ' ':
        //if we are currently reading a token terminate the token
        if (reading_token) {
          *string_iterator = STRING_TERMINATOR;
          reading_token = 0;
        }
        ++string_iterator;
        continue;
      //Check for input redirection
      case INPUT_REDIRECT:
        syslog(LOG_DEBUG, "Read Command: Found input redirection");
        //If this is not the first command do not allow input redirection
        if (command_in->num_commands != 0) {
          return 1;
        }
        //If we have too many input files stop parsing
        if (command_in->has_input_file) {
          result_status = 1;
          break;
        }
        //Replace the current character with a termiantor
        *string_iterator = STRING_TERMINATOR;
        ++string_iterator;
        //If we find a space after the redirect skip this space
        if (*string_iterator == ' ') {
          syslog(LOG_DEBUG, "Read Command: Found space after input redirect");
          ++string_iterator;
        }
        //Error if the character after the redirect symbol is special terminate
        if (is_special_character(*string_iterator)) {
          result_status = 3;
          break;
        }        
        command_in->input_file = string_iterator;
        reading_token = 1;
        command_in->has_input_file = 1;
        continue;
      //Check for output redirection
      case OUTPUT_REDIRECT:
        syslog(LOG_DEBUG, "Read Command: Found output redirection");
        //If we have too many output files stop parsong
        if (command_in->num_output_files >= MAX_OUTPUT_REDIRECTS) {
          result_status = 2;
          break;
        }
        //Replace the current character with a termiantor
        *string_iterator = STRING_TERMINATOR;
        ++string_iterator;
        //If we find a space after the redirect skip this space
        if (*string_iterator == ' ') {
          syslog(LOG_DEBUG, "Read Command: Found space after output redirect");
          ++string_iterator;
        }
        //Error if the character after the redirect symbol is special terminate
        if (is_special_character(*string_iterator)) {
          result_status = 4;
          break;
        }
        //Account for the space following the redirection symbol
        command_in->output_files[command_in->num_output_files] = string_iterator;
        reading_token = 1;
        command_in->num_output_files = command_in->num_output_files + 1;
        continue;
      //No special characters were found
      default:
        //If we are starting a new token add the token to the token list
        if (! reading_token) {
          syslog(LOG_DEBUG, "Read Command: Found start of new token");
          result->tokens[result->num_tokens] = string_iterator;
          result->num_tokens = result->num_tokens + 1;
          reading_token = 1;
        }
        ++string_iterator;
    }    
  }
  
  //Check if a command has no tokens
  if (result->num_tokens < 1) {
    result_status = 5;
  }

  //Terminate the token list to exec does not read too far
  result->tokens[result->num_tokens] = NULL;

  syslog(LOG_DEBUG, "Read Command: Storing parsed command with %d tokens", result->num_tokens);
  //Store the location where parsing finished in the specified location
  *parser_location = string_iterator;
  //Store the new command in next command location
  command_in->commands[command_in->pipes] = result;
  command_in->num_commands = command_in->num_commands + 1;

  syslog(LOG_DEBUG, "Read Command: Stored command '%s' in location '%d'\n", result->tokens[0], command_in->pipes);

  return result_status;
}

//Parses an input command from the user
int process_command(char * input_in, parsed_command_t ** location_in) {
  syslog(LOG_DEBUG, "Process Command: Parsing input command");

  parsed_command_t * new_command; //The newly created command
  char * command_pointer;         //Points to character of string currently being parsed.
  int read_result;                //The result of parsing a command
  int result_status;              //The ending result

  //Create the new command
  new_command = (parsed_command_t *) malloc(sizeof(parsed_command_t));
  new_command->pipes = 0;
  new_command->num_output_files = 0;
  new_command->has_input_file = 0;
  new_command->num_commands = 0;
  
  command_pointer = input_in;

  result_status = 0;

  //Trim the leading spaces from the command
  while (*command_pointer == ' ') {
    ++command_pointer;
  }

  //Check if the command was empty
  if (*command_pointer == STRING_TERMINATOR) {
    result_status = 7;
  }

  //Check if the command starts with a pipe
  if (*command_pointer == COMMAND_PIPE) {
    result_status = 5;
  }

  while (*command_pointer != STRING_TERMINATOR && result_status == 0) {
    syslog(LOG_DEBUG, "Process Command: Found char: %c", *command_pointer);
    //Read in the command
    read_result = read_command(command_pointer, new_command, &command_pointer);
    syslog(LOG_DEBUG, "Process Command: Command has been read");
    //Handle te result of reading the command
    switch(read_result) {
      //case 0:
      //  command_pointer++;
      //  continue;
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
        result_status = read_result;
        continue;
    }
    //If there is a pipe we need to parse another command
    if (*command_pointer == COMMAND_PIPE) {
      syslog(LOG_DEBUG, "Process Command: found pipe in command");
      //Check if we already have output redirection
      if (new_command->num_output_files != 0) {
        //Can only have output redirection in the last command
        return 2;
      }
      //Terminate the steing
      *command_pointer = STRING_TERMINATOR;
      new_command->pipes = new_command->pipes + 1;
      //If we have too many pipes stop parsing
      if (new_command->pipes > MAX_COMMAND_PIPES) {
        result_status = 6;
        continue;
      }
      ++command_pointer;
    }
  }

  //Store the length of the command to free later
  new_command->command_length = new_command->string - command_pointer;
  
  //Store the parsed command in the specified location
  *location_in = new_command;
  
  //If there are pipes and redirects in the same command, set the appropriate termination state
  if (result_status == 0 && new_command->pipes > 0 && (new_command->num_output_files > 0 || new_command->has_input_file)) {
    return 8;
  }
  
  return result_status;
}

//Frees the memory allocated for a parsed command
int parsed_free(parsed_command_t * command_in) {
  syslog(LOG_DEBUG, "Freeing memory for parsed command");

  int command_iterator;
  
  syslog(LOG_DEBUG, "Freeing commands");
  //Free the commands TODO0
  for (command_iterator = 0; command_iterator < command_in->num_commands; command_iterator++){
    syslog(LOG_DEBUG, "Attempting to free command '%d'", command_iterator);
    command_free(command_in->commands[command_iterator]);
  }

  syslog(LOG_DEBUG, "Freeing parsed command structure");
  //Free the memory associated with the input command
  free(command_in);

  return 0;
}

//Checks if a character is a special character
int is_special_character(char character_in) {
  switch(character_in) {
    case STRING_TERMINATOR:
    case INPUT_REDIRECT:
    case OUTPUT_REDIRECT:
    case CARRIAGE_RETURN:
    case COMMAND_PIPE:
    case NEW_LINE:
      return 1;
    default:
      return 0;
  }

}
