#include <string.h>

#include "s21_cat.h"

void long_flags(char *arg, int *stop_flag, int *exit_code, char *program,
                struct Flags *flags) {
  if (strcmp(arg, "--number-nonblank") == 0) {
    flags->number_nonblank = 1;
    flags->number = 0;
  } else if (strcmp(arg, "--number") == 0) {
    if (flags->number_nonblank != 1) {
      flags->number = 1;
    }
  } else if (strcmp(arg, "--squeeze-blank") == 0) {
    flags->squeeze_blank = 1;
  } else {
    fprintf(stderr, "%s: unrecognized option '%s'\n", program, arg);
    *stop_flag = 1;
    *exit_code = 1;
  }
}

void short_flags(char *arg, int *stop_flag, int *exit_code, char *program,
                 struct Flags *flags) {
  for (int j = 1; arg[j] != '\0' && !(*stop_flag); j++) {
    if (arg[j] == 'b') {
      flags->number_nonblank = 1;
      flags->number = 0;
    } else if (arg[j] == 'e') {
      flags->ends = 1;
      flags->v = 1;
    } else if (arg[j] == 'E') {
      flags->ends = 1;
    } else if (arg[j] == 'n') {
      if (flags->number_nonblank != 1) {
        flags->number = 1;
      }
    } else if (arg[j] == 's') {
      flags->squeeze_blank = 1;
    } else if (arg[j] == 't') {
      flags->tabs = 1;
      flags->v = 1;
    } else if (arg[j] == 'T') {
      flags->tabs = 1;
    } else if (arg[j] == 'v') {
      flags->v = 1;
    } else {
      fprintf(stderr, "%s: invalid option -- '%c'\n", program, arg[j]);
      *stop_flag = 1;
      *exit_code = 1;
    }
  }
}