CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = s21_grep
SRCS = s21_grep.c compile.c output.c parser.c process.c
OBJS = $(SRCS:.c=.o)

all: s21_grep

s21_grep: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c s21_grep.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re