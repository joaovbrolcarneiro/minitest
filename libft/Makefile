# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/21 11:57:23 by hde-barr          #+#    #+#              #
#    Updated: 2025/04/03 17:51:44 by hde-barr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= libft.a
CC= cc
CFLAGS= -Wall -Wextra -Werror
AR= ar rc
RM= rm -rf
SRCS= ft_isalpha.c\
      ft_isdigit.c\
      ft_isalnum.c\
      ft_isascii.c\
      ft_isprint.c\
      ft_strlen.c\
      ft_memset.c\
      ft_bzero.c\
      ft_memcpy.c\
      ft_memmove.c\
	  ft_strcmp.c\
      ft_strlcpy.c\
      ft_strlcat.c\
      ft_toupper.c\
      ft_tolower.c\
	  ft_strchr.c\
	  ft_strrchr.c\
	  ft_strncmp.c\
	  ft_memchr.c\
	  ft_memcmp.c\
	  ft_strnstr.c\
	  ft_atoi.c\
	  ft_calloc.c\
	  ft_strdup.c\
	  ft_substr.c\
	  ft_strjoin.c\
	  ft_strtrim.c\
	  ft_split.c\
	  ft_itoa.c\
	  ft_strmapi.c\
	  ft_striteri.c\
	  ft_putchar_fd.c\
	  ft_putstr_fd.c\
	  ft_putendl_fd.c\
	  ft_putnbr_fd.c\
	  ft_printf.c ft_putptr.c ft_putulnbr_base.c \
	  ft_putunbr_base.c ft_putchar.c ft_putstr.c ft_putnbr.c\
		get_next_line.c\
		get_next_line_utils.c\
		garbege_colector.c
SRC_BONUS= ft_lstnew.c\
           ft_lstadd_front.c\
		   ft_lstsize.c\
		   ft_lstlast.c\
		   ft_lstadd_back.c\
		   ft_lstdelone.c\
		   ft_lstclear.c\
		   ft_lstiter.c\
		   ft_lstmap.c\
		get_next_line_bonus.c\
		get_next_line_utils_bonus.c
OBJS=$(SRCS:.c=.o)
OBJS_BONUS=$(SRC_BONUS:.c=.o)
INCL= libft.h

all:$(NAME)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)

%.o : %.c $(INCL)
	@$(CC) $(CFLAGS) -c -I . $< -o $@

clean:
	@$(RM) $(OBJS) $(OBJS_BONUS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

bonus: $(OBJS) $(OBJS_BONUS)
	@$(AR) $(NAME) $(OBJS) $(OBJS_BONUS)