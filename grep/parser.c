#include "s21_grep.h"

void add_pattern(char *pattern, struct GrepData *data, int *exit_code) {
  if (data->patterns_count >= MAX_PATTERNS) {
    fprintf(stderr, "Too many patterns");
    *exit_code = 1;
  } else {
    char *copy = strdup(pattern);
    data->pattern[data->patterns_count] = copy;
    data->patterns_count += 1;
  }
}

void parse_file(char *arg, struct GrepData *data, int *exit_code) {
  FILE *file = fopen(arg, "r");
  char *str = NULL;
  size_t size = 0;
  if (file == NULL) {
    print_perror(arg, data->flags, exit_code);
  } else {
    while (getline(&str, &size, file) != -1) {
      size_t len = strlen(str);

      if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
      }
      add_pattern(str, data, exit_code);
    }

    if (ferror(file)) {
      *exit_code = 1;
    }

    if (fclose(file) != 0) print_perror(arg, data->flags, exit_code);

    free(str);
  }
}

void handle_ef_flags(int argc, char *argv[], struct GrepData *data, int *i,
                     int j, int *exit_code, int *stop_parse, int *stop_flag) {
  if (argv[*i][j + 1] == '\0') {
    if (*i + 1 >= argc) {
      fprintf(stderr, "%s: option requires an argument -- e\n", argv[0]);
      *exit_code = 1;
      *stop_flag = 1;
    } else {
      if (argv[*i][j] == 'e') {
        add_pattern(argv[*i + 1], data, exit_code);
      } else {
        parse_file(argv[*i + 1], data, exit_code);
      }
      *i += 1;
      *stop_parse = 1;
    }
  } else {
    if (argv[*i][j] == 'e') {
      add_pattern(argv[*i] + j + 1, data, exit_code);
    } else {
      parse_file(argv[*i] + j + 1, data, exit_code);
    }
    *stop_parse = 1;
  }
}

void parse_flags(int argc, char *argv[], int *i, struct GrepData *data,
                 int *stop_flag, int *exit_code) {
  for (int j = 1, stop_parse = 0;
       argv[*i][j] != '\0' && !*stop_flag && !stop_parse; j++) {
    if (argv[*i][j] == 'i') {
      data->regex_flags |= REG_ICASE;
    } else if (argv[*i][j] == 'E') {
      data->regex_flags |= REG_EXTENDED;
    } else if (argv[*i][j] == 'e' || argv[*i][j] == 'f') {
      handle_ef_flags(argc, argv, data, i, j, exit_code, &stop_parse,
                      stop_flag);
    } else if (argv[*i][j] == 'o') {
      data->flags.only_match = 1;
    } else if (argv[*i][j] == 'c') {
      data->flags.count = 1;
    } else if (argv[*i][j] == 'n') {
      data->flags.number = 1;
    } else if (argv[*i][j] == 'h') {
      data->flags.no_filename = 1;
    } else if (argv[*i][j] == 'l') {
      data->flags.files_match = 1;
    } else if (argv[*i][j] == 'v') {
      data->flags.invert = 1;
    } else if (argv[*i][j] == 's') {
      data->flags.no_error = 1;
    } else {
      fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], argv[*i][j]);
      *stop_flag = 1;
      *exit_code = 1;
    }
  }
}

int parse_args(int argc, char *argv[], struct GrepData *data, int *stop_flag,
               int *exit_code) {
  for (int i = 1; i < argc && !*stop_flag && !data->file_start_index; i++) {
    if (!data->file_start_index && strcmp(argv[i], "--") == 0) {
      if (i + 1 < argc) {
        add_pattern(argv[i + 1], data, exit_code);
        data->file_start_index = i + 2;
      } else {
        fprintf(stderr,
                "Usage: %s [-cEhilnosv] [-e pattern] [-f file] [pattern] [file "
                "...]\n",
                argv[0]);
        *exit_code = 1;
      }
    } else if (!data->file_start_index && argv[i][0] == '-' &&
               strcmp(argv[i], "-") != 0) {
      parse_flags(argc, argv, &i, data, stop_flag, exit_code);
    } else {
      if (!data->patterns_count) {
        add_pattern(argv[i], data, exit_code);
        data->file_start_index = i + 1;
      } else {
        data->file_start_index = i;
      }
    }
  }
  if (!data->patterns_count) {
    *exit_code = 1;
  }

  return *exit_code;
}