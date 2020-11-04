# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbach <cbach@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/03 22:27:45 by cbach             #+#    #+#              #
#    Updated: 2020/11/04 22:32:40 by cbach            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	cub3D
FLAGS = -Wall -Wextra -Werror
MLX_FLAGS = -framework OpenGL -framework AppKit

LIBFT_DIR = libft

GNL_DIR = get_next_line

CUB3D_DIR = .

PARSER_DIR = parser

RAYCASTING_DIR = raycasting

MINILIBX_DIR = minilibx

MINILIBX = libmlx.dylib

HEADERS =										\
				$(wildcard $(LIBFT_DIR)/*.h)	\
				$(wildcard $(GNL_DIR)/*.h)		\
				$(wildcard $(CUB3D_DIR)/*.h)	\
				$(wildcard $(MINILIBX_DIR)/*.h)

HEAD =											\
				$(wildcard $(LIBFT_DIR)/*.h)	\
				$(wildcard $(GNL_DIR)/*.h)		\
				$(wildcard $(CUB3D_DIR)/*.h)	\

SRC = 		$(wildcard $(GNL_DIR)/*.c)			\
			$(wildcard $(PARSER_DIR)/*.c)		\
			$(wildcard $(RAYCASTING_DIR)/*.c)	\
			$(wildcard $(CUB3D_DIR)/*.c)


OBJ = 		$(patsubst %.c, %.o, $(SRC))
OBJ_BONUS = $(patsubst %.c, %.o, $(BONUS))

ifdef BONUS_FILES
OBJ_FILES = $(OBJ_BONUS)
else
OBJ_FILES = $(OBJ)
endif

.PHONY: all bonus clean fclean re make_libs norm


all: make_libs $(NAME)
	@rm -f .depend

make_libs:
	$(MAKE) -C libft all
	$(MAKE) -C libft bonus
	$(MAKE) -C minilibx all


$(NAME): $(OBJ_FILES)
	@cp ${MINILIBX_DIR}/${MINILIBX} ${CUB3D_DIR}
	@gcc $(FLAGS) $(MLX_FLAGS) $(OBJ_FILES) -L${LIBFT_DIR} -lft -L${MINILIBX_DIR} -lmlx -o $(NAME)

bonus:
	$(MAKE) BONUS_FILES=1 all

.depend: $(SRC) $(BONUS)
	@rm -f .depend
	@gcc $(FLAGS) -MM $^ > .depend

-include .depend

%.c:
	gcc $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ) $(OBJ_BONUS) .depend
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MINILIBX_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@rm -f ${MINILIBX}
	$(MAKE) -C $(LIBFT_DIR) fclean

norm:
	@norminette ${SRC} ${HEAD}

re: fclean all
