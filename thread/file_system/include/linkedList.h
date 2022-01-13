#ifndef SFS_LINKEDLIST_H
#define SFS_LINKEDLIST_H

/**
 * The i-node object
 * In storage, it will take the following structure:
 * - 4 bytes for the file pointer
 * - 4 bytes for the file length
 * - 56 bytes for file name
 * In total, each i-node w.ill be 64 bytes long.
 */
typedef struct node{
    // file pointer
    int fp;
    // file length
    int fl;
    // file name
    char name[56];
    // node position in the linked list
    int position;
    // previous node
    struct node *previous;
    // next node
    struct node *next;
} LLnode_object, *LLnode;

/**
 * Creates a linked list and returns it's head.
 * @return the linked list's head.
 */
LLnode ll_init();

/**
 * Adds a node after the head of the linked list. It assigns the node an id as well as neighbors in the linked list.
 * @param head the head of the linked list
 * @param fp the file pointer the node refers to
 * @param fl the file length
 * @param name the name of the file
 * @return the created node
 */
LLnode ll_add(LLnode head, int fp, int fl, char *name);

/**
 * Will restitch the linked list together without the given node. It frees the memory of that node as well.
 * @param node the node to remove
 */
void ll_remove(LLnode node);

/**
 * Loops through the linked list and returns the node with the same file name. If that node is not found, NULL will be returned.
 * It will only return the latest added node with that name if there are multiple nodes with that name.
 * @param head the head of the linked list
 * @param name the name of the file to remove
 * @return the found node if it exists, NULL otherwise.
 */
LLnode ll_query(LLnode head, char *name);

/**
 * Clears all the memory allocated to the linked list.
 * @param head the head of the linked list
 */
void ll_clear(LLnode head);

#endif //SFS_LINKEDLIST_H
