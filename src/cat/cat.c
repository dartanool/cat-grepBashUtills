#include "cat.h"

arguments argument_parsing(int argc, char** argv) {
  arguments arg = {0};

  struct option long_options[] = {
      {"number", no_argument, NULL, 'n'},
      {"number-nonblank", no_argument, NULL, 'b'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0},
  };

  int option_index;

  int opt;
  while ((opt = getopt_long(argc, argv, "beEnsvtT", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        arg.b = 1;
        break;
      case 'v':
        arg.v = 1;
        break;
      case 'e':
        arg.e = 1;
        arg.v = 1;
        break;
      case 'E':
        arg.E = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 't':
        arg.t = 1;
        arg.v = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case 'T':
        arg.T = 1;
        break;
      case '?':
        perror("error");
        exit(1);
        break;
      default:
        break;
    }
  }
  if (optind >= argc) {
    fprintf(stderr, "Provide filename\n");
    exit(1);
  }

  return arg;
}
int output(arguments* arg, const char* filename) {
  FILE* file = fopen(filename, "r");

  int ch, count = 1, blank_count = 0, pr = '\n';

  if (file == NULL) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
    return 1;
  }

  while ((ch = fgetc(file)) != EOF) {
    if (arg->s) {
      if (pr == '\n' && ch == '\n') {
        blank_count++;
        if (blank_count > 1) {
          continue;
        }
      } else {
        blank_count = 0;
      }
    }
    if (arg->b) {
      if (pr == '\n' && ch != '\n') {
        fprintf(stdout, "%6d\t", count);
        count++;
      }
    } else if (arg->n) {
      if (pr == '\n') {
        fprintf(stdout, "%6d\t", count);
        count++;
      }
    }
    if (arg->e || arg->E) {
      if (ch == '\n') {
        fputc('$', stdout);
      }
    }
    if (arg->t || arg->T) {
      if (ch == '\t') {
        fputc('^', stdout);
        ch = 'I';
      }
    }
    if (arg->v) {
      if ((ch > 0 && ch <= 9) || (ch > 10 && ch < 32)) {
        fputc('^', stdout);
        ch += 64;
      } else if (ch == 127) {
        fputc('^', stdout);
        ch = 63;
      }
    }

    fputc((char)ch, stdout);
    pr = ch;
  }
  fclose(file);
  return 0;
}

int main(int argc, char* argv[]) {
  arguments arg = argument_parsing(argc, argv);
  int has_error = 0;

  for (int i = optind; i < argc; i++) {
    if (output(&arg, argv[i]) != 0) {
      has_error = 1;
    }
  }

  return has_error;
}