#include "s21_grep.h"

void print_filename_number(char *filename, struct GrepData data, int count_f,
                           int line, int *exit_code) {
  if (count_f > 1 && !data.flags.no_filename) {
    if (printf("%s:", filename) < 0) *exit_code = 1;
  }
  if (data.flags.number) {
    if (printf("%d:", line) < 0) *exit_code = 1;
  }
}

void search_or_cursor(char *cursor, regmatch_t best_match,
                      int *keep_searching) {
  if (cursor[best_match.rm_eo] == '\0') {
    *keep_searching = 0;
  } else {
    cursor += best_match.rm_eo + 1;
  }
}

void find_best_matches(int *found, regmatch_t *best_match,
                       regmatch_t current_match) {
  if (!*found || current_match.rm_so < best_match->rm_so ||
      (current_match.rm_so == best_match->rm_so &&
       current_match.rm_eo > best_match->rm_eo)) {
    *found = 1;
    *best_match = current_match;
  }
}

int print_only_matches(char *str, char *filename, struct GrepData data,
                       regex_t *regexes, int count_f, int line) {
  int exit_code = 0;
  int found_any = 0;
  char *cursor = str;
  int keep_searching = 1;
  while (*cursor != '\0' && keep_searching) {
    int found = 0;
    regmatch_t best_match;
    for (int i = 0; i < data.patterns_count; i++) {
      regmatch_t current_match;
      int status = regexec(&regexes[i], cursor, 1, &current_match, 0);
      if (status == 0) {
        find_best_matches(&found, &best_match, current_match);
      }
    }
    if (!found) {
      keep_searching = 0;
    } else {
      int len = best_match.rm_eo - best_match.rm_so;
      if (len > 0) {
        found_any = 1;
        print_filename_number(filename, data, count_f, line, &exit_code);
        if (printf("%.*s\n", len, cursor + best_match.rm_so) < 0) exit_code = 1;
        cursor += best_match.rm_eo;
      } else {
        search_or_cursor(cursor, best_match, &keep_searching);
      }
    }
  }

  int result = found_any;
  if (exit_code != 0) {
    result = -1;
  }

  return result;
}

void handle_match_not(struct GrepData data, char *str, char *filename,
                      regex_t *regexes, int count_f, int line, int *exit_code) {
  if (data.flags.only_match && !data.flags.invert) {
    int only_match_status =
        print_only_matches(str, filename, data, regexes, count_f, line);
    if (only_match_status < 0) {
      *exit_code = 1;
    }
  } else if (!data.flags.only_match) {
    print_filename_number(filename, data, count_f, line, exit_code);
    if (printf("%s", str) < 0) *exit_code = 1;
  }
}

int handle_file(FILE *file, char *filename, struct GrepData data,
                regex_t *regexes, int count_f) {
  char *str = NULL;
  size_t size = 0;
  int exit_code = 0;
  int count_match = 0;
  int line = 0;
  int match_detection = 0;
  while (!match_detection && getline(&str, &size, file) != -1) {
    int is_match = 0;
    line++;
    for (int i = 0; i < data.patterns_count && !is_match; i++) {
      is_match = regexec(&regexes[i], str, 0, NULL, 0) == 0;
    }
    if (data.flags.invert) {
      is_match = !is_match;
    }
    if (is_match) {
      count_match++;
      if (data.flags.files_match) {
        match_detection = 1;
      }
      if (!data.flags.count && !data.flags.files_match) {
        handle_match_not(data, str, filename, regexes, count_f, line,
                         &exit_code);
      }
    }
  }
  if (data.flags.count) {
    if (count_f > 1 && !data.flags.no_filename) {
      if (printf("%s:", filename) < 0) exit_code = 1;
    }
    if (printf("%d\n", count_match) < 0) exit_code = 1;
  }

  if (match_detection) {
    if (printf("%s\n", filename) < 0) exit_code = 1;
  }
  free(str);
  if (ferror(file)) {
    exit_code = 1;
  }
  return exit_code;
}