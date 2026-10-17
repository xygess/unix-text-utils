#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int exit_code = 0;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s pattern [file... ]\n", argv[0]);
    exit_code = 1;
  } else {
    regex_t regexes[MAX_PATTERNS];
    int stop_flag = 0;
    struct GrepData data = {0};
    if (parse_args(argc, argv, &data, &stop_flag, &exit_code) != 0) {
      exit_code = 1;
    } else if (compile_regex(regexes, data, &exit_code, &stop_flag) != 0) {
      exit_code = 1;
    } else {
      process_inputs(argc, argv, data, regexes, &exit_code);
      for (int k = 0; k < data.patterns_count; k++) {
        regfree(&regexes[k]);
        free(data.pattern[k]);
      }
    }
  }

  return exit_code;
}