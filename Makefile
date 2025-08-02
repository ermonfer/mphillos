NAME := philo

CC := cc
CFLAGS := -Wall -Wextra -Werror 
SRC_MAN := main.c \
	init_simulation.c \
	philo_routine.c \
	utilities.c

OBJ := $(SRC_MAN:.c=.o)

HEADER := philo.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus