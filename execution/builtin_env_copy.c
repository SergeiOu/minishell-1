#include "../includes/minishell.h"

char	**copy_envp(char **envp)
{
	int		i;
	int		size;
	char	**copy;

	size = env_size(envp);
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < size)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			free_array(&copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
