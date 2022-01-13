#include <ucontext.h>
#include <string.h>
#include <stdio.h>

#include <sut.h>
#include <queue.h>
#include <helpers.h>
#include <malloc.h>
#include <unistd.h>
#include <semaphore.h>

#define FUNCTION_SIZE 1024 * 64

void printError(char *string) {
    fprintf(stderr, "\033[0;31m");
    fprintf(stderr, "%s", string);
    fprintf(stderr, "\033[0m");
}


void execExecutor() {
    // getting the position in current context array
    int position;
    if(pthread_self() == exec_thread1_id) position = 0;
    else position = 1;

    int counter = 0;
    // constantly look for tasks
    while (true) {
        sem_wait(&execSem);
        struct queue_entry *newTask = queue_pop_head(execQueue);
        if(shutdown_app && !newTask && !queue_peek_front(ioQueue) && counter > 5 && !currentRunningContexts[0] && !currentRunningContexts[1]){
            sem_post(&execSem);
            break;
        }
        sem_post(&execSem);
        // if there is still some tasks in the queue
        if (newTask) {
            currentRunningContexts[position] = newTask->context;
            free(newTask);
            // executing next task
            swapcontext(&execThreadContext[position], currentRunningContexts[position]);

            // removing task from queue and doing some cleanup
            sem_wait(&execSem);
            bool presentExec = context_in_queue(execQueue, currentRunningContexts[position]);
            bool presentIo = context_in_queue(ioQueue, currentRunningContexts[position]);

            if(!presentExec && !presentIo){
                free(currentRunningContexts[position]->uc_stack.ss_sp);
                free(currentRunningContexts[position]);
            }
            currentRunningContexts[position] = NULL;
            sem_post(&execSem);

            counter = 0;

        } else {
            counter++;
            usleep(100*1000);
        }
    }
}

void ioExecutor() {
    int counter = 0;
    // constantly look for tasks
    while (true) {
        sem_wait(&execSem);
        struct queue_entry *newTask = queue_peek_front(ioQueue);
        if(shutdown_app && !newTask && !queue_peek_front(ioQueue) && counter > 5 && !currentRunningContexts[0] && !currentRunningContexts[1]){
            sem_post(&execSem);
            break;
        }
        sem_post(&execSem);
        // if there is still some tasks in the queue
        if (newTask) {
            // executing the next task
            swapcontext(&ioThreadContext, newTask->context);

            // removing task from queue and doing cleanup
            sem_wait(&execSem);
            free(queue_pop_head(ioQueue));
            sem_post(&execSem);
            counter = 0;

        } else {
            counter++;
            usleep(100*1000);
        }
    }
}

void *execWrapper(){
    int position;
    if(pthread_self() == exec_thread1_id) position = 0;
    else position = 1;

    // creating context with attached function
    if (getcontext(&execThreadContext[position])) return 0;
    execThreadContext[position].uc_stack.ss_sp = malloc(FUNCTION_SIZE);
    execThreadContext[position].uc_stack.ss_size = FUNCTION_SIZE;
    execThreadContext[position].uc_link = 0;
    makecontext(&execThreadContext[position], execExecutor, 0);

    // switching to the executor
    swapcontext(&execContext, &execThreadContext[position]);

    // end of thread cleanup
    free(execThreadContext[position].uc_stack.ss_sp);
    pthread_exit(NULL);
}

void *ioWrapper(){
    // creating context with attached function
    if (getcontext(&ioThreadContext)) return 0;
    ioThreadContext.uc_stack.ss_sp = malloc(FUNCTION_SIZE);
    ioThreadContext.uc_stack.ss_size = FUNCTION_SIZE;
    ioThreadContext.uc_link = 0;
    makecontext(&ioThreadContext, ioExecutor, 0);

    // switching to executor
    swapcontext(&ioContext, &ioThreadContext);

    // end of thread cleanup
    free(ioThreadContext.uc_stack.ss_sp);
    pthread_exit(NULL);
}


void ioInitializer() {
    // adding to the back of io queue
    sem_wait(&execSem);

    int position;
    if(pthread_self() == exec_thread1_id) position = 0;
    else position = 1;

    ucontext_t *context = currentRunningContexts[position];
    queue_insert_tail_data(ioQueue, context);
    sem_post(&execSem);

    // going back to the main exec thread
    swapcontext(context, &execThreadContext[position]);
}

void ioEnd(){
    // adding to the back of exec queue
    sem_wait(&execSem);
    struct queue_entry *entry = queue_peek_front(ioQueue);
    queue_insert_tail_data(execQueue, entry->context);
    sem_post(&execSem);

    // going back to the main io thread
    swapcontext(entry->context, &ioThreadContext);
}

