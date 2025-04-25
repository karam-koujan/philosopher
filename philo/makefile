NAME = philo
CFLAGS = -g -Werror -Wextra -Werror
CC = cc
OBJ = philo.o philo_helper.o

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)
