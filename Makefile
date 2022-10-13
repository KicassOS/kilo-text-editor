NAME = kilo.out
FLAGS = -Wall -Wextra -Werror
CC = gcc --std=c99

SRC = main.c
OBJ = $(SRC:.c=.o)
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	rm *.o

fclean:
	rm -f kilo

re:
	rm -f kilo
	make 