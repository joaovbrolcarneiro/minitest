/* ************************************************************************** */
/* main.c                                                                     */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

/*
** konopwd:
**  - If the trimmed input equals "pwd", prints the current directory.
*/
bool konopwd(bool cmd_exist, const char *input)
{
    char *buf;
    const size_t size = 4096;
    char *trimmed;

    buf = malloc(size);
    if (!buf)
        return (cmd_exist);
    trimmed = ft_strtrim(input, " ");
    if (ft_strcmp(trimmed, "pwd") == 0)
    {
        ft_printf(RDK"%s\n", getcwd(buf, size));
        free(buf);
        free(trimmed);
        return (true);
    }
    free(trimmed);
    free(buf);
    return (cmd_exist);
}


/* Updated parser_cmd_no_found function */
void parser_cmd_no_found(t_token *token, char **env)
{
    t_token_type type; // To check original type easily

    while (token)
    {
        type = token->type; // Get current type

        // *** ADD CHECK TO SKIP REDIRECTION TOKENS ***
        if (type != TOKEN_PIPE && type != TOKEN_ASSIGNMENT &&
            type != TOKEN_REDIR_IN && type != TOKEN_REDIR_OUT &&
            type != TOKEN_APPEND && type != TOKEN_HEREDOC)
        {
            // This block now only processes potential commands/words

            // Check if it looks like a command but isn't found
            if (token->rank <= RANK_B && // Check if it was initially ranked as potential command
                !search_list(token->value, env) &&
                // Check against builtins that aren't found via search_list
                ft_strcmp(token->value, "cd") != 0 &&
                ft_strcmp(token->value, "exit") != 0 &&
                ft_strcmp(token->value, "pwd") != 0 &&
                ft_strcmp(token->value, "export") != 0 &&
                ft_strcmp(token->value, "echo") != 0)
            {
                // It looked like a command but wasn't found/builtin
                token->type = TOKEN_WORD; // Downgrade to word
                // Optional: Keep the printf for user feedback, or remove if noisy
                ft_printf(RED "konosubash: %s: command not found\n" RESET, token->value);
            }
            // Removed the 'else' block that incorrectly reassigned type to TOKEN_CMD
            // If it passes the checks above (is found or is a builtin),
            // its type (presumably TOKEN_CMD already) should remain unchanged.
            // If it was already TOKEN_WORD, it also remains unchanged.
        }
        token = token->next;
    }
}

t_token *rm_node_lst(t_token *token, t_token *first)
{
    t_token *prev;

    if (!token || !first)
        return (NULL);
    prev = get_prev_node(token, first);
    if (!prev)
        return (NULL);
    prev->next = token->next;
    return (token);
}

void add_node_lst(t_token *dst_lst, t_token *token)
{
    t_token *tmp;

    tmp = dst_lst->next;
    dst_lst->next = token;
    token->next = tmp;
}

void join_and_split(t_token *priv, t_token *token)
{
    priv->value = ft_strjoin(priv->value, "\x1F");
    priv->value = ft_strjoin(priv->value, token->value);
    priv->args = ft_split(priv->value, '\x1F');
}

t_token *redir_handler_file(t_token *token, t_token *first)
{
    if (token->coretype == REDIR && token->next)
        if (token->next->rank != RANK_S)
        {
            ft_printf(YLW"%s\n", token->value);
            ft_printf(PNK"%s\n", token->next->value);
            token->file = rm_node_lst(token->next, first)->value;
        }
    return (token);
}

t_token *cmd_handler_args(t_token *token, t_token *first)
{
    t_token *redir = NULL;

    while (token && token->rank != RANK_S)
    {
        while (token->next && token->rank != RANK_S)
        {
            if (redir_handler_file(token->next, first)->coretype == REDIR)
                redir = rm_node_lst(token->next, first);
            if (token->next && token->next->rank != RANK_S)
            {
                join_and_split(token, rm_node_lst(token->next, first));
                if (token->next && token->next->coretype == REDIR)
                    continue;
                if (!token->next || token->next->rank == RANK_S)
                    token->value = token->args[0];
            }
            else
            {
                if (token->args)
                    token->value = token->args[0];
                break;
            }
        }
        if (redir)
        {
            add_node_lst(token, redir);
            redir = NULL;
        }
        token = token->next;
    }
    return (first);
}

t_token *handler_args_file(t_token *token, t_token *first)
{
    while (token)
    {
        redir_handler_file(token, first);
        cmd_handler_args(token, first);
        token = token->next;
    }
    return (first);
}

t_token *last_token(t_token *token)
{
    if (token)
        while (token->next)
            token = token->next;
    return (token);
}

void handler_pipes(t_token_vec *token_vec)
{
    if (token_vec)
        if (token_vec->so_far)
            if (token_vec->so_far->type == TOKEN_PIPE)
            {
                last_token(token_vec->first)->next = token_vec->so_far;
            }
}

t_token *remap_lst(t_token *token)
{
    t_token *first;
    t_token_vec *token_vec;

    first = malloc(sizeof(t_token));
    first = NULL;
    while (token)
    {
        handler_pipes(token_vec);
        if (first == NULL)
            first = token_vec->first;
        token = token_vec->so_far;
        if (token)
            if (token_vec->so_far->type == TOKEN_PIPE)
                token = token->next;
    }
    return (first);
}

void handle_ctrl_c(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0); // Clear the readline buffer
    rl_on_new_line();
    rl_redisplay();
}

void is_minishell_exit(char *input)
{
    char *res;

    res = ft_strtrim(input, " ");
    if (ft_strcmp("exit", res) == 0)
    {
        free(res);
        exit(0);
    }
    free(res);
}

bool merge_to_token_exception(t_token *token)
{
    t_token *first;
    t_token *rm_node;
    first = token;
    if (token->merge_next == true)
    {
        rm_node = rm_node_lst(token->next, first);
        token->value = ft_strjoin(token->value, rm_node->value);
    }
    return (true);
}

/*
** input_handler:
ALTEREI UM POUCO
*/
void input_handler(t_shell *shell, char *input)
{
    t_token *token;
    t_node_tree *tree;

    /* Do NOT reinitialize shell.env here!
       Use the persistent shell->env set in main(). */
    token = delegated_by_input_handler(input, shell->env);

    // Apply variable assignments - MUST be done BEFORE typealize
    process_variable_assignments(shell, token);

    // Expand variables in the remaining tokens (excluding assignments)
    expand_token_list_no_assignments(token, shell->env, shell->exit_status);

    // Now proceed with the rest of your token processing
    token = handler_args_file(token, token); // Use original function
    parser_cmd_no_found(token, shell->env); // Modify this to ignore assignments
    print_token_lst(token); // Modify this to handle assignment tokens
    tree = init_yggdrasil(token); // Use original function
    if (tree)
        shell->exit_status = execute_ast(shell, tree);
    //free(input); //tinha colocado mas tava dando double-free
}



t_token *delegated_by_input_handler(char *input, char **env)
{
    t_token *first;
    t_token *token;

    token = split_input(input, 0);
    first = token;
    quote_handler_call_loop(token, env);
    typealize_call_loop(token, env);
    merg_tok_excep_cll_revloop(token);
    return (token);
}

void typealize_call_loop(t_token *token, char **env)
{
    while (token)
    {
        token = typealize(token, env);
        token = token->next;
    }
}

void merg_tok_excep_cll_revloop(t_token *token)
{
    t_token *first;

    first = token;
    while (token->next)
        token = token->next;
    while (token != first)
    {
        token = get_prev_node(token, first);
        merge_to_token_exception(token);
    }
}

void quote_handler_call_loop(t_token *token, char **env)
{
    int is_unclosed;

    while (token)
    {
        is_unclosed = 0;
        token->value = quote_handler(token, env, &is_unclosed);
        if (is_unclosed == 1)
            token->err = 1;
        else
            token->err = 0;
        token = token->next;
    }
}

/*
** readline_loop:
**   The main loop: prints the prompt, reads input, and processes it.
*/
void readline_loop(t_shell *shell)
{
    char *input;

    while (1)
    {
        ft_printf("💥"ULI);
        konopwd(true, "pwd");
        ft_printf(RST);
        input = readline(TITLE);
        if (!input) // Ctrl+D
            exit(0);
        if (*input)
            add_history(input);
        is_minishell_exit(input);
        if (!ft_strlen(input))
        {
            free(input);
            continue;
        }
        input_handler(shell, input);
        free(input);
    }
}

/*
** main:
*/
int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    /* Create one modifiable copy of the initial environment */
    shell.env = ft_strdup_array(envp);
    shell.exit_status = 0;
    shell.saved_stdin = dup(STDIN_FILENO);
    shell.saved_stdout = dup(STDOUT_FILENO);
    shell.heredoc_fd = -1;
    shell.in_heredoc = 0;
    shell.ast_root = NULL;
    signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT (Ctrl+/)
    readline_loop(&shell);
    cleanup_shell(&shell);
    return (0);
}

bool is_executable(const char *path)
{
	if (access(path, F_OK | X_OK) == 0)
		return (true);
	return (false);
}

bool is_regular_file(const char *path)
{
	struct stat path_stat;

	if (stat(path, &path_stat) != 0)
	{
		return (false);
	}
	return (S_ISREG(path_stat.st_mode));
}

bool is_valid_exc(const char *path)
{
	return (is_regular_file(path) && is_executable(path));
}

bool is_builtin(const char *cmd)
{
    if (!cmd)
        return (false);
    // Add all your builtin commands here
    if (ft_strcmp(cmd, "echo") == 0 ||
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "pwd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
       //ft_strcmp(cmd, "unset") == 0 || // Uncomment if you implement unset
       //ft_strcmp(cmd, "env") == 0 ||   // Uncomment if you implement env
        ft_strcmp(cmd, "exit") == 0)
    {
        return (true);
    }
    return (false);
}
