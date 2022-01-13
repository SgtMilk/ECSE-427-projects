#ifndef THREAD_SUT_H
#define THREAD_SUT_H

#include <stdbool.h>
#include <pthread.h>
#include <ucontext.h>
#include <semaphore.h>

// the queue semaphore
extern sem_t execSem;

// Task queue for the exec executor
extern struct queue_entry **execQueue;
// Task queue for the io executor
extern struct queue_entry **ioQueue;

// Parent context for the exec thread
extern ucontext_t execContext;
// Parent context for the io thread
extern ucontext_t ioContext;

// Parent context for the exec thread
extern ucontext_t execThreadContext[2];
// Parent context for the io thread
extern ucontext_t ioThreadContext;

// the exec thread id
extern pthread_t exec_thread1_id;
// the exec thread id
extern pthread_t exec_thread2_id;
// the io thread id
extern pthread_t io_thread_id;

//Flag to signal the threads to clean-up and shutdown.
extern bool shutdown_app;

// if there are multiple c-exec threads
extern bool multipleThreads;

extern ucontext_t *currentRunningContexts[2];

/**
 * Will initialize threads and queues to start the library.
 */
void sut_init();

/**
 * Creates a task from a function and stores it's context in a queue for later execution.
 * @param fn the function to run in a thread
 * @return if the function is successful (0) or not (1).
 */
bool sut_create(void *fn);

/**
 * Will suspend the execution of the current function and store it's context at the back of the task queue.
 */
void sut_yield();

/**
 * Will completely stop the execution of the current function without putting it back in the back of the task queue.
 */
void sut_exit();

/**
 * Will open a file with the IO executor.
 * @param fname the name of the file
 * @return the file descriptor of the opened file
 */
int sut_open(char *fname);

/**
 * Will read from a file.
 * @param fd file descriptor of the file
 * @param buf buffer to which the reading will be mapped to
 * @param size size of the buffer
 * @return the text read from the file
 */
char *sut_read(int fd, char *buf, int size);

/**
 * Will write onto a file.
 * @param fd file descriptor of the file
 * @param buf buffer with the text to write to the file
 * @param size size of the buffer
 */
void sut_write(int fd, char *buf, int size);

/**
 * Will close an opened file.
 * @param fd file descriptor of the file
 */
void sut_close(int fd);

/**
 * Will clean-up and shutdown the library.
 */
void sut_shutdown();

#endif //THREAD_SUT_H