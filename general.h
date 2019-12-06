#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
//for initializing to arguments array
const int MAX_ARG = 256;

//types of commands
enum typeOfCommand {PIPE, REDIRECTION, COMMON};

//parses the commands and determines the type
typeOfCommand parseCommand(int, char**, char**, char**);

//execute command with pipe
void pipeCmd(char**, char**);

//read input from myShell
int readArgs(char**);

// execute command with redirection
void redirectCmd(char**, char**);

//execute command with neither pipe nor redirection
void runCmd(int, char**);


