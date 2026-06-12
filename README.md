*This project has been created as part of the 42 curriculum by yabodaya, marvin.*

# minishell

## Description

`minishell` is a minimalist POSIX-style shell, written in C, that reproduces a usable subset of `bash`. It reads command lines interactively, tokenizes them with respect to single- and double-quote rules, expands environment variables, sets up redirections and pipelines, and either executes built-in commands directly or forks and `execve`s an external program found via `PATH`.

The goal of the project is to gain hands-on experience with the foundational Unix programming concepts that a shell relies on: process creation (`fork`, `execve`, `waitpid`), file descriptor management (`dup2`, `pipe`, `open`), signal handling (`sigaction`), and parsing/expanding a small but precise language.

The mandatory feature set, implemented here, is:

- A prompt (`minishell> `) and a working history (via `readline`/`add_history`)
- Single quotes `'...'` preserve their contents literally
- Double quotes `"..."` preserve their contents except `$` expansion
- Environment variable expansion (`$VAR`) and exit-status expansion (`$?`)
- The four redirections: `<`, `>`, `>>`, `<<` (heredoc, no history)
- Pipelines with `|` of arbitrary length
- The seven built-ins: `echo` (with `-n`), `cd` (relative/absolute path), `pwd`, `export`, `unset`, `env`, `exit`
- Bash-compatible signal handling: Ctrl-C displays a new prompt on a new line, Ctrl-D exits the shell, Ctrl-\ does nothing at the prompt
- A single global variable (`g_signal`) that only stores the received signal number, as required by the subject
- Bash-compatible exit codes (0, 1, 2 for syntax errors, 126, 127, 130 for SIGINT, 131 for SIGQUIT)

## Instructions

### Building

The project ships with a `Makefile` at the root and a `Makefile` for `libft` under `includes/libft/`. Building requires `cc`, GNU `make`, and the `readline` development headers.

On Debian/Ubuntu:

```bash
sudo apt-get install libreadline-dev
```

Then, from the project root:

```bash
make         # builds libft then minishell
make clean   # removes .o files
make fclean  # removes .o files and the minishell binary
make re      # fclean + make
```

The build uses `-Wall -Wextra -Werror` and links against `-lreadline`.

### Running

```bash
./minishell
```

The shell starts in interactive mode and displays a prompt. To exit, either press Ctrl-D on an empty line or type `exit`.

### Usage examples

```text
minishell> echo hello world
hello world
minishell> echo "user is $USER, pwd is $PWD"
user is yabodaya, pwd is /home/yabodaya
minishell> ls -la | grep '^d' | wc -l
12
minishell> cat << EOF > /tmp/note
> reminder: review pipes
> EOF
minishell> cat < /tmp/note
reminder: review pipes
minishell> export GREETING=hi
minishell> env | grep GREETING
GREETING=hi
minishell> unset GREETING
minishell> echo "[$GREETING]"
[]
minishell> false
minishell> echo $?
1
minishell> exit 42
exit
$ echo $?
42
```

## Project layout

```
minishell/
├── Makefile
├── README.md
├── includes/
│   ├── minishell.h         # central header (types, prototypes, includes)
│   └── libft/              # the 42 libft, used for memory and string utilities
├── input/                  # tokenizer, parser, expander
│   ├── main.c              # main loop, prompt, envp copy lifetime
│   ├── token.c             # token list creation
│   ├── token_merge.c       # adjacency merge (e.g. "ab"cd → abcd)
│   ├── tokenizer_utils.c   # word/quoted-section extraction
│   ├── handling_tokenization.c
│   ├── command.c           # t_cmd lifecycle
│   ├── command_builder.c   # tokens → t_cmd pipeline
│   ├── command_redirs.c    # < > >> << collection
│   ├── command_utils.c     # arg array build
│   ├── syntax_validator.c  # quote/pipe/redir syntax checks
│   ├── syntax_checks.c     # pipe + redir adjacency rules
│   ├── expansion_builder.c # in-place rebuild of expanded strings
│   ├── expansion_utils.c   # should_expand
│   ├── expansion_helpers.c # var name parsing helpers
│   ├── variable_expander.c # $VAR / $? expansion driver
│   ├── utils.c             # whitespace/quote/operator predicates
│   └── ft_utils.c          # const-friendly ft_strcmp
└── execution/              # builtins, pipes, signals, exec
    ├── execute.c           # dispatch + waitpid status decoding
    ├── exec_one_cmd.c      # single-command fork+execve path
    ├── pipe.c              # multi-stage pipeline (t_pipe_ctx)
    ├── builtin.c           # builtin dispatcher + stdio save/restore
    ├── builtin_cmds1.c     # env / exit / unset / export
    ├── builtin_cmds2.c     # pwd / cd
    ├── builtin_echo.c      # echo with -n
    ├── builtin_cmds_utils.c
    ├── builtin_cmd_utils.c # is_numeric / ft_atol / env_size / free_array
    ├── builtin_env_utils.c # env_name_match, get/add/update env entries
    ├── builtin_env_copy.c  # copy_envp
    ├── builtin_export_print.c
    ├── signals.c           # sigaction setups (prompt / exec / child / heredoc)
    ├── redirection.c       # setup_redirections + open helpers
    ├── heredoc.c           # forked heredoc reader
    └── cmd_path.c          # PATH resolution
```

## Technical choices

- **Environment ownership.** Rather than rely on `extern char **environ` (which is not in the allowed function list), `main` is declared as `int main(int argc, char **argv, char **envp)` and we deep-copy the initial environment into memory we own. All modifications by `export`/`unset` operate on this copy; the same copy is passed as the third argument to `execve` so children see the right state.
- **Signal-safe signal handler.** `g_signal` is a `volatile sig_atomic_t` and the handler only ever assigns the signal number to it (plus the `rl_*` redraw calls that `readline` documents as safe). No data structures are reachable from the handler.
- **Heredoc reads in a child process.** Reading the heredoc in the main shell process would force us to install `SIG_DFL` for SIGINT to interrupt `readline`, which would also kill the shell. Forking a dedicated heredoc reader lets that child take `SIG_DFL` cleanly: if the user presses Ctrl-C, only the child dies, and the parent observes `WIFSIGNALED(status) && WTERMSIG(status) == SIGINT`, sets `$? = 130`, and aborts the command.
- **Quote adjacency.** Quoted and unquoted segments that touch without whitespace (e.g. `"ab"cd"ef"`, `prefix"$V"suffix`) need to become a single argument. Each segment is tokenized independently and tagged with a `joined` flag when there is no whitespace before it; after expansion runs (with the correct quote rules per segment), a `merge_joined_tokens` pass concatenates them into one `t_token`.
- **Pipeline context struct.** `pipe_child` originally needed five parameters (cmd, envp, pipefd, in_fd, exit_status). Norm caps at four, so the per-pipeline state is packed into a `t_pipe_ctx` struct (`pipefd[2]`, `in_fd`, `exit_status`, `envp`).

## Resources

Classic references used while building this:

- The GNU Bash Reference Manual — <https://www.gnu.org/software/bash/manual/bash.html>, especially the sections on tokenization, quoting, parameter expansion, redirections, and exit status.
- The POSIX.1 Shell & Utilities specification (chapter "Shell Command Language") — <https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html>.
- The Linux manual pages for `fork(2)`, `execve(2)`, `pipe(2)`, `dup2(2)`, `waitpid(2)`, `sigaction(2)`, `signal(7)`, `readline(3)`, `access(2)`, `stat(2)`.
- Julia Evans, "How does a shell work" — short, conceptual write-up on the fork/exec loop.
- Eduardo Schettino's "Writing a Unix Shell" series — useful for the I/O redirection and pipeline structure.
- The `bash` source code itself, for resolving ambiguous behavior (exit-code conventions, the wording of error messages, when `exit` prints `exit` on stdout vs stderr).

### AI usage

An AI assistant (Anthropic's Claude) was used as a structured-review tool, in the spirit described in the project's "AI Instructions" chapter:

- **Code review and refactor proposals.** After writing each module, we walked the AI through the code and asked it to flag norm violations (function length, parameter count, declaration order, file size), subject violations (banned functions, signal-handler safety, the "one global, signal-number only" rule), and bash-divergence (exit codes, message destinations, quote/expansion edge cases). Findings were reviewed manually and the fixes were written and tested by us.
- **Edge-case enumeration.** We asked the AI to enumerate edge cases for each builtin (`exit` with non-numeric / too-many-args / overflow; `cd` with no args / too many args / no `HOME`; `export` with invalid identifiers; `unset` with `_` and digit-leading names) and for the parser (unclosed quotes, redirection without a target, empty pipes, adjacent quoted segments). These became our manual test list.
- **Documentation drafts.** This README's structure and the layout of the "Technical choices" section were drafted with AI help and then edited by us.

The AI was **not** used to write code that we cannot defend during the peer evaluation. Every function in the repository has been read, tested, and explained by us; the architectural decisions (fork-based heredoc, `joined` flag for adjacency, `t_pipe_ctx` for parameter reduction, owned `envp` copy) are ours and we are responsible for them.
