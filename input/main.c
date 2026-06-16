#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	process_input(char *input, char ***envp, int *exit_status)
{
	t_token	*tokens;
	t_cmd	*commands;

	tokens = tokenize(input);
	if (!validate_syntax(input, tokens))
	{
		*exit_status = 2;
		free_tokens(tokens);
		return ;
	}
	expand_tokens(tokens, *envp, *exit_status);
	merge_joined_tokens(&tokens);
	commands = build_commands(tokens);
	if (commands && commands->args
		&& (commands->args[0] || commands->redirs || commands->next))
		execute(commands, envp, exit_status);
	free_commands(commands);
	free_tokens(tokens);
}

static int	handle_readline(char ***envp, int *exit_status)
{
	char	*input;

	g_signal = 0;
	set_signal_prompt();
	input = readline("minishell> ");
	if (g_signal == SIGINT)
	{
		*exit_status = 130;
		g_signal = 0;
	}
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (-1);
	}
	if (*input)
		add_history(input);
	process_input(input, envp, exit_status);
	free(input);
	return (0);
}

static int	run_loop(char ***envp)
{
	int	exit_status;

	exit_status = 0;
	while (1)
	{
		if (handle_readline(envp, &exit_status) == -1)
			break ;
	}
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;
	int		status;

	(void)argc;
	(void)argv;
	my_env = copy_envp(envp);
	if (!my_env)
		return (1);
	status = run_loop(&my_env);
	free_array(&my_env);
	rl_clear_history();
	return (status);
}
