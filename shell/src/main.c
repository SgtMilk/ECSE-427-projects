#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "helpers.h"

#define input_length 128

/**
 * This program will provide a command line interface, with basic commands such as cd, pwd, fg, jobs and exit as well
 * as any other commands integrated in the Linux-based OS running this.
 */
int main() {
    signal(SIGTSTP, ctrlZHandler);
    signal(SIGINT, ctrlCHandler);
    jobList = NULL;
    jobListLength = 0;
    runningPID = -1;
    int parentID = getpid();
    char input[input_length];
    printf("sh > ");
    char * strBuffer = fgets(input, input_length, stdin);
    while(strBuffer && strcmp(strBuffer, "exit\n") != 0){

        // ========== getting the input ========== //
        if(*input == '\n'){
            printf("sh > ");
            continue;
        }

        input[strcspn(input, "\r\n")] = 0;

        if(!strchr(input, '|')){
            // ========== if there is no piping ========== //
            if(strchr(input, '&')){
                while(strchr(input, '&')) input[strcspn(input, "&")] = 0;
                delegateCommand(input, false, 0, NULL);
            }else{
                delegateCommand(input, true, 0, NULL);
            }
        }else{
            // ========== if there is piping ========== //
            int *pipeDescriptor = malloc(2 * sizeof(int));
            pipe(pipeDescriptor);

            char *pipeEnd;

            // first command
            char *wholeCommand = strtok_r(input, "|", &pipeEnd);
            delegateCommand(wholeCommand, false, 1, pipeDescriptor);

            //second command
            wholeCommand = strtok_r(NULL, "", &pipeEnd);
            if(wholeCommand)delegateCommand(wholeCommand, false, 2, pipeDescriptor);

            close(pipeDescriptor[0]);
            close(pipeDescriptor[1]);
            wait(0);
            wait(0);
            free(pipeDescriptor);
        }
        if(getpid() == parentID)printf("sh > ");
        strBuffer = fgets(input, input_length, stdin);
    }

    // ========== terminating all background jobs ========== //
    printf("%s", "exiting");
    for(int i = 0 ; i < jobListLength ; i++){
        kill(jobList[i], SIGKILL);
    }
    free(jobList);
}
