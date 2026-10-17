#ifndef S21_GREP_H
#define S21_GREP_H

#define _POSIX_C_SOURCE 200809L
#define MAX_PATTERNS 100

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Flags {
  int invert;
  int count;
  int files_match;
  int number;
  int no_filename;
  int no_error;
  int only_match;
};

struct GrepData {
  struct Flags flags;
  int regex_flags;
  int file_start_index;
  char *pattern[MAX_PATTERNS];
  int patterns_count;
};

void print_perror(char *in, struct Flags flags, int *exit_code);
int print_only_matches(char *str, char *filename, struct GrepData data,
                       regex_t *regexes, int count_f, int line);
int handle_file(FILE *file, char *filename, struct GrepData data,
                regex_t *regexes, int count_f);
void add_pattern(char *pattern, struct GrepData *data, int *exit_code);
void parse_file(char *arg, struct GrepData *data, int *exit_code);
void handle_ef_flags(int argc, char *argv[], struct GrepData *data, int *i,
                     int j, int *exit_code, int *stop_parse, int *stop_flag);
void parse_flags(int argc, char *argv[], int *i, struct GrepData *data,
                 int *stop_flag, int *exit_code);
int compile_regex(regex_t *regexes, struct GrepData data, int *exit_code,
                  int *stop_flag);
void process_file(char *arg, struct GrepData data, regex_t *regexes, int argc,
                  int *exit_code);
void process_stdin(struct GrepData data, regex_t *regexes, int argc,
                   int *exit_code);
void process_inputs(int argc, char *argv[], struct GrepData data,
                    regex_t *regexes, int *exit_code);
int parse_args(int argc, char *argv[], struct GrepData *data, int *stop_flag,
               int *exit_code);
void handle_match_not(struct GrepData data, char *str, char *filename,
                      regex_t *regexes, int count_f, int line, int *exit_code);

#endif