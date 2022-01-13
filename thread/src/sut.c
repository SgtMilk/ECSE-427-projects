#include <stdbool.h>
#include <ucontext.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#include <sut.h>
#include <queue.h>
#include <helpers.h>
#include <semaphore.h>
#include <malloc.h>

#define FUNCTION_SIZE 1024 * 64

// the queue semaphore
sem_t execSem;

// Task queue for the exec executor
struct queue_entry **execQueue;
// Task queue for the io executor
struct queue_entry **ioQueue;

// Parent context for the exec thread
ucontext_t execContext;
// Parent context for the io thread
ucontext_t ioContext;

// Parent context for the exec thread
ucontext_t execThreadContext[2];
// Parent context for the io thread
ucontext_t ioThreadContext;

// the exec thread id
pthread_t exec_thread1_id;
// the exec thread id
pthread_t exec_thread2_id;
// the io thread id
pthread_t io_thread_id;

// Flag to signal the threads to clean-up and shutdown.
bool shutdown_app;

// if there are multiple c-exec threads
bool multipleThreads;

ucontext_t *currentRunningContexts[2];

void sut_init() {
    currentRunningContexts[0] = NULL;
    currentRunningContexts[1] = NULL;

    multipleThreads = false;    // GRADER: CHANGE THIS

    // initializing semaphores
    sem_init(&execSem, 0, 1);

    // initializing queues
    execQueue = queue_create();
    ioQueue = queue_create();

    // starting exec thread
    pthread_create(&exec_thread1_id, NULL, execWrapper, NULL);

    // conditionally starting other exec thread
    if(multipleThreads) pthread_create(&exec_thread2_id, NULL, execWrapper, NULL);

    // starting io thread
    pthread_create(&io_thread_id, NULL, ioWrapper, NULL);

    shutdown_app = false;
}


bool sut_create(void *fn) {
    // creating context with attached function
    ucontext_t *context = malloc(sizeof(ucontext_t));
    if (getcontext(context) == -1) return 0;
    context->uc_stack.ss_sp = malloc(FUNCTION_SIZE);
    context->uc_stack.ss_size = FUNCTION_SIZE;
    context->uc_link = 0;
    makecontext(context, fn, 0);

    // adding that context to the queue
    sem_wait(&execSem);
    queue_insert_tail_data(execQueue, context);
    sem_post(&execSem);
    return 1;
}


void sut_yield() {
    sem_wait(&execSem);
    // getting the context
    int position;
    if(pthread_self() == exec_thread1_id) position = 0;
    else position = 1;
    ucontext_t *context = currentRunningContexts[position];

    // adding it to the end of the queue
    queue_insert_tail_data(execQueue, context);

    sem_post(&execSem);

    // going back to the main task thread
    swapcontext(context, &execThreadContext[position]);
}


void sut_exit() {
    int position;
    if(pthread_self() == exec_thread1_id) position = 0;
    else position = 1;
    // going back to the main task thread
    setcontext(&execThreadContext[position]);
}


int sut_open(char *fname) {
    // adding back to the io queue
    ioInitializer();

    // opening the file
    int file = open(fname, O_RDWR, 0777);

    // adding back to the exec queue
    ioEnd();

    // returning the file descriptor value
    return file;
}


char *sut_read(int fd, char *buf, int size) {
    // adding back to the io queue
    ioInitializer();

    // reading the file
    read(fd, buf, size);

    // adding back to the exec queue
    ioEnd();

    return buf;
}


void sut_write(int fd, char *buf, int size) {
    // adding back to the io queue
    ioInitializer();

    // writing to file
    write(fd, buf, size);

    // adding back to the exec queue
    ioEnd();
}


void sut_close(int fd) {
    // adding back to the io queue
    ioInitializer();

    // closing the file
    close(fd);

    // adding back to the exec queue
    ioEnd();
}


void sut_shutdown() {
    // going back to the main task thread with shutdown flag
    shutdown_app = true;
    pthread_join(exec_thread1_id, NULL);
    if(multipleThreads) pthread_join(exec_thread2_id, NULL);
    pthread_join(io_thread_id, NULL);
    sem_destroy(&execSem);

    struct queue_entry *entry;

    while ((entry = queue_pop_head(execQueue)) != NULL){
        free(entry->context->uc_stack.ss_sp);
        free(entry->context);
        free(entry);
    }

    while ((entry = queue_pop_head(ioQueue)) != NULL){
        free(entry->context->uc_stack.ss_sp);
        free(entry->context);
        free(entry);
    }

    free(execQueue);
    free(ioQueue);
}
