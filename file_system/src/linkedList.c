#include <malloc.h>
#include <string.h>
#include "disk_emu.h"
#include "linkedList.h"
#include "sfs_api.h"

LLnode ll_init(){
    // creating the head node
    LLnode head = malloc(sizeof(LLnode_object));
    head->fl = -1;
    head->position = 0;
    head->next = NULL;
    head->previous = NULL;

    return head;
}


LLnode ll_add(LLnode head, int fp, int fl, char *name){
    // creating the node
    LLnode node = malloc(sizeof(LLnode_object));
    node->fp = fp;
    node->fl = fl;

    int strLen = (int)strlen(name);
    if(strLen > 55) strLen = 55;
    for(int i = 0 ; i < strLen ; i++) node->name[i] = name[i];
    node->name[strLen] = '\0';

    if(head->next) node->position = head->next->position + 1;
    else node->position = 1;

    // neighbor allocation
    node->previous = head;
    node->next = head->next;
    head->next = node;
    if(node->next)node->next->previous = node;

    return node;
}


void ll_remove(LLnode node){
    // restitching the linked list
    node->previous->next = node->next;
    if(node->next != NULL) node->next->previous = node->previous;

    // changing the positions of all next nodes
    LLnode curNode = node;
    StoredNode storedNode = malloc(sizeof(StoredNodeObject));
    while((curNode = curNode->previous)->position != 0) {
        // changing position in node
        curNode->position--;
        curNode->fp -= node->fl;
        // changing position in storage
        storedNode->fp = curNode->fp;
        storedNode->fl = curNode->fl;

        int strLen = (int)strlen(curNode->name);
        if(strLen > 55) strLen = 55;
        for(int i = 0 ; i < strLen ; i++) storedNode->name[i] = curNode->name[i];
        storedNode->name[strLen] = '\0';

        write_blocks(superBlock->superBlockSize + (curNode->position - 1) * superBlock->inodeSize, sizeof(StoredNodeObject), storedNode);
    }
    free(storedNode);

    // freeing memory
    free(node);
}


LLnode ll_query(LLnode head, char *name){
    LLnode cur_node = head;
    while((cur_node = cur_node->next) != NULL && strcmp(cur_node->name, name) != 0);
    return cur_node;
}


void ll_clear(LLnode head){
    LLnode cur_node = head;
    while(cur_node->next != NULL) free((cur_node = cur_node->next)->previous);
    free(cur_node);
}