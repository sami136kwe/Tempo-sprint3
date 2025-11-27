#include "validation.h"

#include <stdio.h>
#include <stdlib.h>

void report_memory_problem(const char* context) {
  fprintf(stderr, "error: memory allocation failed %s\n", context);
  exit(1);
}

void report_invalid_datetime(int year, int month, int day,
                             int hour, int min, int sec) {
  fprintf(stderr,
          "error: invalid datetime (%.4d-%.2d-%.2dT%.2d:%.2d:%.2d)\n", 
          year, month, day, hour, min, sec);
  exit(2);
}

void report_time_conversion_error(time_t t) {
  fprintf(stderr, "error: could not convert timestamp %ld to struct tm\n", t);
  exit(-1);
}
