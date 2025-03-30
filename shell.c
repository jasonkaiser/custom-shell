#include "shell.h"

void get_prompt(char *prompt){

    char hostname[HOST_NAME_MAX];
    char *username;
    char cwd[PATH_MAX];	
    
    gethostname(hostname, sizeof(hostname));
    username = getenv("USER");

    if (!username) {
   	struct passwd *pw = getpwuid(getuid());
	username = pw->pw_name;
    }
    
    getcwd(cwd, sizeof(cwd));
    snprintf(prompt, SHELL_MAX_INPUT, 
             "\033[1;35m[OS-P Shell]\033[0m "  
             "\033[1;32m%s@%s\033[0m:"           
             "\033[1;36m%.800s\033[0m"             
             "\033[1;33m$\033[0m ",          
             username, hostname, cwd);

 
          
  }


void execute_command(char *input) {

 char *args[SHELL_MAX_INPUT];
 int i = 0;

 char *token = strtok(input, " ");
 char *outfile = NULL;
 int append = 0;

  while (token != NULL) {
     if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) {
         append = (strcmp(token, ">>") == 0); 
         token = strtok(NULL, " ");            
         if (token == NULL) {
             printf("Syntax error: expected filename after %s\n", append ? ">>" : ">");
             return;
         }
         outfile = token;  
         break;          
     }
     args[i++] = token;   
     token = strtok(NULL, " ");
  }
    args[i] = NULL; 

    if (i == 0) return; 
   
   if (strcmp(args[0], "rm") == 0){

       if (args[1] == NULL) {
         printf("Usage: rm [-f] <filename>\n");
      
       } else if (strcmp(args[1], "-f") == 0 && args[2] != NULL){

	 unlink(args[2]);

       } else {
	 
	    if (remove(args[1]) !=0){
		perror("rm failed");
	    }

	}
	   return; 
	
   }

   if(strcmp(args[0], "cowsay") == 0) {
      
      if(args[1] == NULL){
        printf("Usage cowsay [-r] message\n");
      } else if (strcmp(args[1], "-r") == 0 && args[2] != NULL){
         printf("   < %s >\n", args[2]);
         printf("     \\    (=' _ '=)\n");
         printf("      \\   (o.o   )\n");
         printf("          ( > < )\n");
      } else {

	 printf("   <%s >\n", args[1]);
	 printf("    \\   ^__^\n");
	 printf("     \\   (oo)\\______\n ");
	 printf("        (__)\\       )\\/\\\n");
	 printf("           ||------w |\n");
	 printf("           ||        ||\n");
      }
	
	return;
   }

  if(strcmp(args[0], "clear") == 0 ){
	printf("\033[H\033[J");
	return;
  }

  if(strcmp(args[0], "cat") == 0){

	if (args[1] == NULL) {
		printf("Usage: cat <filename>\n");
	} else {
	   
	  FILE *file = fopen(args[1], "r");
	  if (!file) {
		perror("cat failed");
		return;
          }

	  char line[MAX_INPUT];
	  while (fgets(line, sizeof(line), file)) {
	 	printf("%s", line);
	  }
	 
             fclose(file);
	}
	
	return;
  }

  if (strcmp(args[0], "kill") == 0) {
        if (args[1] == NULL) {
            printf("Usage: kill <PID>\n");
            return;
        }
        pid_t pid = atoi(args[1]);
        if (kill(pid, SIGKILL) == 0) {
            printf("Process %d killed successfully\n", pid);
        } else {
            perror("kill failed");
        }
        return;
   }


  if (fork() == 0) {
 
     if (outfile) {
          
        int fd = open(outfile, append ? (O_WRONLY | O_CREAT | O_APPEND)
                                      : (O_WRONLY | O_CREAT | O_TRUNC),
                                      0644);
      if (fd == -1) {
         perror("Error opening file");
             exit(1);
        }
          dup2(fd, STDOUT_FILENO);  
          close(fd);                
    }

     execvp(args[0], args);  
     perror("execvp failed");
     exit(1);

    } else {
  
      wait(NULL);  

    }  

}


int main(void){

   char input[SHELL_MAX_INPUT];
   char prompt[SHELL_MAX_INPUT];
   
   while (1) {
     get_prompt(prompt);
     printf("%s", prompt);
     
     if (!fgets(input, SHELL_MAX_INPUT, stdin)){
        printf("\n");
        break;

     }
     
     input[strcspn(input, "\n")] = 0;
     if (strcmp(input, "exit") == 0) break;
     
     execute_command(input);
   }
	
   return 0;
}
