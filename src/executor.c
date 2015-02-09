//Contains functions for executing commands

#include "executor.h"

#define DEFAULT_COMMAND_NUM 1

//Runs the specified command in a new process
int run_command(parsed_command_t * commands_in) {
  syslog(LOG_DEBUG, "Run Command: Receiving command with '%d' pipes", commands_in->pipes);

  pid_t pid;
  
  //Attempt to fork for the command execution
  switch(pid = fork()) {
    case -1:
      //Parent process
      syslog(LOG_DEBUG, "Run Command: The fork failed.");
      return 1;
    case 0:
      //Child process
      if (execute_pipeline(commands_in, DEFAULT_COMMAND_NUM, STDIN_FILENO) != 0) {
        syslog(LOG_DEBUG, "Child process returned failure");
        return 2;
      }
      terminate_execution();
      break;
    default:
      //Parent process
      wait(0);
      break;
  }
  return 0;
}

//Executes a command pipeline recursivly
int execute_pipeline(parsed_command_t * command_in, int command_num, int descriptor_in) {
  syslog(LOG_DEBUG, "Executing command pipeline. Command %d/%d", command_num, command_in->num_commands);

  command_t * current_command;

  //Extract the current command
  current_command = command_in->commands[command_num - 1];
  syslog(LOG_DEBUG, "Execute Pipeline: Current Command '%s' with %d parameters", current_command->tokens[0], current_command->num_tokens - 1);
  
  //If this is the last command in the pieline, we do not need to fork, just use the current process
  if (command_num == command_in->num_commands) {
    syslog(LOG_DEBUG, "Execute Pipeline: Found final command of pipeline");
    //Redirect input to the specified value
    redirect_descriptor(descriptor_in, STDIN_FILENO);
    //Execute the command
    execvp(current_command->tokens[0], current_command->tokens);
    //If there is an error executing plug the pipeline
    syslog(LOG_ERR, "Command Not Found '%s'", current_command->tokens[0]);
    //terminate_execution();
    return 2;
  }
  //This is the pipe that the command will output on
  int fd[2];
  //Attempt to create the output pipe
  if (pipe(fd) == -1) {
    //If tehre is an error creating the pipe plug the pipeline
    terminate_execution();
  }
  switch(fork()) {
    //Check if we fail to fork
    case -1:
      //If we fail to fork ctop execution
      syslog(LOG_ERR, "Execute Pipeline: Error forking");
      terminate_execution();
    //Check if the for was successful and this is the child process
    case 0:
      //Close the read end of the output pipe
      close(fd[0]);
      //Set the input for this process to the input pipe
      redirect_descriptor(descriptor_in, STDIN_FILENO);
      //Set the output for this process to be the output pipe
      redirect_descriptor(fd[1], STDOUT_FILENO);
      //Execute the command at this node in the pipeline
      execvp(current_command->tokens[0], current_command->tokens);
      //If there is an error executing plug the pipeline
      syslog(LOG_ERR, "Command Not Found '%s'", current_command->tokens[0]);
      //terminate_execution();
      return 2;
    //Check if fork was successful and this is the parent
    default:
      //Wait for the previous child to finish before executing
      wait(NULL);
      //Close the write end of the output pipe
      close(fd[1]);
      //Close the input file descriptor
      close(descriptor_in);
      //Continue executing the pipeline
      return execute_pipeline(command_in, command_num + 1, fd[0]);
  }
}

//Terminates execution if this process is not the parent
void terminate_execution() {
  syslog(LOG_DEBUG, "Terminate Execution: Attempting to terminate pipeline execution");
  exit(1);
}