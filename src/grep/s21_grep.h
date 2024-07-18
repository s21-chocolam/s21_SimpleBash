#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int files_count;
  int line_number;
  int match_count;
} options;

int get_options(int argc, char **argv, options *opts, char **templates);
void pattern_search(char *str, options *options, char *tmpl, char *name);
void output_with_flag_f(options *opts, char **templates, char *optarg);
void output_with_flag_e(options *opts, char **templates, char *optarg);
void output_with_flag_o(regex_t my_regex, char *str, options opts);
void pattern_search_in_file(char *name, options opts, char *tmpl);
void output_with_flags_c_l(options opts, char *files);
void *increase_length(void *str, int size);
int get_templates(char **str, char *optarg);
void check_match_flag(options *opts, int argc);
void add_templates(char **str, char *optarg);

#endif
