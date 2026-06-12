#include "../includes/minishell.h"

static int	is_n_option(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 2;
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

static void	echo_print_args(t_cmd *cmd, int start)
{
	int	i;

	i = start;
	while (cmd->args[i])
	{
		write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
}

void	echo_bi(t_cmd *cmd, int *exit_status)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	while (cmd->args[i] && is_n_option(cmd->args[i]))
	{
		newline = 0;
		i++;
	}
	echo_print_args(cmd, i);
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	*exit_status = 0;
}
