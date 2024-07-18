#include "s21_cat.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  char flag = '0';
  int file_start_index = 1;
  int incorrect_flag = 0;
  int number_on_flags = 0;
  flag_search(argc, argv, &file_start_index, &flag, &incorrect_flag,
              &number_on_flags);
  print_file(argc, argv, file_start_index, flag, &incorrect_flag);
  return 0;
}

void flag_search(int argc, char **argv, int *file_start_index, char *flag,
                 int *incorrect_flag, int *number_on_flags) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      *file_start_index += 1;
      flags_comparsion(flag, argv[i], incorrect_flag, number_on_flags);
    } else {
      break;
    }
  }
}

struct str_flags {
  char flag;
  char *gnu_flag1;
  char *gnu_flag2;
  char *gnu_flag3;
};

void flags_comparsion(char *flag, char *stdin_flag, int *incorrect_flag,
                      int *number_on_flags) {
  struct str_flags flags[8] = {{'b', "-b", "--number-nonblank", "-b"},
                               {'E', "-E", "-E", "-E"},
                               {'e', "-Ev", "-vE", "-e"},
                               {'n', "-n", "--number", "-n"},
                               {'s', "-s", "--squeeze-blank", "-s"},
                               {'T', "-T", "-T", "-T"},
                               {'t', "-Tv", "-vT", "-t"},
                               {'v', "-v", "-v", "-v"}};

  for (int i = 0; i < 8; i++) {
    if (strcmp(flags[i].gnu_flag1, stdin_flag) == 0 ||
        strcmp(flags[i].gnu_flag2, stdin_flag) == 0 ||
        strcmp(flags[i].gnu_flag3, stdin_flag) == 0) {
      *flag = flags[i].flag;
      *number_on_flags = *number_on_flags + 1;
    }
  }
  if (*flag == '0' || *number_on_flags > 1) {
    fprintf(stderr,
            "non-existent flag or more than one flags, enter cat [-benstv] "
            "[FILE ...]\n");
    *incorrect_flag = 1;
  }
}

void print_file(int argc, char **argv, int file_start_index, char flag,
                int *incorrect_flag) {
  if (*incorrect_flag == 0) {
    for (int i = file_start_index; i < argc; i++) {
      reading_file(argv[i], flag);
    }
  }
}

void reading_file(char *name, char flag) {
  FILE *file = fopen(name, "rt");
  int index = 1;
  int flag_print = 0;
  if (file != NULL) {
    char tmp = '\n';
    int c = fgetc(file);
    while (c != EOF) {
      if (flag == '0') {
        putc(c, stdout);
      } else {
        print_symbols(&c, flag, &tmp, &index, &flag_print);
        if (flag_print == 0) {
          putc(c, stdout);
        }
      }
      tmp = c;
      c = fgetc(file);
    }
    fclose(file);
  } else {
    fprintf(stderr, "cat: %s: No such file or directory\n", name);
  }
}

void print_symbols(int *c, char flag, char *tmp, int *index, int *flag_print) {
  if (flag == 'b') {
    flag_b(c, tmp, index);
  } else if (flag == 'n') {
    flag_n(tmp, index);
  } else if (flag == 's') {
    flag_s(c, index, flag_print);
  } else if (flag == 'E') {
    flag_E(c);
  } else if (flag == 'T') {
    flag_T(c);
  } else if (flag == 'v') {
    flag_v(c);
  } else if (flag == 'e') {
    flag_e(c);
  } else if (flag == 't') {
    flag_t(c);
  }
}

void flag_b(int *c, char *tmp, int *index) {
  if (*c != '\n' && *tmp == '\n') {
    printf("%6d\t", *index);
    *index = *index + 1;
  }
}

void flag_n(char *tmp, int *index) {
  if (*tmp == '\n') {
    printf("%6d\t", *index);
    *index = *index + 1;
  }
}

void flag_s(int *c, int *index, int *flag_print) {
  if (*c == '\n') {
    *index = *index + 1;
  } else {
    *index = 1;
    *flag_print = 0;
  }
  if (*c == '\n' && *index > 3) {
    *flag_print = 1;
  }
}

void flag_E(int *c) {
  if (*c == '\n') {
    printf("$");
  }
}

void flag_T(int *c) {
  if (*c == '\t') {
    printf("^");
    *c = *c + 64;
  }
}

void flag_v(int *c) {
  if (*c == '\0' || *c == '\a' || *c == '\b' || *c == '\v' || *c == '\f' ||
      *c == '\r') {
    printf("^");
    *c = *c + 64;
  }
}

void flag_e(int *c) {
  if (*c == '\n') {
    printf("$");
  } else if (*c == '\0' || *c == '\a' || *c == '\b' || *c == '\v' ||
             *c == '\f' || *c == '\r') {
    printf("^");
    *c = *c + 64;
  }
}

void flag_t(int *c) {
  if (*c == '\t') {
    printf("^");
    *c = *c + 64;
  } else if (*c == '\0' || *c == '\a' || *c == '\b' || *c == '\v' ||
             *c == '\f' || *c == '\r') {
    printf("^");
    *c = *c + 64;
  }
}