NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
HEADER = ./ft_printf.h
SRC_FILES = ft_printf.c
OBJ_FILES = $(patsubst %.c,%.o,$(SRC_FILES))

all: $(NAME)

$(NAME): $(OBJ_FILES) $(HEADER)
	ar -crs $(NAME) $?

bonus: $(NAME)

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

.PHONY: all clean fclean