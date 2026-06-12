/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:54:48 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	unset_process_arg(char ***envp, const char *name)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while ((*envp)[i])
	{
		if (env_name_match((*envp)[i], name))
		{
			free((*envp)[i]);
			i++;
			continue ;
		}
		(*envp)[k++] = (*envp)[i++];
	}
	(*envp)[k] = NULL;
}

static void	export_split_assign(const char *arg, char **name, char **value)
{
	char	*eq;
	int		nlen;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		*name = ft_strdup(arg);
		*value = NULL;
		return ;
	}
	nlen = eq - arg;
	*name = ft_substr(arg, 0, nlen);
	*value = ft_strdup(eq + 1);
}

static int	env_has_name(char **envp, const char *name)
{
	int	i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
	{
		if (env_name_match(envp[i], name))
			return (1);
		i++;
	}
	return (0);
}

void	export_handle_arg(char ***envp, const char *arg, int *exit_status)
{
	char	*name;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		*exit_status = 1;
		return ;
	}
	export_split_assign(arg, &name, &value);
	if (value)
		update_env_variable(envp, name, value);
	else if (!env_has_name(*envp, name))
		add_env_variable(envp, name);
	free(name);
	free(value);
}
