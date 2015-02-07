//Contains functions for executing commands

#include "executor.h"

#define DEFAULT_COMMAND_NUM 1

//By default this process is not a child
executor_child_flag = 0;

//Runs the specified command in a new process
int run_command(parsed_command_t * commands_in) {
  syslog(LOG_DEBUG, "Receiving command with '%d' pipes", commands_in->pipes);
  
  //Attempt to fork for the command execution
  switch(pid = fork()) {
    case -1:
      //Parent process
      syslog(LOG_DEBUG, "The fork failed.");
      return 1;
    case 0:
      //Child process
      exec_pipeline(commands_in, DEFAULT_COMMAND_NUM, STDIN_FILENO);
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
void execute_pipeline(parsed_command_t * command_in, int command_num, int descriptor_in) {
  syslog("Executing command pipeline. Command %d/%d", command_num, command_in->num_commands);

  command_t * current_command;

  //Extract the current command
  current_command = command_in->commands[command_num - 1];
  
  //If this is the last command in the pieline, we do not need to fork, just use the current process
  if (command_num == command_in->num_commands) {
    syslog("Found final command of pipeline");
    //Redirect input to the specified value
    redirect_descriptor(descriptor_in, STDIN_FILENO);
    //Execute the command
    execvp(current_command->tokens[0], current_command->tokens);
    terminate_execution();
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
      syslog(LOG_ERR, "Error forking");
      terminate_execution();
    //Check if the for was successful and this is the child process
    case 0:
      //Mark that this process is a child process
      executor_child_flag = 1;
      //Close the read end of the output pipe
      close(fd[0]);
      //Set the input for this process to the input pipe
      redirect_descriptor(descriptor_in, STDIN_FILENO);
      //Set the output for this process to be the output pipe
      redirect_descriptor(fd[1], STDOUT_FILENO);
      //Execute the command at this node in the pipeline
      execvp(cmds[pos][0], cmds[pos]);
      //If there is an error executing plug the pipeline
      syslog(LOG_ERR, "Error executing command");
      terminate_execution();
    //Check if fork was successful and this is the parent
    default:
      //Close the write end of the output pipe
      close(fd[1]);
      //Close the input file descriptor
      close(in_fd);
      //Continue executing the pipeline
      execute_pipeline(cmds, pos + 1, fd[0]);
  }
}

//Terminates execution if this process is not the parent
void terminate_execution() {
  //If this is the child exit
  if (executor_child_flag) {
    exit(1);
  }
}