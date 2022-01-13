#include <stddef.h>
#include <stdlib.h>
#include <sys/ucontext.h>

#include <queue.h>
#include <helpers.h>

#define MAXSIZE 1000

struct queue_entry **queue_create() {
    struct queue_entry **tempQueue = malloc(sizeof(struct queue_entry *) * MAXSIZE);
    return tempQueue;
}

void queue_error() {
    printError("Fatal error in queue operations\n");
    exit(1);
}

struct queue_entry *queue_new_node(ucontext_t *context) {
    struct queue_entry *entry = (struct queue_entry *) malloc(sizeof(struct queue_entry));
    if (!entry) {
        queue_error();
    }
    entry->context = context;
    return entry;
}

struct queue_entry *queue_insert_tail_data(struct queue_entry **queue, ucontext_t *context) {
    struct queue_entry *e = queue_new_node(context);

    int i = 0;
    while(queue[i] != NULL && i < MAXSIZE) i++;
    if(i == MAXSIZE) queue_error();
    else queue[i] = e;

    return e;
}

struct queue_entry *queue_peek_front(struct queue_entry **queue) {
    if(!queue) return NULL;
    return queue[0];
}

struct queue_entry *queue_pop_head(struct queue_entry **queue) {
    struct queue_entry *elem = queue_peek_front(queue);
    if (elem) {
        int i = 1;
        while(queue[i - 1] != NULL && i < MAXSIZE){
            queue[i - 1] = queue[i];
            i++;
        }
    }
    return elem;
}

bool context_in_queue(struct queue_entry **queue, ucontext_t *context){
    bool condition = false;
    int i = 0;
    while(queue[i] != NULL && i < MAXSIZE){
        if(queue[i]->context == context){
            condition = true;
            break;
        }
        i++;
    }
    return condition;
}

