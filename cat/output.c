#include "s21_cat.h"

void print_number(struct State *state, int *exit_code) {
  if (printf("%6d\t", state->number_line) < 0) *exit_code = 1;
  state->line_start = 0;
  state->number_line += 1;
}

void squeeze_count(struct State *state, int ch) {
  if (state->line_start && ch == '\n') {
    state->empty_line += 1;
  } else {
    state->empty_line = 0;
  }
}

void print_perror(char *in, int *exit_code) {
  perror(in);
  *exit_code = 1;
}

void print_char(struct Flags flags, int ch, int *exit_code) {
  if (flags.tabs && ch == '\t') {
    if (printf("^I") < 0) *exit_code = 1;
  } else if (flags.ends && ch == '\n') {
    if (printf("$\n") < 0) *exit_code = 1;
  } else if (flags.v && ch >= 0 && ch < 32 && ch != '\t' && ch != '\n') {
    if (printf("^%c", ch + 64) < 0) *exit_code = 1;
  } else if (flags.v && ch == 127) {
    if (printf("^?") < 0) *exit_code = 1;
  } else if (flags.v && ch >= 128 && ch < 160) {
    if (printf("M-^%c", ch - 64) < 0) *exit_code = 1;
  } else if (flags.v && ch >= 160 && ch < 255) {
    if (printf("M-%c", ch - 128) < 0) *exit_code = 1;
  } else if (flags.v && ch == 255) {
    if (printf("M-^?") < 0) *exit_code = 1;
  } else if (putchar(ch) == EOF)
    *exit_code = 1;
}

int handle_file(FILE *file, struct Flags flags, struct State *state) {
  int exit_code = 0;
  int ch;

  while ((ch = fgetc(file)) != EOF) {
    if (flags.squeeze_blank) squeeze_count(state, ch);
    if (state->empty_line < 2) {
      if (flags.number_nonblank) {
        if (state->line_start && ch != '\n') print_number(state, &exit_code);
      } else if (flags.number) {
        if (state->line_start) print_number(state, &exit_code);
      }
      print_char(flags, ch, &exit_code);
      if (ch == '\n') {
        state->line_start = 1;
      } else {
        state->line_start = 0;
      }
    }
  }

  if (ferror(file)) {
    exit_code = 1;
  }

  return exit_code;
}