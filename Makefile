# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/24 13:00:14 by ndo-vale          #+#    #+#              #
#    Updated: 2024/08/01 17:32:32 by fivieira         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC	= cc
CFLAGS	= -Wall -Werror -Wextra -g
IFLAGS	= -I/usr/include/readline
LDFLAGS	= -L/usr/lib -lreadline
RM	= rm -rf

SRC_DIR	= src/
OBJ_DIR	= obj/

SRCS	= $(addprefix $(SRC_DIR), main.c free_exit.c signals.c signals_handlers.c handle_syntax.c \
	  		$(addprefix tokenizer/, tokenize_line.c tokenize_line2.c \
			tokenizer_parsers.c tokenizer_exit_free.c) \
			tokenlst_helpers.c get_env_value.c \
			$(addprefix tree_builder/, build_tree.c free_tree.c \
			node_constructors.c) \
			$(addprefix heredoc/, set_heredocs.c heredoc.c \
			heredoc_helpers.c create_heredoc_file.c \
			get_env_value_hd.c) \
			$(addprefix tree_executer/, execute_node.c command_helpers.c) \
			$(addprefix builtins/, builtins_utils.c ft_echo.c ft_cd.c \
			ft_pwd.c ft_export.c ft_unset.c ft_env.c ft_exit.c))
OBJS	= $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

LIBFT_DIR	= ./libft
LIBFT		= $(LIBFT_DIR)/libft.a

all: $(NAME)

$(LIBFT):
	make -s -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p obj
	@mkdir -p obj/tokenizer
	@mkdir -p obj/tree_builder
	@mkdir -p obj/builtins
	@mkdir -p obj/tree_executer
	@mkdir -p obj/heredoc
	@mkdir -p .tempfiles
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	make -s clean -C $(LIBFT_DIR)
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(LIBFT)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re 
