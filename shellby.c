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

char **getav(char *line)
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
			free(line);
			perror("./simple_shell");
			exit(EXIT_FAILURE);
		}
		for (ind = 0; !(ac < ind); ind++, line = NULL)
		{
			av[ind] = strtok(line, " ");
			if (av[ind] == NULL)
			{
				if (ind == 0)
				{
					free(av);
					return (NULL);
				}
				break;
			}
		}
	}

	return (av);
}


/**
 * prompt - prompt for input from terminal
 *
 * Return: pointer to arguments from terminal
 */

char **prompt()
{
	char **av, *lineptr = NULL;
	size_t n = 0;
	ssize_t num_chars;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, ":) ", 3);
	num_chars = getline(&lineptr, &n, stdin);
	if (num_chars == -1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		if (lineptr != NULL)
			free(lineptr);
		exit(EXIT_SUCCESS);
	}
	av = getav(lineptr);
	free(lineptr);
	return (av);
}


/**
 * execute - executes a program via the calling process
 *
 * @name: name of process
 * @av: arguments vector
 * @envp: environment variables vector
 *
 * Return: 0 if successful
 */

int execute(char *name, char **av, char **envp)
{
	pid_t child_pid;
	int wstatus;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror(name);
		free(av);
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
		execve(av[0], av, envp);
		perror(av[0]);
	} else
		wait(&wstatus);

	return (0);
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
	char **av;

	if (argc == 1)
	{
		while (1)
		{
			av = prompt(argv);
			if (av)
			{
				execute(argv[0], av, envp);
				free(av);
			}
		}
	}
	else
	{
		av = argv + 1;
		execute(argv[0], av, envp);
	}

	return (0);
}
