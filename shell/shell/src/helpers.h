#ifndef ASSIGNMENT1_HELPERS_H
#define ASSIGNMENT1_HELPERS_H
#include <stdbool.h>

/**
 * jobList and jobListLength are two variables to create and edit a list of running jobs.
 */
int *jobList;
int jobListLength;

/**
 * The running job to kill with CTRL + C
 */
int runningPID;

/**
 * Given a whole command (command + optional arguments), will execute the right function. Will do some piping if
 * necessary.
 * @param wholeCommand: command + optional arguments in one string.
 * @param wait: boolean telling the parent to wait or not.
 * @param pipeSignal: 0 if no pipe, 1 if first pipe, 2 if second pipe.
 * @param pipeDescriptor: NULL if no piping, otherwise info about the pipe.
 * @return NULL if nothing is to be printed, otherwise the thing to be printed.
 */
void delegateCommand(char *wholeCommand, bool wait, int pipeSignal, int *pipeDescriptor);

/**
 * Will add a job to the end of the jobs list.
 * @param job: the job' id
 */
void addJob(int job);

/**
 * Will remove a job from the job list by id.
 * @param job: the job id to remove from the list.
 */
void removeJob(int job);

/**
 * Wrapper around printf to print in red.
 * @param string: the string to be printed
 */
void printRed(char *string);

/**
 * Prints a generic error
 */
void error();

/**
 * Will separate a string of arguments seperated by spaces into an array of arguments.
 * @param args: string of arguments
 * @return the same arguments but in an array
 */
char **separateArgs(char* args);

void ctrlCHandler();

void ctrlZHandler();

#endif //ASSIGNMENT1_HELPERS_H
