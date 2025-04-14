/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* tokenlst.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/03/31 21:06:35 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/14 ??:??:?? by Copilot         ###   ########.fr       */
/* */
/* ************************************************************************** */


#include "minishell.h"        // Adjust includes as necessary
#include "minishell_part2.h"

// size_t ft_strnlen(char *s, char n); // Assume declared elsewhere (libft.h)
// char *get_envar(char **env, char *var); // Assume declared elsewhere


/**
 * @brief Gets the list of directories from the PATH environment variable.
 * Caller must free the returned array and its contents.
 *
 * @param env Environment variables.
 * @return char** Array of path strings, or NULL if PATH not set/empty.
 */
char **get_path_list(char **env)
{
    char *path;
    char **path_list;

    // Assuming get_env_value is the correct function now
    path = get_env_value(env,"PATH"); // Use get_env_value instead of get_envar?
    if (!path)
        return (NULL); // Handle PATH not existing
    path_list = ft_split(path, ':');
    // No need to free 'path' as it points into the env array
    return (path_list);
}

/**
 * @brief Calculates size and allocates memory for the command list based on PATH. (MODIFIED Signature)
 *
 * @param env Environment variables.
 * @return char** Allocated char** for command list, or NULL on error.
 */
char **command_list_malloc(char **env) // MODIFIED: Removed unused 'list' parameter
{
    long long       count = 0; // Initialize count
    DIR             *dir;
    char            **path_list;
    char            **allocated_list;
    struct dirent   *entry; // To read directory entries

    path_list = get_path_list(env);
    if (!path_list)
        return (NULL); // Handle error from get_path_list

    // Count total files in all PATH directories
    int path_idx = 0;
    while (path_list[path_idx])
    {
        dir = opendir(path_list[path_idx]);
        if(dir == NULL) {
            path_idx++; // Skip directories we can't open
            continue;
        }
        while ((entry = readdir(dir)) != NULL) // Assign and check in condition
        {
             // Potentially filter out "." and ".." if not desired
             // if (ft_strcmp(entry->d_name, ".") != 0 && ft_strcmp(entry->d_name, "..") != 0)
             count++;
        }
        closedir(dir);
        path_idx++;
    }
    ft_free_strarray(path_list); // Free the path list obtained from ft_split

    // Allocate memory for the list of command names + NULL terminator
    allocated_list = malloc(sizeof(char *) * (count + 1)) ;
    if (!allocated_list)
        perror("malloc command list"); // Report allocation error

    return(allocated_list);
}

/**
 * @brief Creates a list of potential command names from PATH directories. (MODIFIED Signature)
 * Caller must free the returned array and its contents.
 *
 * @param env Environment variables.
 * @return char** Array of command name strings, or NULL on error.
 */
char **init_command_list(char **env) // MODIFIED: Removed unused 'count' parameter
{
    char            **list;
    char            **path_list;
    DIR             *dir;
    struct dirent   *d;
    long long       current_index = 0; // Use a local index

    path_list = get_path_list(env);
    if (!path_list) return (NULL);

    list = command_list_malloc(env); // Allocate memory first
    if (!list) {
        ft_free_strarray(path_list);
        return (NULL); // Return if allocation failed
    }

    // Fill the allocated list
    int path_idx = 0;
    while (path_list[path_idx])
    {
        dir = opendir(path_list[path_idx]);
        if(dir == NULL) {
             path_idx++;
             continue; // Skip directories we can't open
        }
        d = readdir(dir); // Read first entry
        while (d)
        {
             // Optionally filter "." and ".."
             // if (ft_strcmp(d->d_name, ".") != 0 && ft_strcmp(d->d_name, "..") != 0)
             // {
                 list[current_index] = ft_strdup(d->d_name);
                 if (!list[current_index]) {
                     // Handle strdup failure: free already added strings and the list
                     perror("strdup in init_command_list");
                     ft_free_strarray(list); // Custom free function handles partial list
                     list = NULL; // Mark as NULL to return
                     closedir(dir);
                     goto cleanup; // Use goto for cleanup on inner error
                 }
                 current_index++;
             // }
            d = readdir(dir); // Read next entry
        }
        closedir(dir);
        path_idx++;
    }

cleanup: // Label for cleanup
    ft_free_strarray(path_list); // Free path list regardless of success/failure
    if (list) // If list wasn't NULLed by an error
        list[current_index] = NULL; // Null-terminate the list

    return(list);
}

/**
 * @brief Searches for a command in PATH or checks if it's a valid executable path. (MODIFIED)
 * Removed unused variables 'i' and 'count'.
 *
 * @param search The command name or path to search for.
 * @param env Environment variables.
 * @return true if found/valid executable, false otherwise.
 */
bool search_list(char *search, char **env)
{
    char **list = NULL; // Initialize to NULL
    // int i = 0; // REMOVED - Unused
    // long long count; // REMOVED - Unused and uninitialized

    // Check if 'search' is itself a valid executable path first
    if(is_valid_exc(search))
        return (true);

    // Check for NULL search string or env
    if (!search || !env)
        return (false);

    // Initialize the command list from PATH
    list = init_command_list(env); // MODIFIED call
    if (!list) {
         // Error during command list initialization (e.g., malloc failure)
         // Should probably print an error or handle it
         return (false);
    }


    // Iterate through the generated list (note: modifies 'list' pointer)
    // Use a temporary pointer to iterate if needed elsewhere
    char **current = list;
    while (*current) // Iterate until NULL terminator
    {
        if(ft_strcmp(search, *current) == 0) // Use strcmp for exact match
        {
            ft_free_strarray(list); // Free the list before returning true
            return(true);
        }
        current++; // Move to next command name in the list
    }

    ft_free_strarray(list); // Free the list if command not found
    return(false);
}


/**
 * @brief Returns a unique ID for tokens.
 */
long long get_token_id(void) // Was missing return type in header? Add 'long long'
{
    static long long id = 0;
    return (id++);
}

/**
 * @brief Returns a string representation of a token type.
 */
char *print_type(t_token *lst)
{
    if (!lst) // Check lst itself first
        return ("NULL TOKEN");
    if (!lst->value && lst->type != TOKEN_EOF) // Handle case where value might be NULL unexpectedly
         return ("INVALID TOKEN?"); // Or handle based on type

    // Based on e_token_type enum provided earlier
    switch (lst->type)
    {
        case TOKEN_PIPE: return ("PIPE");
        case TOKEN_REDIR_IN: return ("REDIR IN");
        case TOKEN_REDIR_OUT: return ("REDIR OUT");
        case TOKEN_APPEND: return ("APPEND");
        case TOKEN_HEREDOC: return ("HEREDOC");
        case TOKEN_CMD: return ("COMMAND");
        case TOKEN_WORD: return ("WORD");
        case TOKEN_EOF: return ("EOF");
        case REDIR: return ("REDIR_CORE"); // What is this type?
        case TOKEN_ASSIGNMENT: return ("ASSIGNMENT");
        default: return ("UNKNOWN_TYPE");
    }
    // Original code had checks like `if (!lst->value) return ("Root node");`
    // which seems incorrect as root shouldn't be printed here.
}

/**
 * @brief Prints the token list for debugging. (MODIFIED)
 * Removed unused variable 'tmp'.
 */
void print_token_lst(t_token *lst)
{
    // t_token *tmp; // REMOVED - Unused
    if(!lst) {
        // Use ft_putstr_fd for consistency if printf restricted
        ft_putstr_fd(RED "   (Token list is NULL)\n" PINK, 2); // Print to stderr
        return;
    }
    while (lst)
    {
        // Use ft_printf or stdio printf if allowed, otherwise use fd functions
        // Assuming ft_printf is allowed for now
        ft_printf(BLUE "( %s -> %s )\n" PINK,
                  print_type(lst),
                  lst->value ? lst->value : "(null value)"); // Handle NULL value safely
        lst = lst->next;
    }
}


/*t_token	*last_node(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

t_token *token_lst(t_token *token)
{
	static t_token *lst;

	if (lst)
		last_node(lst)->next = token;
	else 
		lst = token;*/
/*	static t_token *lst[4096];
	static int i = 0;
	
	lst[i++] = token;
	lst[i] = NULL;*/
//	return(lst);
//}
