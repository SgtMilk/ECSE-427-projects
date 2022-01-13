#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include "helpers.h"
#include "commands.h"


void delegateCommand(char *wholeCommand, bool wait, int pipeSignal, int *pipeDescriptor){
    char **argsArray = separateArgs(wholeCommand);

    // checking if output redirection is applicable
    char *file = NULL;
    int counter = -1;
    while(strcmp(argsArray[++counter], "ENDSTRING") != 0) if(strcmp(argsArray[counter], ">") == 0 && argsArray[counter + 1]) file = argsArray[counter + 1];
    argsArray[counter] = NULL;

    // delegating commands
    if(strcmp(argsArray[0], "cd") == 0) cd(argsArray[1]);
    else if(strcmp(argsArray[0], "pwd") == 0) pwd();
    else if(strcmp(argsArray[0], "fg") == 0) fg(argsArray[1]);
    else if(strcmp(argsArray[0], "jobs") == 0) jobs();
    else {
        int pid = fork();
        if(pid == 0){
            if(pipeSignal == 1){
                close(STDOUT_FILENO);
                dup(pipeDescriptor[1]);
                close(pipeDescriptor[0]);
                close(pipeDescriptor[1]);
            } else if(pipeSignal == 2){
                close(STDIN_FILENO);
                dup(pipeDescriptor[0]);
                close(pipeDescriptor[1]);
                close(pipeDescriptor[0]);
            } else if(file){
                argsArray[counter - 1] = NULL;
                argsArray[counter - 2] = NULL;
                int openedFile = open(file, O_WRONLY | O_CREAT, 0777);
                dup2(openedFile, 1);
                close(openedFile);
            }
            execvp(argsArray[0], argsArray);
            free(argsArray);
            exit(EXIT_SUCCESS);
        }else {
            addJob(pid);
            if(wait) {
                waitpid(pid, NULL, 0);
                removeJob(pid);
            } else{
                runningPID = pid;
            }
        }
    }
    free(argsArray);
}

void addJob(int job){
    jobList = realloc(jobList, ++jobListLength * sizeof(int));
    jobList[jobListLength - 1] = job;
}

void removeJob(int job){
    bool condition = false;
    for(int i = 0 ; i < jobListLength ; i++){
        if(jobList[i] == job) condition = true;
        if(condition && i < jobListLength - 1){
            jobList[i] = jobList[i + 1];
        }
    }
    if(condition) jobList = realloc(jobList, --jobListLength * sizeof(int));
}

void printRed(char *string){
    printf("\033[0;31m");
    printf("%s", string);
    printf("\033[0m");
}

void error(){
    printRed("An error occurred\n");
}

char **separateArgs(char* args){
    char **argsList = NULL;
    int counter = 0;
    char *endArg;

    // first arg
    char *arg = strtok_r(args, " ", &endArg);
    // reallocating memory and adding a variable
    while(arg){
        argsList = realloc(argsList, ++counter * sizeof(char *));
        argsList[counter - 1] = arg;
        arg = strtok_r(NULL, " ", &endArg);
    }
    argsList = realloc(argsList, ++counter * sizeof(char *));
    argsList[counter - 1] = "ENDSTRING";
    return argsList;
}

void ctrlCHandler(){
    if(runningPID > 0){
        kill(runningPID, SIGKILL);
    }
}

void ctrlZHandler(){}
