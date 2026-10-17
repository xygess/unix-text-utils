#include "s21_grep.h"

void print_perror(char *in, struct Flags flags, int *exit_code) {
  if (!flags.no_error) {
    perror(in);
  }
  *exit_code = 1;
}

void process_file(char *arg, struct GrepData data, regex_t *regexes, int argc,
                  int *exit_code) {
  FILE *fin = fopen(arg, "r");
  if (fin == NULL) {
    print_perror(arg, data.flags, exit_code);
  } else {
    if (handle_file(fin, arg, data, regexes, argc - data.file_start_index) != 0)
      print_perror(arg, data.flags, exit_code);
    if (fclose(fin) != 0) print_perror(arg, data.flags, exit_code);
  }
}

void process_stdin(struct GrepData data, regex_t *regexes, int argc,
                   int *exit_code) {
  clearerr(stdin);
  if (handle_file(stdin, "(standard input)", data, regexes,
                  argc - data.file_start_index) != 0)
    print_perror("stdin", data.flags, exit_code);
}

void process_inputs(int argc, char *argv[], struct GrepData data,
                    regex_t *regexes, int *exit_code) {
  if (data.file_start_index >= argc) {
    process_stdin(data, regexes, argc, exit_code);
  } else {
    for (int i = data.file_start_index; i < argc; i++) {
      if (strcmp(argv[i], "-") == 0) {
        process_stdin(data, regexes, argc, exit_code);
      } else {
        process_file(argv[i], data, regexes, argc, exit_code);
      }
    }
  }
}