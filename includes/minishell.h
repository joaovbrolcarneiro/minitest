#ifndef MINISHELL_H
 # define MINISHELL_H

 # include <stdbool.h>
 # include <stdlib.h>
 # include <unistd.h>
 # include <stdio.h>
 # include <signal.h>
 # include <sys/wait.h>
 # include <fcntl.h>
 # include <errno.h>
 # include <dirent.h>
 # include <readline/readline.h>
 # include <readline/history.h>
 # include <limits.h>
 # include <string.h>

 # include "../libft/libft.h"
 # include "../libft/ft_printf.h"
 # include "../libft/get_next_line.h"

 /*
 ** Colors
 */
 # define RED     "\033[0;31m"
 # define REDK    "\033[38;2;220;20;60m"
 # define PINK    "\033[38;2;255;0;127m"
 # define BLUE    "\033[0;34m"
 # define CYAN    "\033[0;36m"
 # define GREEN   "\033[0;32m"
 # define YELLOW  "\033[38;2;255;150;0m"
 # define BROWN   "\033[38;2;105;50;0m"
 # define PURPLE  "\033[0;35m"
 # define RESET   "\033[0;0m"

 /*
 ** Global variables
 */
 extern int      g_exit_code;

 /*
 ** Types
 */

typedef enum e_token_type {
    TOKEN_PIPE = 0,       // 0
    TOKEN_REDIR_IN = 1,   // 1
    TOKEN_REDIR_OUT = 2,  // 2 -> Deve mapear para AST_REDIR_OUT
    TOKEN_APPEND = 3,     // 3 -> Deve mapear para AST_APPEND
    TOKEN_HEREDOC = 4,    // 4 -> Deve mapear para AST_HEREDOC
    TOKEN_CMD = 5,        // 5 -> Deve mapear para AST_COMMAND
    TOKEN_WORD,           // 6
    TOKEN_EOF,            // 7
    REDIR,                // 8
    TOKEN_ASSIGNMENT      // 9
} t_token_type;

 typedef enum e_ast_type {
    AST_PIPE = 0,         // OK (TOKEN_PIPE = 0)
    AST_REDIR_IN = 1,     // OK (TOKEN_REDIR_IN = 1)
    AST_REDIR_OUT = 2,    // ALTERADO! (para corresponder a TOKEN_REDIR_OUT = 2)
    AST_APPEND = 3,       // ALTERADO! (para corresponder a TOKEN_APPEND = 3)
    AST_HEREDOC = 4,      // ALTERADO! (para corresponder a TOKEN_HEREDOC = 4)
    AST_COMMAND = 5,      // ALTERADO! (para corresponder a TOKEN_CMD = 5)
    // Você pode precisar adicionar outros tipos AST se necessário,
    // garantindo que os valores não colidam ou definindo-os explicitamente.
} t_ast_type;

 typedef enum e_ranking
 {
     RANK_F,
     RANK_E,
     RANK_D,
     RANK_C,
     RANK_B,
     RANK_A,
     RANK_S,
     RANK_SS
 }               t_ranking;

 typedef struct s_token
 {
     long long       id;
     t_token_type    type;
     char            *value;
     struct s_token  *next;
     t_ranking       rank;
     bool            used;
     int             err;
     char            **args;
     char            *file;
     t_token_type    coretype;
     bool            literal;
     bool            merge_next;
 }               t_token;

 typedef struct s_node_tree
 {
     t_ranking           rank;
     t_ast_type          type;
     char                **args;
     char                *file;
     char                *content;
     struct s_node_tree  *left;
     struct s_node_tree  *right;
 }               t_node_tree;

 typedef t_node_tree *t_tree;

 typedef enum e_direction_node
 {
     RIGHT,
     LEFT
 }               t_direction_node;

 typedef struct s_shell
 {
     char        **env;
     int         exit_status;
     int         saved_stdin;
     int         saved_stdout;
     int         heredoc_fd;
     int         in_heredoc;
     t_node_tree *ast_root;
 }               t_shell;

  typedef struct s_exp_vars {
     const char  *input;
     char        **env;
     int         last_exit_status;
     size_t      i;
     size_t      res_len;
     size_t      res_cap;
     char        *result;
     size_t      var_start;
     int         pos;
     char        *var_value;
 }   t_exp_vars;

 /*
 ** Parser functions
 */
 t_token     *tokenize(char *input, char **env);
 t_node_tree *parse_tokens(t_token *tokens);
 void        free_tokens(t_token *tokens);
 char        *expand_vars(char *str, char **env, int status);
 void        handle_quotes(char *str, int *i, char quote);

 /*
 ** Executor functions
 */
 int         execute_ast(t_shell *shell, t_node_tree *node);
 int         execute_command(t_shell *shell, char **args);
 int         execute_pipe_command(t_shell *shell, t_node_tree *node);
 int         execute_simple_command(t_shell *shell, t_node_tree *node);
 int         execute_external_command(t_shell *shell, char **args);
 int         handle_redirections(t_node_tree *node);
 int         handle_heredoc(t_node_tree *node);
 int         setup_pipes(int pipefd[2]);
 void        save_std_fds(t_shell *shell);
 void        restore_std_fds(t_shell *shell);
 void        handle_child_signals(void);
 void        handle_parent_signals(void);
 int         handle_redir_in(t_node_tree *node);
 int         handle_redir_out(t_node_tree *node);
 int         handle_append(t_node_tree *node);
 void        restore_fds(int fds[2]);
 void        close_fds(int fds[2]);

 /*
 ** Builtin functions
 */
 int         ft_echo(char **args);
 int         ft_cd(char **args, char ***env);
 int         ft_pwd(void);
 int         ft_export(char **args, char ***env);
 int         ft_exit(char **args, t_shell *shell);

 /*
 ** Utils functions
 */
 void        init_shell(t_shell *shell, char **env);
 void        cleanup_shell(t_shell *shell);
 void        ft_free_strarray(char **array);
 char        *find_command_path(char *cmd, char **env);
 char        *ft_path_join(char *path, char *file);
 char        *get_env_value(char **env, const char *name);
 char        *get_envar(char **env, char *var);
 size_t      ft_strnlen(char *s, char n);
 void        print_env(char **env);

 /*
 ** Token manipulation functions
 */
 void        print_token_lst(t_token *lst);
 t_token     *get_lastone_nodeof_rank(t_token *lst, t_ranking this_ranking);
 t_token     *get_prev_node(t_token *node, t_token *lst);
 t_token     *untie_node(t_token *node, t_token *lst);
 t_token     *last_one_of_lst(t_token_type who, t_token *lst);

 /*
 ** AST / Tree manipulation functions
 */
 
 char        *domane_expantion(char **env, char *input);
 void    expand_token_list(t_token *token_list, char **env /* REMOVED int last_exit_status */); // If you use this one int update_env(char ***env, char *var, char *value);
 char **sort_env(char **env);

 void    left_child(t_shell *shell, t_node_tree *left, int pipefd[2]);
 void    right_child(t_shell *shell, t_node_tree *right, int pipefd[2]);
 int     fork_left(t_shell *shell, t_node_tree *node, int pipefd[2]);
 int fork_right(t_shell *shell, t_node_tree *node, int pipefd[2]);

void    exp_var_init(t_exp_vars *v, const char *input, char **env /* REMOVED , int last_exit_status */);
 int     append_char(char **buf, size_t *len, size_t *cap, char c);
 void    handle_exit_status(t_exp_vars *v);
 void    handle_variable(t_exp_vars *v);
 void    handle_dollar_expansion(t_exp_vars *v);
 void    append_normal_char(t_exp_vars *v);
 char    *get_env_value_exp(const char *name, char **env);
 int     is_valid_var_char(char c, int pos);
 int is_valid_assignment(const char *token);
 void apply_variable_assignment(t_shell *shell, t_token *token_list);
 void process_variable_assignments(t_shell *shell, t_token *token_list);
 char    *expand_variables(const char *input, char **env /* REMOVED int last_exit_status */); 
 void    expand_token_list_no_assignments(t_token *token_list, char **env /* REMOVED int last_exit_status */); // If you use this onechar **ft_strdup_array(char **array);
bool is_builtin(const char *cmd);
int ft_unset(char **args, t_shell *shell);
bool is_valid_identifier(const char *identifier);
int update_env(char ***env, char *var, char *value);
int	handle_redir_execution(t_shell *shell, t_node_tree *node);
int	handle_command_execution(t_shell *shell, t_node_tree *node);
int	handle_word_token_execution(t_node_tree *node);
int	handle_assignment_execution(t_node_tree *node);
int	handle_pipe_execution(t_shell *shell, t_node_tree *node);
int	save_original_fds(int original_fds[2]);
int	execute_redir_cmd_node(t_shell *shell, t_node_tree *redir_node);
int	read_heredoc_input(int pipe_write_fd, const char *delimiter);

 #endif