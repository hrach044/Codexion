NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = src/main.c src/parse.c src/init.c src/time.c src/log.c \
	src/coder.c src/dongle.c src/heap.c src/monitor.c src/cleanup.c \
	src/sched.c
OBJS = $(SRCS:.c=.o)
HEADER = src/codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re