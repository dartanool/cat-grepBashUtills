#ifndef S21_GREP_H
#define S21_GREP_H

#include <regex.h>
#include <stdio.h>

typedef struct {
  int e, i, v, c, l, n, h, s, f, o;
} flags;

typedef struct {
  char* buffer;
  int buffer_size;
} patterns;

int parse_arguments(int argc, char** argv, patterns* pattern, flags* flags);
int add_pattern(char* string, patterns* patterns, flags flags);
int add_pattern_from_file(char* filename, patterns* patterns, flags flags);
int print_grep(flags flags, char* filename, regex_t* re);
void print_match(flags flags, regex_t* re, char* line, char* filename,
                 int line_count);

#endif