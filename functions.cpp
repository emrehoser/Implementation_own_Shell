#include "general.h"
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

vector<string> commands; // to keep commands

int readArgs(char **argv) {
  char *cstr;
  
  string arg;
  int argc = 0;

  


 
  while (cin >> arg) {
	arg.erase(std::remove(arg.begin(),arg.end(),'\"'), arg.end()); // erase the double quotes
	
	commands.push_back(arg); // keeps previous argument for footprint
        

   
    	if (arg == "exit") {   // exit for program
      		exit(0);
    	
	}
	
    
	if(arg == "listdir"){               // compares for myShell commands with terminal commands to convert it
	   	arg = "ls";
	   }else if(arg == "currentpath"){
	   	arg = "pwd";
           }else if(arg == "printfile"){
	   	arg = "cat";
           }else if(arg == "footprint"){
	   	for(int i = 0;i<commands.size();i++){
			cout << commands[i] << "\n";
					
		}
		break;
           }
   
    cstr = new char[arg.size()+1];  
	
    strcpy(cstr, arg.c_str()); //convert string to char array
	
    argv[argc] = cstr;

    argc++;

    if (cin.get() == '\n')
      break;
  }

  argv[argc] = NULL;

  return argc;
}

typeOfCommand parseCommand(int argc, char** argv, char** lcmd, char** rcmd) {
  typeOfCommand result = COMMON;  // default for commands type

  int split = -1;
	
	
  
  for (int i=0; i<argc; i++) {
  
    if (strcmp(argv[i], "|") == 0) {  // check if there is  pipe or not and change type to pipe
      result = PIPE;
      split = i;

    
    } else if (strcmp(argv[i], ">") == 0) { // check if there is  redirect or not and change to redirect
      result = REDIRECTION;
      split = i;
    }
  }

  
  if (result != COMMON) {       // parse the command into two part 
    
    for (int i=0; i<split; i++)
      lcmd[i] = argv[i];		

   
    int count = 0;
    for (int i=split+1; i<argc; i++) {
      
      rcmd[count] = argv[i];
      count++;
    }

    
    lcmd[split] = NULL;    // make sure execlp work well
    rcmd[count] = NULL;
  }

  return result;
}


void pipeCmd(char** lcmd, char** rcmd){
           int fds[2];
           pipe(fds);
	   pid_t pid1, pid2;

           
           if (pid1 = fork() == 0) { //child 1
                dup2(fds[1], 1);
		close(fds[0]);
		close(fds[1]);
                execvp(lcmd[0], lcmd); //execute second command
                perror("execvp failed");

                
                } else if ((pid2 = fork()) == 0) { //child 2
                  dup2(fds[0], 0);

                  close(fds[1]);
                  execvp(rcmd[0], rcmd); // execute first command
                  perror("execvp failed");


            } else {  // parent process
    	close(fds[1]);

    	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
    	
		}
}
 

void redirectCmd(char** cmd, char** file) {
  int fds[2]; 
  int count;  
  int fd;     
  char c;     
  pid_t pid;  
  pipe(fds);

 
  if (fork() == 0) {  // first child
   
    fd = open(file[0], O_RDWR | O_CREAT, 0666);

    
    if (fd < 0) {
      printf("Error: %s\n", strerror(errno));
      return;
    }

    dup2(fds[0], 0);

    
    close(fds[1]);   // execute second command

    
    while ((count = read(0, &c, 1)) > 0)
      write(fd, &c, 1); 

    execlp("echo", "echo", NULL);

 
  } else if ((pid = fork()) == 0) { //second child
    dup2(fds[1], 1);

    close(fds[0]);

    
    execvp(cmd[0], cmd);  // execute first child
    perror("execvp failed");


  } else {
    waitpid(pid, NULL, 0);     //parent process
    close(fds[0]);
    close(fds[1]);
  }
}


void runCmd(int argc, char** argv) {
  pid_t pid;

  pid = fork();


  if (pid < 0){   // error
    perror("Error (pid < 0)");
	exit(1);
}

  else if (pid == 0) { // child process

    execvp(argv[0], argv);
    perror("execvp error");
	exit(1);

  } else{
    waitpid(pid, NULL, 0);  // parent process
}
}





