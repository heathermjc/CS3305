// CS3305_Asn1.c : Defines the entry point for the console application.
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "sys/types.h"
#include "stdbool.h"

#define MAX_CMD_ARG 20
#define MAX_LENGTH 1024

#define PIPES false
#define REDIRECTION true

// returns false for pipes, true for redirection
// if there is no operators, then numops will be 0
bool findtype(char *line, int* numops, int* numcmds)
{
	*numops = 0;
	bool type = (strchr(line, '|') == NULL) ? true : false;
	*numcmds = 0;

	while (*line != '\0') {
		if (strchr("|<>", *line) != NULL) {
			(*numops)++;
		}
		printf("%c\n", *line);
		line++;

	}
	*numcmds = *numops + 1;
	return type;
}

void separateCommands(char* line, char** argv)
{
	char* pline = line;
	while (*pline != '\0') {       //while there is still pline left
		while (*pline == '|' || *pline == '>' || *pline == '<')
			*pline++ = '\0';     //replace these with 0, so that operands now signify end of a string
		*argv++ = pline;          // save the argument position
		printf("s: %s\n", *(argv - 1));
		while (*pline != '\0' && *pline != '|' &&
			*pline != '>' && *pline != '<')
			pline++;             //keep skipping through pline to go through all of it
	}
	*argv = '\0';                 // end of argument list
}

char** tokenizeCmd(char *line)
{
	char** argv = malloc(sizeof(char*) * MAX_CMD_ARG);
	char** argv2 = &argv;
	while (*line != '\0') {       //while there is still line 
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     //replace these with 0, so that it's saved as the end of the word
		*argv++ = line;          // save the arg position
		printf("t: %s\n", *(argv - 1));
		while (*line != '\0' && *line != ' ' &&
			*line != '\t' && *line != '\n')
			line++;             //keep skipping through line to go through all of it
	}
	*argv = '\0';                 // end of argument list
	while (*argv2 != NULL) {
		printf("%s\n", *argv2);
		argv2++;
	}
	return argv;
}

void parse(char *line, char ****argv) // 
{
	char* linet[15];
	separateCommands(line, linet);
	int i = 0;
	while (linet[i] != NULL) {
		*(argv[i]) = tokenizeCmd(linet[i]);
		i++;
	}

}

int main(void)
{
	//char **argv[12]; //command line argument
	char*** argv = malloc(sizeof(char**) * 12);
	char line[MAX_LENGTH];
	char *cmd[20];

	while (1) {
		printf("heather> ");
		//main loop: read a line. Parse the command, execute the pipeline.
		// To parse the command, can  write recursive parses or use yavv(1) to generate
		gets(line);
		printf("\n");
		int numops = 0;
		int numcmds = 0;
		bool cmdType = findtype(line, &numops, &numcmds); //Find type of commands (False if pipes, True if redirection)
		printf("%s\n", line);			//test
		parse(line, &argv);
		printf("numops; %d, numcmds; %d, cmdType; %s\n", numops, numcmds, cmdType == true ? "true" : "false"); //test

		printf("%s\n", argv[0][0]);
		printf("%s\n", **argv);

		for (int i = 0; i < 5; i++) {
			while (*argv[i] != NULL) {
				printf("%s\n", *argv[i]);
				argv[i]++;
			}
		}

		if (strstr(line, "exit") != NULL)
			exit(0);
		else if (strstr(line, "history") != NULL) {
			//TODO: history loop *************************************
			//by default this command will display last 10 commands issued
			//if there are fewer than 10 commands will display all previously issued
			//history command may take a numberical argument eg history 5
			return 0;
		}
		else if (cmdType == PIPES) {
			//pipes
		}
		else if (cmdType == REDIRECTION) {
			//redirection
		}
		else {
			//if it's a regular command, no pipes
		}
	}
	return 0;
}

int pipes(char **args, int pipes) {
	int commands = pipes + 1;
	int i = 0;
	int pipefds[2 * pipes];
	for (i = 0; i < pipes; i++){
		if (pipe(pipefds + i * 2) < 0) {
			perror("Pipe error");
			exit(0);
		}
	}
	int pid;
	int status;
	int j = 0;
	for (i = 0; i < (commands - 1); i++) {
		pid = fork();
		if (pid == 0) { //if it is not the last command
			if (i < commands-2) {
				if (dup2(pipefds[j + 1], 1) < 0) {
					perror("dup2 issue");
					exist(0);
				}
			}

			if (j != 0){ //not first command
				if (dup2(pipefds[j - 2], 0) < 0) {
					perror("dup2");
					exit(0);
				}
			}

			for (i = 0; i < 2 * pipes; i++) {
				close(pipefds[i]);
			}

			if (execvp(**args) < 0) {
				perror("error");
				eixt(0);
			}
			//need something here to exit if there are 0 commands?
		}
		else if (pid < 0){
			perror("pipe error");
			exit(0);
		}
		j += 2;
	}
	for (i = 0; i < 2 * pipes; i++) {
		close(pipefds[i]);
	}
	for (i = 0; i < pipes + 1; i++) {
		wait(&status);
	}
}