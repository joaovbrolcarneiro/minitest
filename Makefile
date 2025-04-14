# Updated Makefile with readline paths
NAME        = minishell
CC          = cc
##CFLAGS      = -I/usr/local/opt/readline/include -Iincludes -Ilibft
CFLAGS      = -Wall -Wextra -Werror -g -I/usr/local/opt/readline/include -Iincludes -Ilibft
LDFLAGS     = -L/usr/local/opt/readline/lib -lreadline -lncurses
#READLINE_PATH = /opt/homebrew/opt/readline
#CFLAGS += -I$(READLINE_PATH)/include
#LDFLAGS += -L$(READLINE_PATH)/lib -lreadline -lncurses

# Directories
INC         = includes
LIBFT_DIR   = libft
EXEC_DIR    = execution
PARSER_TL   = parser/tokenlst
PARSER_BT   = parser/binery_tree
PARSER_TREE = parser/tree
PARSER_QUOTE = parser/quote_handling

SRCS = \
    $(EXEC_DIR)/executor.c \
    $(EXEC_DIR)/executor_dispatch.c \
    $(EXEC_DIR)/executor_extra.c \
    $(EXEC_DIR)/executor_redir_utils.c \
    $(EXEC_DIR)/executor_simple_cmd.c \
    $(EXEC_DIR)/executor_utils.c \
    $(EXEC_DIR)/executor_redirections.c \
    $(EXEC_DIR)/executor_pipes.c \
    $(EXEC_DIR)/expander_utils.c \
    $(EXEC_DIR)/expander.c \
    $(EXEC_DIR)/expander_variable_assignment.c \
    $(EXEC_DIR)/executor_pipe_utils.c \
    $(PARSER_TL)/tokenlst.c \
    $(PARSER_TL)/tokenlst_v2.c \
    $(PARSER_TL)/yggdrasil.c \
    $(PARSER_BT)/binery_tree.c \
    $(PARSER_QUOTE)/quote_handling.c              

OBJS        = $(SRCS:.c=.o)
LIBFT       = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_DIR) main.c $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_DIR) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re