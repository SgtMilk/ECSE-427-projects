#ifndef FS_HELPERS_H
#define FS_HELPERS_H

#include "linkedList.h"


/**
 * The stored node object
 */
typedef struct storedNode{
    // file pointer
    int fp;
    // file length
    int fl;
    // file name
    char name[56];
} StoredNodeObject, *StoredNode;


/**
 * The stored super block
 */
typedef struct storedSuper{
    // magic # (4 bytes)
    int magicNumber;
    // root directory (4 bytes)
    int rootDirectory;
    // super block size (4 bytes)
    int superBlockSize;
    // i-node block size (4 bytes)
    int inodeBlockSize;
    // i-node length (4 bytes)
    int inodeBlockLength;
    // data block size (4 bytes)
    int dataBlockSize;
    // data block length (4 bytes)
    int dataBlockLength;
    // inode size (4 bytes)
    int inodeSize;
}StoredSuperObject, *StoredSuper;


/**
 * An opened file object
 */
typedef struct file{
    char *buffer;
    int pointer;
    int length;
    int fp;
} FileObject, *File;


/**
 * Will generate and write to storage the super block.
 * @return 0 if successful, otherwise -1
 */
int initSuperBlock();


/**
 * Will read the super block from storage.
 * @return 0 if successful, otherwise -1
 */
int readSuperBlock();


/**
 * Will read all the inodes and store them in a linked list assuming that the linked list and the super block were
 * initialized before.
 * @return 0 if successful, otherwise -1
 */
int readInodes();


/**
 * Creates a file and
 * @param name
 * @return
 */
LLnode createFile(char *name);


/**
 *
 * @param value
 * @param inode
 */
void changeBlockLength(int value, int inode);


/**
 *
 * @param string
 * @return
 */
char *formatString(char *string);

#endif //FS_HELPERS_H
