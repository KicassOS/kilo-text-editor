FLAGS = -Wall -Wextra -Werror
CC = gcc --std=c99

SRC = main.c
OBJ = $(SRC:.c=.o)
all: kilo.out

kilo.out: $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o kilo

clean:
	rm *.o

fclean:
	rm -f kilo

re:
	rm -f kilo
	make 