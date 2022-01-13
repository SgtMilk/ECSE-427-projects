#include <malloc.h>
#include <string.h>
#include "sfs_api.h"
#include "disk_emu.h"
#include "linkedList.h"
#include "helpers.h"

#define DISK_NAME "disk"
#define BLOCK_SIZE 1
#define NUM_BLOCKS 65536 // update in helpers.c as well
#define MAX_OPENED_FILES 64

#define RESET   "\033[0m"
#define GREEN   "\033[32m"

LLnode cur_node;

File file_numbers[MAX_OPENED_FILES];

LLnode i_node_list;

StoredSuper superBlock;

int mksfs(int fresh){
    if(i_node_list) ll_clear(i_node_list);
    i_node_list = NULL;
    cur_node = NULL;
    if(superBlock) free(superBlock);
    superBlock = NULL;
    for(int i = 0 ; i < MAX_OPENED_FILES ; i++) {
        if (file_numbers[i]) {
            free(file_numbers[i]->buffer);
            free(file_numbers[i]);
        }
        file_numbers[i] = NULL;
    }

    i_node_list = ll_init();

    if(fresh) {
        if(init_fresh_disk(DISK_NAME, BLOCK_SIZE, NUM_BLOCKS) == -1) return -1;

        // write default super block
        if(initSuperBlock() < 0) return -1;
        printf(GREEN "Fresh file system initiated.\n" RESET);
    }
    else{
        if(init_disk(DISK_NAME, BLOCK_SIZE, NUM_BLOCKS) == -1) return -1;

        // read super block parameters
        if(readSuperBlock() < 0) return -1;

        // transfer all the i-nodes in the linked list
        if(readInodes() < 0) return -1;
        printf(GREEN "File system opened.\n" RESET);
    }
    printf(GREEN "Number of free bytes left on disk: %i\n" RESET, superBlock->dataBlockSize - superBlock->dataBlockLength);
    return 0;
}


int sfs_getnextfilename(char *fname){
    if(cur_node != NULL) {
        if((cur_node = cur_node->previous)->position != 0) {
            int strLen = (int)strlen(cur_node->name);
            if(strLen > 55) strLen = 55;
            for(int i = 0 ; i < strLen ; i++) fname[i] = cur_node->name[i];
            fname[strLen] = '\0';
        }
        else {
            fname = NULL;
            cur_node = NULL;
            return 1;
        }
    }
    else{
        if(i_node_list->next == NULL) return -1;
        cur_node = i_node_list;
        while((cur_node = cur_node->next)->next != NULL);

        int strLen = (int)strlen(cur_node->name);
        if(strLen > 55) strLen = 55;
        for(int i = 0 ; i < strLen ; i++) fname[i] = cur_node->name[i];
        fname[strLen] = '\0';
    }
    return 0;
}


int sfs_getfilesize(char* path){
    int returnValue = ll_query(i_node_list, path)->fl;
    return returnValue;
}


int sfs_fopen(char *name){
    char *nameCopy;
    if((nameCopy = formatString(name)) == NULL) return -1;

    LLnode node = ll_query(i_node_list, nameCopy);

    // checking if the file has already been opened
    for(int i = 0 ; i < MAX_OPENED_FILES ; i++) if(node != NULL && file_numbers[i] != NULL && file_numbers[i]->fp == node->fp) {
        sfs_fclose(i);
        return sfs_fopen(name);
    }

    // if the file doesn't exit, create it in the linked list and storage
    if(node == NULL && (node = createFile(nameCopy)) == NULL) return -1;

    // finding a spot for opening our file
    int file_number = -1;
    for(int i = 0 ; i < MAX_OPENED_FILES ; i++){
        if(file_numbers[i] == NULL){
            file_number = i;
            break;
        }
    }
    if(file_number != -1){
        File file = malloc(sizeof(FileObject));
        file->fp = node->fp;
        file->length = node->fl;
        file->pointer = 0;
        file->buffer = malloc(node->fl * sizeof(char));
        read_blocks(node->fp, node->fl, file->buffer);
        file_numbers[file_number] = file;
    }

    free(nameCopy);
    return file_number;
}


int sfs_fclose(int fileID){
    // if the file isn't open anymore
    if(file_numbers[fileID] == NULL) return -1;

    // freeing memory and the spot in the opening files array
    free(file_numbers[fileID]->buffer);
    free(file_numbers[fileID]);
    file_numbers[fileID] = NULL;
    return 0;
}


int sfs_fwrite(int fileID, char *buf, int length){
    File file = file_numbers[fileID];
    if(file == NULL) return -1;

    // copy of length
    int lengthCopy = length;

    if(file->length - file->pointer >= length){

        // if we are not going above what we currently have in storage
        for(int i = 0 ; i < length ; i++) file->buffer[i + file->pointer] = buf[i];
        write_blocks(file->fp + file->pointer, length, buf);

    }else{  // if we are busting our storage space

        // the amount of storage we need more
        int offset = length - (file->length - file->pointer);

        // if we would be busting the total amount of storage
        if(file->fp + file->pointer + length > NUM_BLOCKS || superBlock->dataBlockLength + offset > superBlock->dataBlockSize) return -1;

        // write buffer
        file->buffer = realloc(file->buffer, (file->length + offset) * sizeof(char));
        for(int i = 0 ; i < length ; i++) file->buffer[i + file->pointer] = buf[i];

        // write storage
        int bufferSize = superBlock->dataBlockLength + superBlock->inodeBlockSize + superBlock->superBlockSize - (file->fp + file->length);
        if(bufferSize == 0){
            write_blocks(file->fp + file->pointer, length, (void *)buf);
        }else{
            char *readBuffer = malloc(bufferSize * sizeof(char));
            read_blocks(file->fp + file->length, bufferSize, readBuffer);
            bufferSize += length;
            char *tempBuffer = malloc(bufferSize * sizeof(char));
            for(int i = 0 ; i < bufferSize ; i++){
                if(i < length) tempBuffer[i] = buf[i];
                else tempBuffer[i] = readBuffer[i - length];
            }
            write_blocks(file->fp + file->pointer, bufferSize, tempBuffer);
        }
        // changing inode length in opened buffer, linked list, and storage
        // buffer
        file->length += offset;
        // linked list
        LLnode curNode = i_node_list;
        while((curNode = curNode->next) != NULL && curNode->fp != file->fp);
        if(curNode == NULL) return -1;
        curNode->fl += offset;
        // storage
        StoredNode storedNode = malloc(sizeof(StoredNodeObject));
        storedNode->fp = curNode->fp;
        storedNode->fl = curNode->fl;

        int strLen = (int)strlen(curNode->name);
        if(strLen > 55) strLen = 55;
        for(int i = 0 ; i < strLen ; i++) storedNode->name[i] = curNode->name[i];
        storedNode->name[strLen] = '\0';

        write_blocks(superBlock->superBlockSize + (curNode->position - 1) * superBlock->inodeSize, sizeof(StoredNodeObject), storedNode);

        // changing file pointers on opened buffer, inodes and storage
        // inodes and storage
        while((curNode = curNode->previous)->position != 0){
            curNode->fp += offset;
            storedNode->fp = curNode->fp;
            storedNode->fl = curNode->fl;

            strLen = (int)strlen(curNode->name);
            if(strLen > 55) strLen = 55;
            for(int i = 0 ; i < strLen ; i++) storedNode->name[i] = curNode->name[i];
            storedNode->name[strLen] = '\0';

            write_blocks(superBlock->superBlockSize + (curNode->position - 1) * superBlock->inodeSize, sizeof(StoredNodeObject), storedNode);
        }
        // opened files
        for(int i = 0 ; i < MAX_OPENED_FILES ; i++){
            if(file_numbers[i] != NULL && i != fileID && file_numbers[i]->fp > file->fp - offset)  file_numbers[i]->fp += offset;
        }
        free(storedNode);

        // changing data block offset in memory and storage
        changeBlockLength(offset, 0);
    }
    file->pointer += length;
    return lengthCopy;
}


int sfs_fread(int fileID, char *buf, int length){
    File file = file_numbers[fileID];
    if(file == NULL) return -1;

    if(file->fp + file->pointer + length > NUM_BLOCKS) return -1;

    int fileLength = file->length - file->pointer;
    for(int i = 0 ; i < length ; i++) {
        if(i < fileLength)buf[i] = file->buffer[i + file->pointer];
    }
    file->pointer += length;

    if(fileLength < length) return fileLength;
    return length;
}


int sfs_fseek(int fileID, int loc){
    File file = file_numbers[fileID];
    if(file == NULL) return -1;

    if(loc > file->length) return -1;
    else file->pointer = loc;
    return 0;
}


int sfs_remove(char *file){
    // get the node
    LLnode node = ll_query(i_node_list, file);
    if(!node) return -1;

    if(cur_node && cur_node->fp == node->fp) cur_node = NULL;

    // remove the node from storage
    int numInodes = (superBlock->inodeBlockLength - node->position * superBlock->inodeSize) / superBlock->inodeSize;
    if(numInodes < 0 || numInodes > superBlock->inodeBlockSize / superBlock->inodeSize) return -1;
    if(numInodes > 0){
        StoredNodeObject *storedNodes = malloc(numInodes * sizeof(StoredNodeObject));
        read_blocks(node->position * superBlock->inodeSize + superBlock->superBlockSize, (int)(numInodes * sizeof(StoredNodeObject)), storedNodes);
        write_blocks((node->position - 1) * superBlock->inodeSize + superBlock->superBlockSize, (int)(numInodes * sizeof(StoredNodeObject)), storedNodes);
        free(storedNodes);
    }
    changeBlockLength(-1 * (superBlock->inodeSize), 1);

    // remove the file from storage
    int bufferLength = superBlock->dataBlockLength + superBlock->inodeBlockSize + superBlock->superBlockSize - (node->fp + node->fl);
    if(bufferLength > 0){
        char *buffer = malloc(bufferLength * sizeof(char));
        read_blocks(node->fp + node->fl, bufferLength, buffer);
        write_blocks(node->fp, bufferLength, buffer);
        free(buffer);
    }
    changeBlockLength(-1 * (node->fl), 0);

    // looping through the opened files to see if it's there
    for(int i = 0 ; i < MAX_OPENED_FILES ; i++) {
        if (file_numbers[i] != NULL && file_numbers[i]->fp == node->fp)sfs_fclose(i);
        else if(file_numbers[i] != NULL && file_numbers[i]->fp > node->fp) file_numbers[i]->fp -= node->fl;
    }

    // remove node from linked list
    ll_remove(node);
    return 0;
}
