#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timeseries.h"

// Constants
// ---------

#define HELP "Usage: tempo SUBCOMMAND\n\
Displays information about a timeseries.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  describe: describes the timeseries\n\
  help: shows this message\n\
  show: list the observations of the timeseries\n\
\n\
A timeseries is a text stream that must satisfy the following syntax:\n\
\n\
  1. The first line must contain a valid reference datetime with format\n\
       YYYY-mm-DDTHH:MM:SS\n\
     where YYYY are the 4 digits for the year,\n\
           mm are the 2 digits for the month,\n\
           DD are the 2 digits for the day,\n\
           HH are the 2 digits for the hour,\n\
           MM are the 2 digits for the minutes and\n\
           SS are the 2 digits for the seconds\n\
  2. Each of the remaining line must contain an observation, given with the\n\
     format\n\
       OFFSET VALUE\n\
     where OFFSET is a positive integer indicating the number of seconds offset\n\
                  with respect to the reference datetime and\n\
           VALUE is an integer\n"

// Help functions
// --------------

/**
 * Runs the 'describe' subcommand
 */
void run_describe(void) {
  struct Timeseries timeseries;
  timeseries_initialize_from_stdin(&timeseries);
  timeseries_print_stats(&timeseries);
  timeseries_delete(&timeseries);
}

/**
 * Runs the 'show' subcommand
 */
void run_show(void) {
  struct Timeseries timeseries;
  timeseries_initialize_from_stdin(&timeseries);
  timeseries_print_observations(&timeseries);
  timeseries_delete(&timeseries);
}

/**
 * Reports that an unrecognized subcommand has been provided
 *
 * @param subcommand  The provided subcommand
 */
void report_unrecognized_subcommand(const char* subcommand) {
  fprintf(stderr, "error: unrecognized subcommand '%s'", subcommand);
  exit(1);
}

/**
 * Reports that too many arguments have been provided
 *
 * @param subcommand  The provided subcommand
 */
void report_too_many_arguments(const char* subcommand) {
  fprintf(stderr, "error: too many arguments to '%s' subcommand", subcommand);
  exit(1);
}

// Main
// ----

/**
 * Main function
 *
 * @param argc  The number of parsed arguments
 * @param argv  The arguments
 * @return      0 if usage is normal, 1 otherwise
 */
int main(int argc, char *argv[]) {
  if (argc >= 2) {
    const char* subcommand = argv[1];
    if (strcmp(subcommand, "describe") == 0)
      run_describe();
    else if (strcmp(subcommand, "help") == 0) {
      if (argc >= 3)
        report_too_many_arguments("help");
      printf(HELP);
    } else if (strcmp(subcommand, "show") == 0)
      run_show();
    else
      report_unrecognized_subcommand(subcommand);
    return 0;
  }
  fprintf(stderr, "error: subcommand is mandatory\n");
  return 1;
}
