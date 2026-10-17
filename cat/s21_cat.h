#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>

struct Flags {
  int number_nonblank;
  int number;
  int squeeze_blank;
  int ends;
  int tabs;
  int v;
};

struct State {
  int number_line;
  int line_start;
  int empty_line;
};

void print_number(struct State *state, int *exit_code);
void squeeze_count(struct State *state, int ch);
void print_perror(char *in, int *exit_code);
void print_char(struct Flags flags, int ch, int *exit_code);
int handle_file(FILE *file, struct Flags flags, struct State *state);
void long_flags(char *arg, int *stop_flag, int *exit_code, char *program,
                struct Flags *flags);
void short_flags(char *arg, int *stop_flag, int *exit_code, char *program,
                 struct Flags *flags);
void process_file(char *arg, struct Flags flags, struct State *state,
                  int *exit_code);

#endif