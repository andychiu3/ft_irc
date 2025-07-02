# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 15:39:37 by jmaiolo           #+#    #+#              #
#    Updated: 2025/03/24 21:48:41 by fiftyblue        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILE = main Server Client Config Channel Replies utils
CMD = pass nick user quit \
	join part invite kick topic mode \
	privmsg notice list names ping

SRC = $(addprefix src/, $(addsuffix .cpp, $(FILE))) \
		$(addprefix src/cmd/, $(addsuffix .cpp, $(CMD)))

OBJ = $(addprefix obj/, $(notdir $(SRC:.cpp=.o)))

vpath %.cpp src/ src/cmd

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Iincludes
# CFLAGS += -fsanitize=address

NAME = ircserv

all: $(NAME)

bonus: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf obj/

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
