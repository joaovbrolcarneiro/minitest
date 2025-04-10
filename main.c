/* ************************************************************************** */
/* main.c                                                                     */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

int g_exit_code = 0;

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

        // Only process tokens that are potential commands initially
        if (type != TOKEN_PIPE && type != TOKEN_ASSIGNMENT &&
            type != TOKEN_REDIR_IN && type != TOKEN_REDIR_OUT &&
            type != TOKEN_APPEND && type != TOKEN_HEREDOC)
        {
            // Check if it was ranked like a command (B or lower) AND
            // it's not found in PATH AND
            // it's NOT one of the known builtins.
            if (token->rank <= RANK_B &&
                !search_list(token->value, env) &&
                // *** Add ft_strcmp(token->value, "unset") != 0 here ***
                ft_strcmp(token->value, "cd") != 0 &&
                ft_strcmp(token->value, "exit") != 0 &&
                ft_strcmp(token->value, "pwd") != 0 &&
                ft_strcmp(token->value, "export") != 0 &&
                ft_strcmp(token->value, "unset") != 0 && // <<< ADDED THIS CHECK
                ft_strcmp(token->value, "echo") != 0)
                // Add other builtins like "env" here if implemented
            {
                // If all conditions above are true, it's likely an error.
                token->type = TOKEN_WORD; // Downgrade to word
                // Print error message (using ft_putstr_fd to stderr is better)
                ft_putstr_fd(RED "minishell: command not found: ", 2);
                ft_putstr_fd(token->value, 2);
                ft_putstr_fd("\n" RESET, 2);
            }
            // If it IS a known builtin OR found in PATH, its type (TOKEN_CMD) remains.
            // If it was already TOKEN_WORD, it also remains TOKEN_WORD.
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

    token = delegated_by_input_handler(input, shell->env);
    process_variable_assignments(shell, token);

    // --- MODIFIED Expander Call ---
    // Pass only needed args, expander reads g_exit_code for $?
    expand_token_list_no_assignments(token, shell->env);
    // ----------------------------

    token = handler_args_file(token, token);
    parser_cmd_no_found(token, shell->env); // Needs is_builtin check inside
    print_token_lst(token);
    tree = init_yggdrasil(token);
    if (tree)
        execute_ast(shell, tree); // execute_ast now updates g_exit_code
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
        // You might want ft_printf here or your libft equivalent
        printf("💥" /*ULI*/); // Assuming ULI/RST are for readline prompt, maybe remove ft_printf if problematic
        konopwd(true, "pwd"); // This seems like a custom debug, ok
        printf(/*RST*/ "");

        input = readline(TITLE); // Read input

        if (!input) // Handle Ctrl+D (EOF)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO); // Optional: print exit message
            cleanup_shell(shell); // Cleanup before exit
            exit(g_exit_code); // Exit with last status
        }

        if (*input) // If input is not empty string
            add_history(input);

        // --- REMOVED is_minishell_exit(input); ---

        // Handle empty input line after potential history add
        if (!ft_strlen(input))
        {
            free(input);
            continue; // Get new prompt
        }

        // Process the command normally - ft_exit will be called if input is "exit"
        input_handler(shell, input);

        free(input); // Free input line
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

    g_exit_code = 0; // Initialize global status

    // Init shell struct (init_shell modified to remove exit_status)
    init_shell(&shell, envp);

    signal(SIGQUIT, SIG_IGN);
    readline_loop(&shell);
    cleanup_shell(&shell);
    return (g_exit_code); // Return last status
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
        ft_strcmp(cmd, "unset") == 0 || // <<< MAKE SURE THIS IS PRESENT AND UNCOMMENTED
        // ft_strcmp(cmd, "env") == 0 ||   // Add if implemented
        ft_strcmp(cmd, "exit") == 0)
    {
        return (true);
    }
    return (false);
}

int ft_unset(char **args, t_shell *shell)
{
    int i = 1;
    int j, k;
    int exit_code = 0; // Assume success
    size_t name_len;
    bool found_and_removed;

    if (!args[1]) // No arguments given is success
        return (0);

    while (args[i])
    {
        // Validate the identifier using the new helper function
        if (!is_valid_identifier(args[i]))
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            exit_code = 1; // Mark failure
            i++;
            continue; // Process next argument
        }

        // Search and remove from environment
        j = 0;
        found_and_removed = false;
        name_len = ft_strlen(args[i]);
        while (shell->env[j])
        {
            // Check if the current env entry matches the variable name to unset
            if (ft_strncmp(shell->env[j], args[i], name_len) == 0 &&
                shell->env[j][name_len] == '=')
            {
                // Found: free the string and shift the rest of the array down
                free(shell->env[j]);
                k = j;
                // Shift pointers down
                while (shell->env[k + 1]) {
                    shell->env[k] = shell->env[k + 1];
                    k++;
                }
                shell->env[k] = NULL; // Null-terminate the shifted array
                found_and_removed = true;
                // Stay at index j, the next element is now here. Or break if only removing first match. Let's break.
                break;
            }
            j++; // Only increment j if no removal happened at this index
        }
        // If not found, we just continue to the next argument silently
        i++; // Move to next argument in args[]
    }
    return (exit_code);
}

bool is_valid_identifier(const char *identifier)
{
    int i = 0;

    // Check if NULL or empty, or if first character is invalid
    if (!identifier || !is_valid_var_char(identifier[i], i))
        return (false);

    // Check remaining characters
    i++;
    while (identifier[i])
    {
        if (!is_valid_var_char(identifier[i], i)) // Use i > 0 implicitly
            return (false);
        i++;
    }

    // If we passed all checks and the string wasn't just empty initially
    return (true);
}