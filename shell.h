#ifndef SHELL_H
#define SHELL_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>

#define SHELL_MAX_INPUT 1024

void get_prompt(char *prompt);
void execute_command(char *input);

#endif
