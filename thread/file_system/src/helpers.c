#include <malloc.h>
#include <string.h>
#include "disk_emu.h"
#include "helpers.h"
#include "linkedList.h"
#include "sfs_api.h"

// default parameters for super block
// random number
#define DEFAULT_MAGIC_NUMBER 0xacbd0005
// random number
#define DEFAULT_ROOT_DIRECTORY 0xacbd0005
// 64kb file system
#define DEFAULT_FS_SIZE 65536 // update in sfs_api.c as well
// size of super block
#define DEFAULT_SUPER_BLOCK_SIZE 1024
// maximum of 64 files in the file system
#define DEFAULT_INODE_BLOCK_SIZE 4096
// what's left for data, so about 60kb
#define DEFAULT_DATA_BLOCK_SIZE DEFAULT_FS_SIZE - DEFAULT_SUPER_BLOCK_SIZE - DEFAULT_INODE_BLOCK_SIZE
// size of one inode
#define DEFAULT_INODE_SIZE 64

int initSuperBlock(){
    // initializing the super block (we don't need to initialize the other blocks)
    if(superBlock != NULL) return -1;
    superBlock = malloc(sizeof(StoredSuperObject));
    // magic # (4 bytes)
    superBlock->magicNumber = DEFAULT_MAGIC_NUMBER;
    // root directory (4 bytes)
    superBlock->rootDirectory = DEFAULT_ROOT_DIRECTORY;
    // super block size (4 bytes)
    superBlock->superBlockSize = DEFAULT_SUPER_BLOCK_SIZE;
    // i-node block size (4 bytes)
    superBlock->inodeBlockSize = DEFAULT_INODE_BLOCK_SIZE;
    // i-node length (4 bytes)
    superBlock->inodeBlockLength = 0;
    // data block size (4 bytes)
    superBlock->dataBlockSize = DEFAULT_DATA_BLOCK_SIZE;
    // data block length (4 bytes)
    superBlock->dataBlockLength = 0;
    // inode size
    superBlock->inodeSize = DEFAULT_INODE_SIZE;

    write_blocks(0, sizeof(StoredSuperObject), superBlock);

    return 0;
}

int readSuperBlock(){
    if(superBlock != NULL) return -1;
    superBlock = malloc(sizeof(StoredSuperObject));
    read_blocks(0, sizeof(StoredSuperObject), superBlock);
    return 0;
}

int readInodes(){
    int numInodes = superBlock->inodeBlockLength / superBlock->inodeSize;
    if(numInodes < 0 || numInodes > superBlock->inodeBlockSize / superBlock->inodeSize) return -1;

    StoredNode node = malloc(sizeof(StoredNodeObject));
    for(int i = 0 ; i < numInodes ; i ++){
        read_blocks(superBlock->superBlockSize + i * (int)sizeof(StoredNodeObject), (int)sizeof(StoredNodeObject), node);
        ll_add(i_node_list, node->fp, node->fl, node->name);
    }
    free(node);
    return 0;
}

LLnode createFile(char *name){
    // returning with an error if there is not enough space in our file system
    if(superBlock->inodeBlockLength + superBlock->inodeSize > superBlock->inodeBlockSize || superBlock->dataBlockLength + 1 > superBlock->dataBlockSize) return NULL;

    // adding to the linked list
    LLnode node = ll_add(i_node_list, superBlock->dataBlockLength + superBlock->inodeBlockSize + superBlock->superBlockSize, 1, name);

    // adding to the storage
    StoredNode storedNode = malloc(sizeof(StoredNodeObject));
    storedNode->fp = node->fp;
    storedNode->fl = node->fl;

    int strLen = (int)strlen(node->name);
    if(strLen > 55) strLen = 55;
    for(int i = 0 ; i < strLen ; i++) storedNode->name[i] = node->name[i];
    storedNode->name[strLen] = '\0';

    write_blocks(superBlock->inodeBlockLength + superBlock->superBlockSize, sizeof(StoredNodeObject), storedNode);
    free(storedNode);

    // changing data block by 1 and inode block by 64
    changeBlockLength(1, 0);
    changeBlockLength(superBlock->inodeSize, 1);

    return node;
}

void changeBlockLength(int value, int inode){
    if(inode == 1)superBlock->inodeBlockLength += value;
    else superBlock->dataBlockLength += value;

    write_blocks(0, sizeof(StoredSuperObject), superBlock);
}

char *formatString(char *string){
    if(string == NULL) return NULL;
    int strLen = (int)strlen(string);
    if(strLen > 55) return NULL;
    char *newString = malloc(sizeof(char) * 56);
    for(int i = 0 ; i < 56 ; i++) newString[i] = '\0';
    strncpy(newString, string, 55);
    //newString[strLen + 1] = '\0';
    for(int i = strLen ; i < 56; i++) newString[i] = '\0';
    return newString;
}