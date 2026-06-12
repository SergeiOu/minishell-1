NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I./includes
READLINE    = -lreadline -lhistory
RM          = rm -f

LIBFT_DIR   = includes/libft
LIBFT       = $(LIBFT_DIR)/libft.a

INPUT_DIR   = input
EXEC_DIR    = execution

INPUT_SRCS  = $(INPUT_DIR)/main.c \
              $(INPUT_DIR)/token.c \
              $(INPUT_DIR)/token_merge.c \
              $(INPUT_DIR)/tokenizer_utils.c \
              $(INPUT_DIR)/handling_tokenization.c \
              $(INPUT_DIR)/command.c \
              $(INPUT_DIR)/command_builder.c \
              $(INPUT_DIR)/command_redirs.c \
              $(INPUT_DIR)/command_utils.c \
              $(INPUT_DIR)/syntax_checks.c \
              $(INPUT_DIR)/syntax_validator.c \
              $(INPUT_DIR)/expansion_builder.c \
              $(INPUT_DIR)/expansion_utils.c \
              $(INPUT_DIR)/expansion_helpers.c \
              $(INPUT_DIR)/variable_expander.c \
              $(INPUT_DIR)/word_split.c \
              $(INPUT_DIR)/utils.c \
              $(INPUT_DIR)/ft_utils.c

EXEC_SRCS   = $(EXEC_DIR)/execute.c \
              $(EXEC_DIR)/exec_one_cmd.c \
              $(EXEC_DIR)/pipe.c \
              $(EXEC_DIR)/builtin.c \
              $(EXEC_DIR)/builtin_cmds1.c \
              $(EXEC_DIR)/builtin_cmds2.c \
              $(EXEC_DIR)/builtin_echo.c \
              $(EXEC_DIR)/builtin_cmds_utils.c \
              $(EXEC_DIR)/builtin_cmd_utils.c \
              $(EXEC_DIR)/builtin_env_utils.c \
              $(EXEC_DIR)/builtin_env_copy.c \
              $(EXEC_DIR)/builtin_export_print.c \
              $(EXEC_DIR)/signals.c \
              $(EXEC_DIR)/redirection.c \
              $(EXEC_DIR)/heredoc.c \
              $(EXEC_DIR)/cmd_path.c

SRCS        = $(INPUT_SRCS) $(EXEC_SRCS)
OBJS        = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(READLINE)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
