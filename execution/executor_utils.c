/* ************************************************************************** */
/* executor_utils.c                                                         */
/* ************************************************************************** */

#include "minishell.h"

/*
** ft_strdup_array: Duplicate an array of strings.
*/
char **ft_strdup_array(char **array)
{
    int     i;
    int     len = 0;
    char    **dup;

    if (!array)
        return (NULL);
    while (array[len])
        len++;
    dup = malloc(sizeof(char *) * (len + 1));
    if (!dup)
        return (NULL);
    i = 0;
    while (i < len)
    {
        dup[i] = ft_strdup(array[i]);
        i++;
    }
    dup[len] = NULL;
    return (dup);
}

/*
** ft_free_strarray: Free an array of strings.
*/
void ft_free_strarray(char **array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

/*
** init_shell: Initialize the shell structure.
*/
void init_shell(t_shell *shell, char **env)
{
    shell->env = ft_strdup_array(env);
    // shell->exit_status = 0; // <<< REMOVED
    shell->saved_stdin = dup(STDIN_FILENO);
    shell->saved_stdout = dup(STDOUT_FILENO);
    if (shell->saved_stdin == -1 || shell->saved_stdout == -1) {
        perror("minishell: init_shell: dup failed");
        if (shell->saved_stdin != -1) close(shell->saved_stdin);
        if (shell->env) ft_free_strarray(shell->env);
        exit(EXIT_FAILURE);
    }
    shell->heredoc_fd = -1;
    shell->in_heredoc = 0;
    shell->ast_root = NULL;
}


/*
** cleanup_shell: Free allocated resources in shell.
*/
void cleanup_shell(t_shell *shell)
{
    if (!shell) return; // Safety check

    // Free environment copy
    if (shell->env) {
        ft_free_strarray(shell->env);
        shell->env = NULL; // Good practice to NULL pointer after free
    }

    // *** Close duplicated standard FDs ***
    // Check if FDs are valid before closing (>= 0)
    if (shell->saved_stdin >= 0) {
        close(shell->saved_stdin);
        shell->saved_stdin = -1; // Mark as closed
    }
    if (shell->saved_stdout >= 0) {
        close(shell->saved_stdout);
        shell->saved_stdout = -1; // Mark as closed
    }
    // ************************************

    // TODO: Free the AST if necessary (depends on parser cleanup)
    // if (shell->ast_root) {
    //     free_ast(shell->ast_root); // Assuming you have a free_ast function
    //     shell->ast_root = NULL;
    // }
}

/*
** ft_cd: Builtin for changing directory.
*/
int ft_cd(char **args, char ***env)
{
    char *prev_dir;
    char *new_dir;
    char *target;

    /* Save the current directory before changing */
    prev_dir = getcwd(NULL, 0);
    if (!prev_dir)
        return (perror("getcwd"), 1);

    if (!args[1] || ft_strcmp(args[1], "~") == 0)
        target = get_env_value(*env, "HOME");
    else if (ft_strcmp(args[1], "-") == 0)
    {
        target = get_env_value(*env, "OLDPWD");
        if (!target || target[0] == '\0')
        {
            ft_printf("cd: OLDPWD not set\n");
            free(prev_dir);
            return (1);
        }
        ft_printf("%s\n", target);
    }
    else
        target = args[1];

    if (!target || chdir(target) != 0)
    {
        perror("cd");
        free(prev_dir);
        return (1);
    }

    /* Update OLDPWD with the saved directory */
    update_env(env, "OLDPWD", prev_dir);
    free(prev_dir);

    new_dir = getcwd(NULL, 0);
    if (!new_dir)
        return (perror("getcwd"), 1);
    update_env(env, "PWD", new_dir);
    free(new_dir);
    return (0);
}

/*
** ft_echo: Builtin for echoing arguments.
*/
int ft_echo(char **args)
{
    int  i = 1;
    bool print_newline = true; // Flag to track if newline should be printed

    // Check for one or more -n options at the beginning
    while (args[i] && ft_strcmp(args[i], "-n") == 0)
    {
        print_newline = false; // Suppress newline if -n is found
        i++;                   // Move to the next argument
    }

    // Print remaining arguments
    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO); // Print argument to stdout
        if (args[i + 1]) // If there's another argument coming
            ft_putstr_fd(" ", STDOUT_FILENO); // Print a space
        i++;
    }

    // Print newline only if the flag is set
    if (print_newline)
        ft_putstr_fd("\n", STDOUT_FILENO);

    return (0); // Echo always returns 0 in bash
}


/*
** ft_pwd: Builtin to print the working directory.
*/
int ft_pwd(void)
{
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_printf("%s\n", cwd);
        return (0);
    }
    else
    {
        perror("pwd");
        return (1);
    }
}

/*
** ft_exit: Builtin to exit the shell.
*/
int ft_exit(char **args, t_shell *shell)
{
    int  exit_val;
    bool is_numeric = true;
    int  i = 0;

    ft_putstr_fd("exit\n", STDERR_FILENO); // Print exit message to stderr

    if (!args[1]) {
        // *** No argument: use global exit code ***
        exit_val = g_exit_code;
    } else {
        // Check if numeric (you can refine this check)
        i = 0;
        if (args[1][i] == '+' || args[1][i] == '-') i++;
        if (args[1][i] == '\0') is_numeric = false;
        while (args[1][i]) {
            if (!ft_isdigit(args[1][i])) { is_numeric = false; break; }
            i++;
        }

        if (!is_numeric) {
             ft_putstr_fd("minishell: exit: ", 2);
             ft_putstr_fd(args[1], 2);
             ft_putstr_fd(": numeric argument required\n", 2);
             exit_val = 2; // Or 255 like bash
             cleanup_shell(shell);
             exit(exit_val); // Must exit on this error
        } else {
             if (args[2]) { // Check for too many arguments
                 ft_putstr_fd("minishell: exit: too many arguments\n", 2);
                 return (1); // Return error 1, DO NOT exit
             }
             // Convert valid numeric argument
             exit_val = ft_atoi(args[1]) % 256;
             if (exit_val < 0) exit_val += 256;
        }
    }
    // If we reach here, perform the exit
    cleanup_shell(shell);
    exit(exit_val);
    // return (0); // Unreachable
}


/*
** get_env_value: Get the value of an environment variable.
*/
char *get_env_value(char **env, const char *name)
{
    int     i;
    size_t  len = ft_strlen(name);

    for (i = 0; env[i]; i++)
    {
        if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
            return (env[i] + len + 1);
    }
    return (NULL);
}

/*
** ft_path_join: Join a directory path and a file name.
*/
char *ft_path_join(char *path, char *file)
{
    char *tmp;
    char *res;

    tmp = ft_strjoin(path, "/");
    res = ft_strjoin(tmp, file);
    free(tmp);
    return (res);
}

/*
** find_command_path: Look for a command in the PATH.
*/
char *find_command_path(char *cmd, char **env)
{
    char    *path_env;
    char    **paths;
    char    *full_path;
    int     i;

    path_env = get_env_value(env, "PATH");
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    i = 0;
    while (paths && paths[i])
    {
        full_path = ft_path_join(paths[i], cmd);
        if (access(full_path, X_OK) == 0)
        {
            ft_free_strarray(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    ft_free_strarray(paths);
    return (NULL);
}

char *ft_strcpy(char *dest, const char *src)
{
    char *tmp = dest;
    while ((*dest++ = *src++))
        ;
    return tmp;
}

char *ft_strcat(char *dest, const char *src)
{
    char *tmp = dest;
    while (*dest)
        dest++;
    while ((*dest++ = *src++))
        ;
    return tmp;
}

/*
** Helper: Update the environment variable if it exists,
** or add it if it doesn’t.
*/
int update_env(char ***env, char *var, char *value)
{
    int i;
    char *new_var;
    char **new_env;
    size_t len = ft_strlen(var) + ft_strlen(value) + 2; // for '=' and '\0'

    new_var = malloc(len);
    if (!new_var)
        return (1);
    ft_strcpy(new_var, var);
    ft_strcat(new_var, "=");
    ft_strcat(new_var, value);

    i = 0;
    while ((*env)[i])
    {
        /* Compare variable name up to '=' */
        if (ft_strncmp((*env)[i], var, ft_strlen(var)) == 0 &&
            (*env)[i][ft_strlen(var)] == '=')
        {
            free((*env)[i]);
            (*env)[i] = new_var;
            return (0);
        }
        i++;
    }
    /* Not found: add new variable */
    new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env)
    {
        free(new_var);
        return (1);
    }
    for (int j = 0; j < i; j++)
        new_env[j] = (*env)[j];
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    free(*env);
    *env = new_env;
    return (0);
}

/*
** ft_export:
**   - Without arguments: prints the environment.
**   - With arguments: expects "VAR=VALUE" pairs. Updates or adds the variable.
*/
char **sort_env(char **env)
{
    int   count = 0;
    int   i, j;
    char **sorted;
    char  *temp;

    while (env[count])
        count++;
    sorted = malloc(sizeof(char *) * (count + 1));
    if (!sorted)
        return (NULL);
    for (i = 0; i < count; i++)
        sorted[i] = ft_strdup(env[i]);
    sorted[count] = NULL;
    /* Bubble sort simples */
    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - i - 1; j++)
        {
            if (ft_strcmp(sorted[j], sorted[j + 1]) > 0)
            {
                temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    return (sorted);
}

/*
** ft_export:
**   - Se chamado sem argumentos, imprime o ambiente (shell->env) ordenado em ordem crescente.
**   - Se houver argumentos no formato VAR=VALUE, atualiza ou adiciona a variável.
*/
int ft_export(char **args, char ***env)
{
    int i;
    int exit_code = 0; // Overall exit code for the command batch
    char *equal;
    char *var_name;

    if (!args[1]) // No arguments: print sorted env
    {
        char **sorted = sort_env(*env);
        if (!sorted)
            return (1); // Malloc error
        i = 0;
        while (sorted[i])
        {
            // Use ft_putstr_fd for consistency? Or keep ft_printf?
            ft_printf("%s\n", sorted[i]);
            free(sorted[i]);
            i++;
        }
        free(sorted);
        return (0); // Success
    }

    // Process arguments
    i = 1;
    while (args[i])
    {
        equal = ft_strchr(args[i], '=');

        if (!equal) // Variable name without '='
        {
            // Bash checks if the identifier itself is valid.
            // If valid, it does nothing (doesn't export). If invalid, error.
            if (!is_valid_identifier(args[i]))
            {
                ft_putstr_fd("minishell: export: `", 2);
                ft_putstr_fd(args[i], 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                exit_code = 1; // Mark error
            }
            // else: valid identifier without '=', do nothing
        }
        else // Variable name with '='
        {
            var_name = args[i];
            *equal = '\0'; // Temporarily terminate string at '=' to check name

            if (!is_valid_identifier(var_name))
            {
                *equal = '='; // Restore '=' before printing error
                ft_putstr_fd("minishell: export: `", 2);
                ft_putstr_fd(args[i], 2); // Print original invalid arg
                ft_putstr_fd("': not a valid identifier\n", 2);
                exit_code = 1; // Mark error
            }
            else
            {
                // Identifier is valid, try to update environment
                if (update_env(env, var_name, equal + 1) != 0) {
                    // update_env failed (likely malloc error)
                    perror("minishell: export"); // update_env might have its own perror
                    exit_code = 1; // Mark error
                }
                *equal = '='; // Restore '=' after processing
            }
        }
        i++; // Move to next argument
    }
    return (exit_code); // Return 0 if all were okay, 1 if any error occurred
}

