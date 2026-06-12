#include "../includes/minishell.h"

int	env_name_match(const char *entry, const char *name)
{
	int	i;

	i = 0;
	while (entry[i] && entry[i] != '=' && name[i]
		&& name[i] != '=' && entry[i] == name[i])
		i++;
	if (entry[i] != '=' && entry[i] != '\0')
		return (0);
	if (name[i] != '=' && name[i] != '\0')
		return (0);
	return (1);
}

char	*env_get_value(char **envp, const char *name)
{
	int		i;
	char	*eq;

	if (!envp || !name)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (env_name_match(envp[i], name))
		{
			eq = ft_strchr(envp[i], '=');
			if (!eq)
				return (NULL);
			return (ft_strdup(eq + 1));
		}
		i++;
	}
	return (NULL);
}

void	add_env_variable(char ***envp, const char *var)
{
	int		size;
	char	**new_env;
	int		i;

	size = env_size(*envp);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return ;
	i = 0;
	while (i < size)
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[size] = ft_strdup(var);
	new_env[size + 1] = NULL;
	free(*envp);
	*envp = new_env;
}

static char	*build_entry(const char *name, const char *value)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	joined = ft_strjoin(tmp, value);
	free(tmp);
	return (joined);
}

void	update_env_variable(char ***envp, const char *name, const char *value)
{
	int		i;
	char	*joined;

	joined = build_entry(name, value);
	if (!joined)
		return ;
	i = 0;
	while ((*envp)[i])
	{
		if (env_name_match((*envp)[i], name))
		{
			free((*envp)[i]);
			(*envp)[i] = joined;
			return ;
		}
		i++;
	}
	add_env_variable(envp, joined);
	free(joined);
}
