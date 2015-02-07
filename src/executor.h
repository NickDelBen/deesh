/***********************************************************
 * Contains functions for executing commands.
 *
 * Created By: Nick DelBen
 * Created On: February 1, 2015
 *
 * Last Modified: February 1, 2015
 *   - Created Initially
 **********************************************************/

#ifndef _H_EXECUTOR
#define _H_EXECUTOR

#include <syslog.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command.h"
#include "parsed_command.h"

//Global flag to keep trak of whether process is child or not
int executor_child_flag;

//Runs the specified command in a new process
//Parameters
//  command_in the command the user entered
//Return
//  0 - success
//  1 - error running command
int run_command(char * command_in);

//Executes a command pipeline recursivly
//Can be a single command with no pipes
//Parameters
//  command_in    command to be run
//  command_num   current numbered command (for piping)
//  descriptor_in descriptor to read input from
//Return
//  0 - success
//  1 - final command failed
void execute_pipeline(parsed_command_t * command_in, int command_num, int descriptor_in);

//Terminates execution if the process is a child
void terminate_execution();

#endif