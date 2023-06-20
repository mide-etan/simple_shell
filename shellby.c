/*
 * File: main.c
 * Auth: Olamide Aworetan
 *       Barnabas Tserga
 */

#include "shell.h"

/**
 * getav - get av from input string
 * @line: string gotten from command line
 *
 * Return: NULL terminated array of strings
 */

char **getav(char line[])
{
	char **av = NULL, cur;
	unsigned int ind = 0, ac = 1;

	for (cur = line[ind]; cur != '\0'; ind++, cur = line[ind])
	{
		if (cur == ' ')
			ac++;
	}

	line = strtok(line, "\n");
	if (line)
	{
		av = malloc(sizeof(char *) * (ac + 1));
		if (av == NULL)
		{
			free(av);
			perror("./simple_shell");
			exit(EXIT_FAILURE);
		}
		for (ind = 0; !(ac < ind); ind++, line = NULL)
			av[ind] = strtok(line, " ");
	}

	return (av);
}


/**
 * main - entry point for shellby
 *
 * @argc: number of args
 * @argv: arguments vector
 * @envp: environment variables vector
 *
 * Return: Int
 */

int main(int argc, char **argv, char **envp)
{
	char **av, *path = NULL;
	size_t n = 0;
	ssize_t num_chars;
	int status;
	pid_t cpid;

	(void) argc;
	while (1)
	{
		write(STDOUT_FILENO, ":) ", 3);
		num_chars = getline(&path, &n, stdin);
		if (num_chars == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
			exit(EXIT_SUCCESS);
		}
		av = getav(path);
		if (av)
		{
			cpid = fork();
			if (cpid == -1)
			{
				perror(argv[0]);
				exit(EXIT_FAILURE);
			}
			if (cpid == 0)
			{
				execve(av[0], av, envp);
				perror(av[0]);
			} else
			{
				wait(&status);
				free(path);
			}
		}
	}
	return (0);
}