# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/03 22:27:45 by cbach             #+#    #+#              #
#    Updated: 2020/11/03 23:28:32 by cbach            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	cub3D
FLAGS = -Wall -Wextra -Werror

HEADERS =								\
		get_next_line/get_next_line.h	\
		cub3d.h							\
		mlx.h

GNL_SRCS = get_next_line/*.c

PARSER_SRCS = parser/*.c

RAYCASTING_SRCS = raycasting/*.c

MAIN_SRCS = *.c

SRC = $(GNL_SRCS) $(PARSER_SRCS) $(RAYCASTING_SRCS) $(MAIN_SRCS)

OBJ = $(patsubst %.c, %.o, $(SRC))

.PHONY: all clean fclean re

all: make_libs $(NAME)
	rm -f .depend
	

$(NAME): $(OBJ_FILES)
	gcc $(FLAGS) $? -L./libft -lft -L./minilibx -lmlx -o $(NAME)

make_libs:
	$(MAKE) -C libft all
	$(MAKE) -C libft bonus
	$(MAKE) -C minilibx all

.depend: $(SRC)
	rm -f .depend
	gcc $(FLAGS) -MM $^ > .depend;

-include .depend

%.o:
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) .depend
	$(MAKE) -C libft clean
	$(MAKE) -C minilibx clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C libft fclean

re: fclean all
