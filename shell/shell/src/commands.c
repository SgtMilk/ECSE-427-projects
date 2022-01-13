#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "commands.h"
#include "helpers.h"

#define bufferSize 10
#define bigBuffer 10000

void cd(char *args){
    if(args){
        if(chdir(args) != 0) error();
    } else error();
}

void pwd(){
    long size = pathconf(".", _PC_PATH_MAX);
    char *buffer = (char *)malloc((size_t)size);
    char *path;

    if(buffer) {
        path = getcwd(buffer, (size_t)size);
        if(path) printf("%s\n", path);
        else error();
    } else error();
    free(buffer);
}

void fg(char *args){
    char *endptr = NULL;
    int index = (int)strtol(args, &endptr, 10);
    waitpid(jobList[index - 1], NULL, 0);
    removeJob(jobList[index - 1]);
}

void jobs(){
    if(jobListLength == 0) printf("%s", "No running jobs\n");
    char jobsString[bigBuffer] = {0};
    for(int i  = 0 ; i < jobListLength ; i++){
        char tempString[bufferSize] = {0};
        sprintf(tempString, "%i", jobList[i]);
        strcat(jobsString, tempString);
        strcat(jobsString, "\n");
    }
    printf("%s", jobsString);
}


