# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mariverg <mariverg@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 12:16:49 by mariverg          #+#    #+#              #
#    Updated: 2024/11/07 10:53:46 by mariverg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosopher

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRCS =	main.c \
		init.c \
		threads.c \
		utils.c \
		randomphilo.c

OBJS = $(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all
