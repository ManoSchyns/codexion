NAME = codexion
SRC = coders/coder_utils.c coders/coder.c coders/main_utils.c coders/main.c coders/monitor_utils.c coders/monitor.c coders/parsing.c coders/scheduler_edf.c coders/scheduler_fifo.c coders/scheduler.c coders/setup.c
INCLUDE = coders/codexion.h
OBJ = $(SRC:.c=.o)
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	cc $(FLAGS) $(OBJ) -I$(INCLUDE) -o $(NAME) 

%.o: %.c
	cc -c $(FLAGS) $< -o $@

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all