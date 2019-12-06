#include "general.h"
#include <stdlib.h>


int main() {

  char *argv[MAX_ARG], *lcmd[MAX_ARG], *rcmd[MAX_ARG]; //array for command parsing
  typeOfCommand type;
  int argc; // number of command
  char *username; 
  username= getenv("USER"); //to take user name

  
  while (true) {
   
    cout << username;
	cout << " >>> ";

    argc = readArgs(argv); //read the line and put it argv array
	

    type = parseCommand(argc, argv, lcmd, rcmd); // parse and determine the type of command

    if (type == PIPE)         
      pipeCmd(lcmd, rcmd); // execute the commands with pipes
    else if (type == REDIRECTION) 
      redirectCmd(lcmd, rcmd); // execute the commands with redirection
    else
      runCmd(argc, argv); 
  
     }
    
    for (int i=0; i<argc; i++){
      argv[i] = NULL;
  }

  return 0;
}
