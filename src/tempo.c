#include <stdio.h>
#include <string.h>

#include "timeseries.h"

// Constants
// ---------

#define HELP "Usage: tempo SUBCOMMAND\n\
Displays information about a timeseries.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  describe: describes the timeseries\n\
  help: shows this message\n"

// Help functions
// --------------

/**
 * Runs the 'describe' subcommand
 */
void run_describe(void) {
  struct Timeseries timeseries;
  timeseries_initialize(&timeseries);
  timeseries_set_start_datetime_from_stdin(&timeseries);
  timeseries_set_observations_from_stdin(&timeseries);
  timeseries_print_stats(&timeseries);
}

/**
 * Runs the 'show' subcommand
 */
void run_show(void) {
  struct Timeseries timeseries;
  timeseries_initialize(&timeseries);
  timeseries_set_start_datetime_from_stdin(&timeseries);
  timeseries_set_observations_from_stdin(&timeseries);
  timeseries_print_observations(&timeseries);
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
  if (argc == 2) {
    const char* subcommand = argv[1];
    if (strcmp(subcommand, "describe") == 0)
      run_describe();
    else if (strcmp(subcommand, "help") == 0)
      printf(HELP);
    else if (strcmp(subcommand, "show") == 0)
      run_show();
    return 0;
  }
  fprintf(stderr, "error: subcommand is mandatory\n");
  return 1;
}
