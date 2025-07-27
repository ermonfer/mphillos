NAME_MAN := philo

CC := cc
CFLAGS := -Wall -Wextra -Werror 
SRC_MAN := main.c \
	init_simulation.c \
	philo_routine.c \
	utilities.c

OBJ_MAN := $(SRC_MAN:.c=.o)

HEADER := philo.h

all: $(NAME_MAN)

$(NAME_MAN): $(OBJ_MAN)
	$(CC) $(CFLAGS) $(OBJ_MAN) -o $(NAME_MAN)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_MAN) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME_MAN) $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean re bonus