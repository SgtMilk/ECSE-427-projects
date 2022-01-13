#ifndef ASSIGNMENT1_COMMANDS_H
#define ASSIGNMENT1_COMMANDS_H

/**
 * cd will change the current working directory when inputted a relative or absolute path.
 * @param args the relative path
 * @returns NULL
 */
void cd(char *args);

/**
 * pwd will return the current working directory.
 * @returns the current working directory
 */
void pwd();

/**
 * fg will bring back in front of the stack a background process.
 * @param args which background process to bring forward
 * @returns NULL
 */
void fg(char *args);

/**
 * jobs will show all current background tasks
 * @returns all the background tasks
 */
void jobs();

#endif //ASSIGNMENT1_COMMANDS_H
