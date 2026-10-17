#include "s21_grep.h"

int compile_regex(regex_t *regexes, struct GrepData data, int *exit_code,
                  int *stop_flag) {
  int regex_compiled = 1;
  char error_message[256];
  for (int i = 0; i < data.patterns_count && !*stop_flag; i++) {
    int status = regcomp(&regexes[i], data.pattern[i], data.regex_flags);
    if (status != 0) {
      regerror(status, &regexes[i], error_message, sizeof(error_message));
      fprintf(stderr, "regex error: %s\n", error_message);
      *exit_code = 1;
      *stop_flag = 1;
    } else {
      regex_compiled = 0;
    }
  }
  return regex_compiled;
}