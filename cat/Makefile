CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = s21_cat
SRCS = s21_cat.c flags.c output.c
OBJS = $(SRCS:.c=.o)

all: s21_cat

s21_cat: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c s21_cat.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re