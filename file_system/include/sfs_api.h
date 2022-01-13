#ifndef FS_SFS_API_H
#define FS_SFS_API_H

#include "linkedList.h"
#include "helpers.h"

// the linked list of inodes
extern LLnode i_node_list;

// super block relevant info
extern StoredSuper superBlock;

/**
 * Creates or mounts the file system.
 * @param fresh if false, will mount the file system, otherwise will create it.
 * @return 0 if success, -1 if failure
 */
int mksfs(int fresh);

/**
 * Used to loop through the directory as an iterator would do. Will change fname to the value of the next filename.
 * @param fname a buffer for the next filename
 * @return 0 if successful, 1 if there are no more files, -1 if there are no files
 */
int sfs_getnextfilename(char *fname);

/**
 * Returns the size of a given file.
 * @param path the path of a file
 * @return 0 if success, -1 if failure
 */
int sfs_getfilesize(char* path);

/**
 * Opens or creates a file depending on if it exists and returns the file descriptor. It will set the read/write pointer
 * to 0.
 * @param name the name of the file
 * @return the file descriptor
 */
int sfs_fopen(char *name);

/**
 * Closes an opened file.
 * @param fileID the file descriptor
 * @return 0 if success, -1 if failure
 */
int sfs_fclose(int fileID);

/**
 * Will write to the file described by the file descriptor.
 * @param fileID the file descriptor
 * @param buf the buffer with what to write
 * @param length the number of bytes to copy from the buffer
 * @return the number of bytes written
 */
int sfs_fwrite(int fileID, char *buf, int length);

/**
 * Will read the file described by the file descriptor.
 * @param fileID the file descriptor.
 * @param buf the buffer to store what is read
 * @param length the number of bytes to read from the file
 * @return the number of bytes written
 */
int sfs_fread(int fileID, char *buf, int length);

/**
 * Will move the read/write pointer to a given location (location in a file).
 * @param fileID the file descriptor
 * @param loc location (offset) in a file
 * @return 0 on success and -1 if failure
 */
int sfs_fseek(int fileID, int loc);

/**
 * Deletes a file from the file system (and from disk).
 * @param file the file descriptor
 * @return 0 on success, -1 if failure
 */
int sfs_remove(char *file);

#endif //FS_SFS_API_H
