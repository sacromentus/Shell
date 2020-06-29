//	Jerry Sufleta, HW #9

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER 200
#define MAX_FLAGS 100

char CWD[BUFFER+1];
char dPath[BUFFER+1];	//

void error(const char *);
void parse_input(char *, char *[], char *);
void prompt();
void execute_command(char *[]);

int main (int argc, char *argv[]) {
	char input[BUFFER+1];
	char *parsed[MAX_FLAGS+1];	

	prompt();
	strncpy(dPath, CWD, BUFFER);	// set default path
	while(fgets(input, BUFFER, stdin) != NULL) {

		parse_input(input, parsed, " \t\n");	
		
		if(strncasecmp("exit", input, BUFFER) == 0)
			return 0;
		
		else if(strncmp("cd", parsed[0], BUFFER) == 0) {
			if(parsed[1] == NULL)
				chdir(dPath);
			else
				chdir(parsed[1]);			
		}
		else 
			execute_command(parsed);
			
		prompt();
	}

	return 0;
}

void error (const char *msg)
{
  perror (msg);
  exit (1);
}

void parse_input(char *input, char *output[], char *delim) {
	char *value;

	value = strtok(input, delim);
	output[0] = value;

	int i;

	for( i = 1; i < MAX_FLAGS && (value =  strtok(NULL, delim)) != NULL; ++i)
		output[i] = value;
	output[i] = NULL;
}

void prompt() {
	char *result = getcwd(CWD, BUFFER);
	
	if(result == NULL)
		error("failed to get current working directory!\n");

	CWD[BUFFER] = '\0';
	printf("bcshell %s %% ", CWD);
}

void execute_command(char *command[]) {
	int childPid = fork();

	if(childPid == 0) {
		execvp(command[0], command);
		error("child is still alive! kill it before it grows!\n");
	}

	else {
		int status;
		wait(&status);
	}
}
