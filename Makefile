NAME1 = encoder

NAME2 = decoder

CC = gcc

CFLAGS = -g -Wall -Wextra #-Werror

SOURCE1 = encoder.c utils.c

SOURCE2 = decoder.c utils.c

OBJ = $(SOURCE:.c=.o)

DEFS = header.h

all: $(NAME1) $(NAME2)

$(NAME1) : encoder.c  $(DEFS)
	$(CC) $(CFLAGS) $(SOURCE1) -o encoder
$(NAME2) : decoder.c $(DEFS)
	$(CC) $(CFLAGS) $(SOURCE2) -o decoder

clean:
	@rm -rf $(OBJ)
fclean: clean
	@rm -rf $(NAME1) $(NAME2)

re: fclean all
