// CS3305_Asn1.c : Defines the entry point for the console application.
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "sys/types.h"

#define MAX_CMD_ARGS 20

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"
int ops, args;

void system(char **argv)
{
	pid_t pid;
	int status;

	if ((pid = fork()) < 0) { //fork a child process
		printf("Error: forking child failed\n";
		exit(1);
	}
	else if (pid == 0) {
		if (execvp(*argv, argv) < 0) {
			printf("Error: execution failed \n");
			exit(1)
		}
	}
	else {
		while (wait(&status) != pid); //wait while child finishes: 
		/*
		This will effectively wait on EACH fork call.  If your input is too large for the buffer though, then
		your program will hang

		Consider moving the wait command to outside the 'system' calls.
		1. fork() all processes for all necessary commands
		2. then perform while(wait...);
		*/
	}
	//other things to do: support I/O redirection: eg: sort <f1.txt >f2.txt
	//support up to three pipes
	//do not have to support command that has both I/O redirection and pipes
}

// returns false for pipes, true for redirection
// if there is no operators, then numops will be 0
bool findtype(char *line, int* numops, int* numcmds)
{
	numops = 0;
	bool type = strchr(line, '|') == NULL ? true : false;
	numcmds = 0;

	while (*line != '\0' && strchr("|<>", *line) != NULL) {
		numops++;
		line++;
	}
	numcmds = numops + 1;
	return type;
}

void separateCommands(char* line, char** argv)
{
	char* pline = line;
	while (*pline != '\0') {       //while there is still pline left
		while (*pline == '|' || *pline == '>' || *pline == '<')
			*pline++ = '\0';     //replace these with 0, so that operands now signify end of a string
		*argv++ = pline;          // save the argument position
		while (*pline != '\0' && *pline != '|' &&
			*pline != '>' && *pline != '<')
			pline++;             //keep skipping through pline to go through all of it
	}
	*argv = '\0';                 // end of argument list
}

char** tokenizeCmd(char *line)
{
	char** argv = malloc( sizeof(char*) * MAX_CMD_ARG );
	while (*line != '\0') {       //while there is still line 
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     //replace these with 0, so that it's saved as the end of the word
		*argv++ = line;          // save the arg position
		while (*line != '\0' && *line != ' ' &&
			*line != '\t' && *line != '\n')
			line++;             //keep skipping through line to go through all of it
	}
	*argv = '\0';                 // end of argument list
	return argv;
}

void parse(char *line, char **argv[], int num) // 
{
	char* linet[15];
	separateCommands(line, linet);
	int i = 0;
	while (linet[i] != NULL) {
		argv[i] = tokenizeCmd(linet[i]);
	}

}

void main(void)
{
	char **argv; //command line argument
	char line[MAX_LENGTH];
	char *cmd[20]

		while (1) {
			printf("heather> ");
			//main loop: read a line. Parse the command, execute the pipeline.
			// To parse the command, can  write recursive parses or use yavv(1) to generate
			gets(line);
			printf("\n");
			int numops;
			int numcmds;
			bool cmdType = findtype(line, &numops, &numcmds); //Find type of commands (False if pipes, True if redirection)
			parse(line, argv);
			if (strcmp(argv[0], "exit") == 0)
				exit(0);
			else if (strcmp(argv[0], "history") == 0) {
				//TODO: history loop *************************************
				//by default this command will display last 10 commands issued
				//if there are fewer than 10 commands will display all previously issued
				//history command may take a numberical argument eg history 5
				return 0;
			}
			else system(argv);
		}
	return 0;
}
