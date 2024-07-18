#ifndef S21_CAT_H
#define S21_CAT_H

void reading_file(char *name, char flag);
void print_file(int argc, char **argv, int file_start_index, char flag,
                int *incorrect_flag);
void flag_search(int argc, char **argv, int *file_start_index, char *flag,
                 int *incorrect_flag, int *number_on_flags);
void flags_comparsion(char *flag, char *stdin_flag, int *incorrect_flag,
                      int *number_on_flags);
void print_symbols(int *c, char flag, char *tmp, int *index, int *flag_print);
void flag_b(int *c, char *tmp, int *index);
void flag_n(char *tmp, int *index);
void flag_s(int *c, int *index, int *flag_print);
void flag_E(int *c);
void flag_T(int *c);
void flag_v(int *c);
void flag_e(int *c);
void flag_t(int *c);

#endif