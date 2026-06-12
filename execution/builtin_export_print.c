#include "../includes/minishell.h"

static void	print_one_entry(char *entry)
{
	int	j;

	ft_putstr_fd("declare -x ", 1);
	j = 0;
	while (entry[j] && entry[j] != '=')
	{
		write(STDOUT_FILENO, &entry[j], 1);
		j++;
	}
	if (entry[j] == '=')
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(entry + j + 1, 1);
		ft_putstr_fd("\"", 1);
	}
	ft_putstr_fd("\n", 1);
}

static char	**dup_env_ptrs(char **envp, int n)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = envp[i];
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

static void	sort_env_ptrs(char **copy, int n)
{
	int		i;
	char	*tmp;

	i = 0;
	while (i < n - 1)
	{
		if (ft_strcmp(copy[i], copy[i + 1]) > 0)
		{
			tmp = copy[i];
			copy[i] = copy[i + 1];
			copy[i + 1] = tmp;
			if (i > 0)
				i -= 2;
		}
		i++;
	}
}

void	export_print_all(char **envp)
{
	char	**copy;
	int		n;
	int		i;

	n = env_size(envp);
	copy = dup_env_ptrs(envp, n);
	if (!copy)
		return ;
	sort_env_ptrs(copy, n);
	i = 0;
	while (i < n)
	{
		print_one_entry(copy[i]);
		i++;
	}
	free(copy);
}
