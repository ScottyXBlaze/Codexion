NAME= codexion

SRC	=	codexion.c \
		parsers.c \
		parser_utils.c \
		initializer.c \
		destructor.c \
		coders.c \
		time.c \
		dongle.c \


OBJS = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread -g
RM = rm -rf

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: $(NAME)

clean: 
	$(RM) $(OBJS) 

fclean: clean
	$(RM) $(NAME)

re: fclean all
so:
	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRC)
	gcc -nostartfiles -shared -o libft.so $(OBJ)

.phony:	all clean fclean re so
