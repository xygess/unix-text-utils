#include "s21_cat.h"

#include <string.h>

void process_file(char *arg, struct Flags flags, struct State *state,
                  int *exit_code) {
  FILE *fin = fopen(arg, "r");
  if (fin == NULL) {
    print_perror(arg, exit_code);
  } else {
    if (handle_file(fin, flags, state) != 0) print_perror(arg, exit_code);
    if (fclose(fin) != 0) print_perror(arg, exit_code);
  }
}

int main(int argc, char *argv[]) {
  int exit_code = 0;
  int parsing_option = 1;
  struct Flags flags = {0};
  int stop_flag = 0;
  int input_detection = 0;
  struct State state = {1, 1, 0};
  for (int i = 1; i < argc && !stop_flag; i++) {
    if (parsing_option && strcmp(argv[i], "--") == 0) {
      parsing_option = 0;
    } else if (strcmp(argv[i], "-") == 0) {
      input_detection = 1;
      parsing_option = 0;
      if (handle_file(stdin, flags, &state) != 0)
        print_perror("stdin", &exit_code);
    } else if (parsing_option && argv[i][0] == '-') {
      if (strncmp(argv[i], "--", 2) == 0) {
        long_flags(argv[i], &stop_flag, &exit_code, argv[0], &flags);
      } else {
        short_flags(argv[i], &stop_flag, &exit_code, argv[0], &flags);
      }
    } else {
      input_detection = 1;
      parsing_option = 0;
      process_file(argv[i], flags, &state, &exit_code);
    }
  }
  if (!stop_flag && !input_detection) {
    if (handle_file(stdin, flags, &state) != 0)
      print_perror("stdin", &exit_code);
  }

  return exit_code;
}