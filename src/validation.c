#include "validation.h"

#include <stdio.h>
#include <stdlib.h>

// Memory
// ------

void report_memory_problem(const char* context) {
  fprintf(stderr, "error: memory allocation failed %s\n", context);
  exit(1);
}

// Datetimes
// ---------

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

// CLI
// ---

void report_unrecognized_subcommand(const char* subcommand) {
  fprintf(stderr, "error: unrecognized subcommand '%s'", subcommand);
  exit(1);
}

void report_too_many_arguments(const char* subcommand) {
  fprintf(stderr, "error: too many arguments to '%s' subcommand", subcommand);
  exit(1);
}

void check_if_too_many_arguments(int argc, const char* subcommand) {
  if (argc >= 3)
    report_too_many_arguments(subcommand);
}

void report_wrong_option(const char* option) {
  fprintf(stderr, "error: expected %s option\n", option);
  exit(1);
}

void report_invalid_format_duration(const char* s) {
  fprintf(stderr, "error: invalid duration format (%s)\n", s);
  exit(1);
}
