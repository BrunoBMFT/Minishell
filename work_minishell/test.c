#include "includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	char **args;
	args = malloc(sizeof(char *) * 3);
	args[0] = ft_strdup("cat");
	args[1] = ft_strdup("");
	args[2] = NULL;
	execve("/bin/cat", args, envp);
}