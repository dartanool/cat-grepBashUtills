#ifndef SRC_CAT_CAT_H_
#define SRC_CAT_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b;
  int n;
  int s;
  int E;
  int e;
  int t;
  int T;
  int v;
} arguments;

arguments argument_parsing(int argc, char** argv);
int output(arguments* arg, const char* filename);

#endif  // SRC_CAT_CAT_H_
