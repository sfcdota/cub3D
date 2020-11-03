# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbach <cbach@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/17 15:43:49 by cbach             #+#    #+#              #
#    Updated: 2020/05/28 23:37:45 by cbach            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	libft.a
NAME_SO = libft.so
HEAD =	libft.h
FLAGS = -Wall -Wextra -Werror

SRC =						\
		ft_atoi.c			\
		ft_bzero.c			\
		ft_isalnum.c		\
		ft_isalpha.c		\
		ft_isascii.c		\
		ft_isdigit.c		\
		ft_isprint.c		\
		ft_itoa.c			\
		ft_memchr.c			\
		ft_memcmp.c			\
		ft_memmove.c		\
		ft_memset.c			\
		ft_putchar_fd.c		\
		ft_putendl_fd.c		\
		ft_putnbr_fd.c		\
		ft_putstr_fd.c		\
		ft_split.c			\
		ft_strchr.c			\
		ft_strjoin.c		\
		ft_strlcat.c		\
		ft_strlcpy.c		\
		ft_strlen.c			\
		ft_strmapi.c		\
		ft_strncmp.c		\
		ft_strrchr.c		\
		ft_strtrim.c		\
		ft_substr.c			\
		ft_tolower.c		\
		ft_toupper.c		\
		ft_calloc.c			\
		ft_memccpy.c		\
		ft_memcpy.c			\
		ft_strdup.c			\
		ft_strnstr.c		\

BONUS =						\
		ft_lstnew.c			\
		ft_lstadd_front.c	\
		ft_lstsize.c		\
		ft_lstlast.c		\
		ft_lstadd_back.c	\
		ft_lstdelone.c		\
		ft_lstclear.c		\
		ft_lstiter.c		\
		ft_lstmap.c			\

OBJ = $(patsubst %.c, %.o, $(SRC))
OBJ_BONUS = $(patsubst %.c, %.o, $(BONUS))

ifdef BONUS_FILES
OBJ_FILES = $(OBJ_BONUS)
else
OBJ_FILES = $(OBJ)
endif

.PHONY: all bonus clean fclean re so

all: $(NAME)
	rm -f .depend

$(NAME): $(OBJ_FILES)
	ar -rcs $(NAME) $?

bonus:
	$(MAKE) BONUS_FILES=1 all

.depend: $(SRC) $(BONUS)
	rm -f .depend
	gcc $(FLAGS) -MM $^ > .depend;

-include .depend

%.o:
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJ_BONUS) .depend

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_SO)

re: fclean all

so: $(NAME_SO)

$(NAME_SO):
	gcc -c -fPIC *.c
	gcc -shared -o libft.so *.o
