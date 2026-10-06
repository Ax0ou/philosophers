NAME		= philo

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
INC			= -I includes
HEADER		= includes/philo.h

SRC			= src/main.c \
			  src/parsing.c \
			  src/init.c \
			  src/simulation.c \
			  src/routine.c \
			  src/monitor.c \
			  src/print.c \
			  src/time.c \
			  src/stop.c \
			  src/cleanup.c

OBJ			= $(SRC:.c=.o)

RM			= rm -f

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(NAME) compiled"

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@$(RM) $(OBJ)
	@echo "Object files cleaned"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME) removed"

re: fclean all

.PHONY: all clean fclean re
