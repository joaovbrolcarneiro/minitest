#ifndef MINISHELL_PART2_H 
#define MINISHELL_PART2_H

#include "minishell.h"
#include <sys/stat.h>
#define QUOTE_SET "!@#&£$% \f\n\r\t\v-+'"
# define RST "\001\e[0m\002"
# define ULI "\001\e[4m\002"
# define BLN "\001\e[5m\002"
# define BLK "\001\e[1;30m\002"
# define PNK "\001\e[38;2;255;0;127m\002"
# define YLW "\001\e[38;2;255;150;0m\002"
# define RDK "\001\e[38;2;220;20;60m\002"
//# define RED "\e[1;31m"
# define GRN "\001\e[1;32m\002"
//# define YLW "\e[1;33m"
# define BLU "\001\e[1;34m\002"
# define PRP "\001\e[1;35m\002"
# define CYN "\001\e[1;36m\002"
# define WHT "\001\e[1;37m\002"
# define CLR "\001\e[0;39m\002"
# define STAR "\001⭐\002"
# define YEN "\001¥\002"
# define BWN   "\001\e[38;2;105;50;0m\002"
# define TITLE BWN"``"PNK"K"YLW"o"PNK"N"YLW"o"PNK"S"YLW"u"PNK"B"YLW"a"PNK"sh!"BWN"\""GRN"$-> "RST
//♦
# define UPPER_PROMPT "╭─ \001\e[4;36m\002"
# define LP_L "╰─ \001\e[1;35m\002KONOSUBASH"
# define LP_R "\001\e[1;33m\002 $-> \001\e[0m\b\002"
/*typedef struct s_envar_obj
{
	char *key;
	char *value;
}	t_envar_obj;

enum e_var
{
	KEY = 0,
	VALUE = 1
};*/
typedef struct s_token_vec
{
	t_token	*first;
	t_token	*so_far;
}	t_token_vec;
//bool search_list(char *search, char **env);
int	ischarset(const char *set, char c);
//long long get_token_id();
//t_token *find_right_token(t_token *token, t_token *eof);
t_token *get_prev_node(t_token *node, t_token *lst);
//t_token *find_left_token(t_token *token, t_token *first);
//t_node_tree *init_yggdrasil(t_token *token_lst);
//void print_yggdrasil(t_node_tree *yggdrasil, int num_tabs, char *leg);
t_token *typealize(t_token *token, char **env);
//t_token *split_input(char *input, int i);
t_token *get_lastone_nodeof_rank(t_token *lst, t_ranking this_ranking);
t_token *untie_node(t_token *node, t_token *lst);
size_t ft_strsetlen(char *s, const char *set);
size_t ft_strnlen(char *s, char n);
char *get_envar(char **env, char *var);
void print_env(char **env);
char *domane_expantion(char **env, char *input);
int quote_handler_counter(char *input, char **env);
char *quote_handler_cpy(int count, char *input, char **env);
char *is_quote_opened(char *input, int *is_unclosed);
bool handler_quote_operator(char *input);
char  *quote_handler(t_token *token, char **env, int *is_unclosed);
bool proximity_exception(char *input, int i);/////////split_input
t_token *split_input(char *input, int i);/////////split_input
t_token *new_pipe(t_token *token);////////////init_node1
t_token *new_redir(t_token *token);///////////////init_node1
t_token *new_cmd(t_token *token);///////////////////init_node1
t_token *new_word(t_token *token);////////////////init_node1
t_token *new_eof(t_token *token);//////////////////init_node1
t_token *new_token_super(t_token *token);////////////////init_node0
bool is_redir_super(char *input);//////////who_is_node
bool is_word_super(char *input);//////////who_is_node
bool is_pipe_super(char *input);//////////who_is_node
bool is_cmd_super(char *input, char **env);//////////who_is_node
bool is_eof(char *input);//////////who_is_node
char **get_path_list(char **env);///////envar1
char **command_list_malloc(char **list, char **env);///////envar1
char **init_command_list(long long count, char **env);///////envar1
bool search_list(char *search, char **env);///////////envar1
long long get_token_id();//////////utils1
char *print_type(t_token *lst);/////////////dev_uitls/print_token_lst
void print_token_lst(t_token *lst);///////////dev_uttils/print_token_lst
t_token *find_right_token(t_token *token, t_token *eof);////////yggdrasil
t_token *find_left_token(t_token *token, t_token *first);////////yggdrasil
t_node_tree *new_yggnode(t_token *token);////////yggdrasil
t_node_tree	*make_yggdrasil(t_token *t, t_token *f, t_token *e, t_node_tree *y);////////yggdrasil
bool token_err(t_token *token_lst);////////////////init_yggdrasil
t_node_tree *init_yggdrasil(t_token *token_lst);////////////////init_yggdrasil
void print_yggdrasil(t_node_tree *yggnode, int num_tabs, char *leg);///////print_yggdrasil
bool konopwd(bool cmd_exist, const char *input);/////rare_candy
t_token *rm_node_lst(t_token *token, t_token *first);//////////lkd_lst1
void add_node_lst(t_token *dst_lst, t_token *token);//////////lkd_lst1
void join_and_split(t_token *priv, t_token *token);/////////////handler_args_file
t_token *redir_handler_file(t_token *token ,t_token *first);/////////////handler_args_file
t_token *cmd_handler_args(t_token *token, t_token *first);/////////////handler_args_file
t_token *handler_args_file(t_token *token, t_token *first);/////////////handler_args_file
t_token *last_token(t_token *token);//////////lkd_lst2
void handler_pipes(t_token_vec *token_vec);/////////handler_pipes
t_token *remap_lst(t_token *token);///////////////lkd_lst2
void handle_ctrl_c(int sig);///////////////handler_ctrl_c
void is_minishell_exit(char *input);//////not to use
bool merge_to_token_exception(t_token *token);/////quote_handling/merge_to_token_exception
void parser_cmd_no_found(t_token *token, char **env);///////exceptions_msg/parser_cmd_no_found
void quote_handler_call_loop(t_token *token, char **env);///////call_loops
void typealize_call_loop(t_token *token, char **env);/////////call_loops
void merg_tok_excep_cll_revloop(t_token *token);//////call_loops
t_token *delegated_by_input_handler(char *input, char **env);/////////////////input_handler
void input_handler(t_shell *shell, char *input);
//void input_handler(char **env, char *input);//////input_handler
void readline_loop(t_shell *shell);//novo loop
//void readline_loop(char **env);///////////readline_loop
bool is_executable(const char *path);
bool is_regular_file(const char *path);
bool is_valid_exc(const char *path);

#endif