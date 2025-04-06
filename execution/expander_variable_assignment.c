/* ************************************************************************** */
 /* expander_variable_assignment.c                                            */
 /* ************************************************************************** */

 #include "minishell.h"

 /*
 ** is_valid_assignment: Checks if a token is a valid variable assignment.
 ** A valid assignment has the format "VAR=VALUE".
 */
 int is_valid_assignment(const char *token)
 {
     int i = 0;
     if (!token || (!ft_isalpha(token[i]) && token[i] != '_'))
         return (0);
     while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
         i++;
     if (token[i] == '=')
         return (1);
     return (0);
 }

 /*
 ** apply_variable_assignment: Applies variable assignments found at the
 ** beginning of the token list to the shell environment. It *marks*
 ** assignment tokens instead of removing or modifying them.
 */
 void apply_variable_assignment(t_shell *shell, t_token *token_list)
 {
     t_token *current = token_list;
     char *equal_sign;

     while (current && is_valid_assignment(current->value))
     {
         equal_sign = ft_strchr(current->value, '=');
         if (equal_sign)
         {
             *equal_sign = '\0';
             if (update_env(&shell->env, current->value, equal_sign + 1) != 0)
             {
                 perror("update_env");
                 // Consider how to handle this error - maybe set exit status
             }
             *equal_sign = '='; // Restore for potential later use (if needed)
         }

         current->type = TOKEN_ASSIGNMENT; // Mark the token as an assignment
         current = current->next;
     }
 }

 void expand_token_list_no_assignments(t_token *token_list, char **env, int last_exit_status)
 {
     t_token *cur;
     char *expanded;

     cur = token_list;
     while (cur)
     {
         if (cur->value && !cur->literal && cur->type != TOKEN_ASSIGNMENT)
         {
             expanded = expand_variables(cur->value, env, last_exit_status);
             free(cur->value); // Free the original value
             cur->value = expanded;
         }
         cur = cur->next;
     }
 }

 void process_variable_assignments(t_shell *shell, t_token *token_list)
 {
     apply_variable_assignment(shell, token_list);
 }