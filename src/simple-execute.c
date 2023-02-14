#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#define WRITE_END 1     // pipe write end
#define READ_END 0      // pipe read end

/*	shell_execute - Function to execute the command with specified command content and command arguments. 
*	@args	String array with command and its arguments. 
*	@argc	Total number of strings including the command, its arguments and the NULL in the end
*/

	 


int shell_execute(char ** args, int argc){


  int child_pid, wait_return, status;

	/* Execute built-in commands */
	/* exit */
	if(strcmp(args[0], "exit") == 0 ){
		return -1; 
	}
	/* TODO: cd */
	if(strcmp(args[0], "cd")== 0){
		if(chdir(args[1])<0){
			printf("cd: %s: %s ", args[1],strerror(errno));
		}
		return 0;
	}
	
  /* Non-built-in commands. These commands should be executed in a child process so the parent process can continue to invoke other commands */	
	/* One command without pipe */
	if((child_pid = fork()) < 0){
		printf("fork() error \n");
	}
	else if(child_pid == 0 ){
		/* TODO: execute the command and check if the command is correctly executed */
		if(execvp(args[0],args)<0){
			printf("execute error\n");
			exit(-1);
		}
	}else{
		if ((wait_return=wait(&status))<0)
		printf("wait error\n");
		
	}
	
	
	/* TODOs: one pipe and two pipes */





    char input[256];
    char *commands[10];
    int i, j, num_commands, num_args;

    int pipefd[10 - 1][2];
    for (i = 0; i < 10 - 1; i++) {
        if (pipe(pipefd[i]) == -1) {
            printf("Failed to create pipe\n");
            exit(1);
        }
    }

    printf("Enter commands separated by '|': ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline character

    num_commands = 0;
    commands[num_commands++] = strtok(input, "|");
    while ((commands[num_commands] = strtok(NULL, "|")) != NULL && num_commands < 10) {
        num_commands++;
    }

    for (i = 0; i < num_commands; i++) {
        num_args = 0;
        args[i][num_args++] = strtok(commands[i], " ");
        while ((args[i][num_args] = strtok(NULL, " ")) != NULL && num_args < 10) {
            num_args++;
        }
        args[i][num_args] = NULL;
    }

    pid_t pids[num_commands];
    for (i = 0; i < num_commands; i++) {
        if ((pids[i] = fork()) == -1) {
            printf("Failed to fork\n");
            exit(1);
        }

        if (pids[i] == 0) {
            // Child process
            if (i > 0) {
                dup2(pipefd[i - 1][0], STDIN_FILENO);
                close(pipefd[i - 1][0]);
            }
            if (i < num_commands - 1) {
                dup2(pipefd[i][1], STDOUT_FILENO);
                close(pipefd[i][1]);
            }

            execvp(args[i][0], args[i]);
            printf("Failed to execute command\n");
            exit(1);
        }
    }

    for (i = 0; i < 10 - 1; i++) {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    for (i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;







 /* wait for child process to terminate */
	while((wait_return = wait(&status)) > 0);
			
 return 0;

}