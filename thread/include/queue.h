#ifndef SUT_QUEUE_H
#define SUT_QUEUE_H

#include <sys/queue.h>
#include <ucontext.h>
#include <stdbool.h>

/**
 * Queue node object
 */
struct queue_entry {
    ucontext_t *context;
};

/**
 * Will create a queue.
 * @return the queue
 */
struct queue_entry **queue_create();

/**
 * Prints an error for the queue.
 */
void queue_error();

/**
 * Will create a new queue node.
 * @param context the context stored inside the node
 * @return the new queue node
 */
struct queue_entry *queue_new_node(ucontext_t *context);

/**
 * Will create and insert a queue node at the tail of the queue.
 * @param q the address of the queue
 * @param context the context stored inside of the queue node
 * @return the queue entry created
 */
struct queue_entry *queue_insert_tail_data(struct queue_entry **queue, ucontext_t *context);

/**
 * Will return the queue node at the front of the queue, without removing it from the queue.
 * @param queue the address of the queue
 * @return the queue node at the front of the queue
 */
struct queue_entry *queue_peek_front(struct queue_entry **queue);

/**
 * Will remove the queue node at the front of the queue and return it.
 * @param queue the address of the queue
 * @return the queue node at the front of the queue
 */
struct queue_entry *queue_pop_head(struct queue_entry **queue);

/**
 * Will check if a context is currently in the queue
 * @param queue the address of the queue
 * @param context the context to search for in the queue
 * @return
 */
bool context_in_queue(struct queue_entry **queue, ucontext_t *context);

#endif //SUT_QUEUE_H
