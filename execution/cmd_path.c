/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 10:45:45 by yabodaya          #+#    #+#             */
/*   Updated: 2026/02/06 19:20:52 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	*join_correct_path(char *cmd, char **paths)
{
	char	*final;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		final = ft_strjoin(temp, cmd);
		free (temp);
		if (!final)
			return (NULL);
		if (!access(final, F_OK))
			return (final);
		free (final);
		i++;
	}
	return (NULL);
}

static char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!(ft_strncmp(envp[i], "PATH=", 5)))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*resolved;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = find_path(envp);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	resolved = join_correct_path(cmd, paths);
	free_split(paths);
	return (resolved);
}
