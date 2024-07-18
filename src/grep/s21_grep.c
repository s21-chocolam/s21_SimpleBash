#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    char *templates = NULL;
    options opts = {0};

    for (int i = get_options(argc, argv, &opts, &templates); i < argc; i++) {
      pattern_search_in_file(argv[i], opts, templates);
    }
    if (templates != NULL) {
      free(templates);
    }
  } else {
    fprintf(stderr, "Usage: ./s21_grep [OPTION]... PATTERNS [FILE]...\n");
  }
  return 0;
}

int get_options(int argc, char **argv, options *opts, char **templates) {
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        output_with_flag_e(opts, templates, optarg);
        break;
      case 'i':
        opts->i = 1;
        break;
      case 'v':
        opts->v = 1;
        break;
      case 'c':
        opts->c = 1;
        break;
      case 'l':
        opts->l = 1;
        break;
      case 'n':
        opts->n = 1;
        break;
      case 'h':
        opts->h = 1;
        break;
      case 's':
        opts->s = 1;
        break;
      case 'f':
        output_with_flag_f(opts, templates, optarg);
        break;
      case 'o':
        opts->o = 1;
        break;
    }
  }
  if ((opts->e == 1 || opts->f == 1) == 0) {
    get_templates(templates, argv[optind]);
    optind += 1;
  }
  check_match_flag(opts, argc);
  return optind;
}

void pattern_search_in_file(char *files, options opts, char *templates) {
  FILE *file = fopen(files, "r");
  if (file != NULL) {
    char sym = '0';
    while ((sym = getc(file)) != EOF) {
      char *line = calloc(256, 1);
      opts.line_number += 1;
      int length = 0;

      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0) {
          line = increase_length(line, length + 256);
        }
      }

      pattern_search(line, &opts, templates, files);
      free(line);
    }
    output_with_flags_c_l(opts, files);
    fclose(file);
  } else if (opts.s == 0) {
    fprintf(stderr, "grep: %s: No such file or directory\n", files);
  }
}

void output_with_flag_f(options *opts, char **templates, char *optarg) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    char sym = '0';
    while ((sym = getc(file)) != EOF) {
      int length = 0;
      char *line = calloc(256, 1);

      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0)
          line = (char *)increase_length(line, length + 256);
      }

      if (!(opts->e == 1 || opts->f == 1))
        opts->f = get_templates(templates, line);
      else
        add_templates(templates, line);
      free(line);
    }
    fclose(file);
  } else if (opts->s == 0) {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
  }
}

void output_with_flag_e(options *opts, char **templates, char *optarg) {
  if (!(opts->e == 1 || opts->f == 1)) {
    opts->e = get_templates(templates, optarg);
  } else {
    add_templates(templates, optarg);
  }
}

void output_with_flags_c_l(options opts, char *files) {
  if (opts.c == 1) {
    if (opts.l == 1) {
      printf("%s\n", files);
    } else {
      if (opts.files_count > 1) {
        printf("%s:", files);
      }
      printf("%i\n", opts.match_count);
    }
  } else if (opts.l == 1) {
    printf("%s\n", files);
  }
}

void pattern_search(char *line, options *opts, char *templates, char *files) {
  regex_t regex;
  if (regcomp(&regex, templates, opts->i ? REG_ICASE : REG_EXTENDED) == 0) {
    if (regexec(&regex, line, 0, NULL, 0) == opts->v) {
      opts->match_count += 1;
      if ((opts->c == 1 || opts->l == 1) == 0) {
        if (opts->files_count > 1 && !(opts->h)) {
          printf("%s:", files);
        }
        if (opts->n) {
          printf("%i:", opts->line_number);
        }
        if (!opts->o) {
          printf("%s\n", line);
        } else {
          output_with_flag_o(regex, line, *opts);
        }
      }
    }
    regfree(&regex);
  }
}

void output_with_flag_o(regex_t regex, char *line, options opts) {
  regmatch_t pmatch[opts.match_count];
  if (regexec(&regex, line, opts.match_count, pmatch, 0) == 0) {
    for (int i = 0; i < opts.match_count; i++) {
      if (pmatch[i].rm_so != -1 && pmatch[i].rm_eo != -1) {
        int len = pmatch[i].rm_eo - pmatch[i].rm_so;
        char match[len + 1];
        strncpy(match, &line[pmatch[i].rm_so], len);
        match[len] = '\0';
        printf("%s\n", match);
      }
    }
  }
}

void check_match_flag(options *opts, int argc) {
  if (opts->o == 1 && opts->v == 1) {
    if (opts->l == 1 || opts->c == 1) {
      opts->o = 0;
      opts->files_count = argc - optind;
    } else {
      optind = argc;
    }
  } else {
    opts->files_count = argc - optind;
  }
}

int get_templates(char **str, char *optarg) {
  *str = calloc(strlen(optarg) + 1, 1);
  int flag = 0;
  if (*str != NULL) {
    strcpy(*str, optarg);
    flag = 1;
  }
  return flag;
}

void add_templates(char **str, char *optarg) {
  *str = increase_length(*str, strlen(*str) + strlen(optarg) + 2);
  if (*str != NULL) {
    strcat(strcat(*str, "|"), optarg);
  }
}

void *increase_length(void *str, int size) {
  char *c = realloc(str, size);
  return c;
}
