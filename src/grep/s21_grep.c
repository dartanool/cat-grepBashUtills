#define _GNU_SOURCE
#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  int error = 0;
  patterns patterns = {};
  flags flags = {};
  regex_t re;

  error = parse_arguments(argc, argv, &patterns, &flags);
  if (argc - optind == 1) flags.h = 1;

  if (!error) {
    if (flags.i) {
      error = regcomp(&re, patterns.buffer, REG_EXTENDED | REG_ICASE);
    } else {
      error = regcomp(&re, patterns.buffer, REG_EXTENDED);
    }
  }

  if (!error) {
    for (int i = optind; i < argc; i++) {
      if (print_grep(flags, argv[i], &re) != 0) {
        error = 1;
      }
    }
    regfree(&re);
  }
  if (patterns.buffer) free(patterns.buffer);

  return error;
}

int parse_arguments(int argc, char** argv, patterns* patterns, flags* flags) {
  int opt, error = 0;

  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && !error) {
    switch (opt) {
      case 'e':
        flags->e = 1;
        error = add_pattern(optarg, patterns, *flags);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        error = add_pattern_from_file(optarg, patterns, *flags);
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        break;
    }
  }

  if (!error) {
    if (patterns->buffer_size == 0 && argv[optind] != NULL) {
      add_pattern(argv[optind], patterns, *flags);
      optind++;
    }
  }

  if (optind >= argc) {
    error = 1;
    if (!flags->s) fprintf(stderr, "s21_grep: Provide filename\n");
  }

  return error;
}

int add_pattern(char* string, patterns* patterns, flags flags) {
  int error = 0, string_len = 0;

  if (string) {
    string_len = strlen(string);
  }
  if (patterns->buffer_size == 0) {
    patterns->buffer = malloc((3 + string_len) * sizeof(char));

    if (patterns->buffer != NULL) {
      patterns->buffer_size = 3 + string_len;
      patterns->buffer[0] = '\0';
      strcat(patterns->buffer, "(");
      strcat(patterns->buffer, string);
      strcat(patterns->buffer, ")");
    } else {
      error = 1;
      if (!flags.s) fprintf(stderr, "Failed to allocate memory\n");
    }
  } else {
    int new_buffer_size = patterns->buffer_size + string_len + 3;
    char* new_buffer = realloc(patterns->buffer, new_buffer_size);
    if (new_buffer != NULL) {
      patterns->buffer = new_buffer;
      patterns->buffer_size = new_buffer_size;
      strcat(patterns->buffer, "|(");
      strcat(patterns->buffer, string);
      strcat(patterns->buffer, ")");
    } else {
      error = 1;
      if (!flags.s) fprintf(stderr, "Failed to allocate memory\n");
    }
  }

  return error;
}

int add_pattern_from_file(char* filename, patterns* patterns, flags flags) {
  int error = 0, line_lenght = 0;
  char* line = NULL;
  size_t n = 0;
  FILE* file;
  file = fopen(filename, "r");
  if (file != NULL) {
    while ((line_lenght = getline(&line, &n, file)) != -1) {
      if (line_lenght > 0 && line[line_lenght - 1] == '\n') {
        line[line_lenght - 1] = '\0';
      }
      if (line) add_pattern(line, patterns, flags);
    }
    if (line) free(line);
    fclose(file);
  } else {
    error = 1;
    if (!flags.s)
      fprintf(stderr, "s21_grep: %s No such file or directory\n", filename);
  }

  return error;
}

int print_grep(flags flags, char* filename, regex_t* re) {
  char* line = NULL;
  int line_lenght = 0, line_count = 0, matches_count = 0, result = 0;
  int status = 0;
  FILE* file = NULL;
  size_t n = 0;

  file = fopen(filename, "r");

  if (file == NULL) {
    if (!flags.s)
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    status = 1;
  } else {
    while ((line_lenght = getline(&line, &n, file)) != -1) {
      result = !regexec(re, line, 0, NULL, 0);
      line_count++;
      matches_count += (result + flags.v) % 2;

      if ((result && !flags.v) || (!result && flags.v)) {
        if (!flags.c && !flags.l) {
          if (flags.o) {
            print_match(flags, re, line, filename, line_count);
          } else {
            if (!flags.h) printf("%s:", filename);
            if (flags.n) printf("%d:", line_count);
            printf("%s", line);
            if (line[line_lenght - 1] != '\n') printf("\n");
          }
        }
      }
    }
    if (flags.c && !flags.l) {
      if (!flags.h) printf("%s:", filename);
      printf("%d\n", matches_count);
    }
    if (flags.l && matches_count > 0) printf("%s\n", filename);
    if (line) free(line);
    fclose(file);
  }

  return status;
}

void print_match(flags flags, regex_t* re, char* line, char* filename,
                 int line_count) {
  regmatch_t match;
  int offset = 0;

  size_t line_len = strlen(line);
  if (line_len > 0 && line[line_len - 1] == '\n') {
    line[line_len - 1] = '\0';
  }

  while (1) {
    if ((long unsigned int)offset > strlen(line)) {
      break;
    }
    int result = regexec(re, line + offset, 1, &match, 0);
    if (result != 0) {
      break;
    }
    if (match.rm_eo != 0) {
      if (!flags.h) printf("%s:", filename);
      if (flags.n) printf("%d:", line_count);

      for (int i = match.rm_so; i < match.rm_eo; i++) {
        putchar(line[offset + i]);
      }
      putchar('\n');
      offset += match.rm_eo;

    } else {
      offset++;
    }
  }
}