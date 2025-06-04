NAME = philo
CFLAGS = -g  -Werror -Wextra -Werror #-fsanitize=thread 
CC = cc
OBJ = philo.o philo_helper.o time.o helper.o philo_action.o

all : $(NAME)
$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all