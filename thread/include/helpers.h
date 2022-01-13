#ifndef SUT_HELPERS_H
#define SUT_HELPERS_H

#include <queue.h>

/**
 * Prints an error in red from a string.
 * @param string the string to print
 */
void printError(char *string);

/**
 * Function to run inside of an executor thread. It will constantly look for tasks to run in the exec queue.
 */
void execExecutor();

/**
 * Function to run inside of an executor thread. It will constantly look for tasks to run in the exec queue.
 */
void ioExecutor();

/**
 * Wrapper for the exec thread to have a base context inside.
 */
void *execWrapper();

/**
 * Wrapper for the io thread to have a base context inside.
 */
void *ioWrapper();

/**
 * Helper to place the io task in the io queue and wait for the io executor.
 */
void ioInitializer();

/**
 * Helper to place the io task in the exec queue and wait for the exec executor.
 */
void ioEnd();

#endif //SUT_HELPERS_H
