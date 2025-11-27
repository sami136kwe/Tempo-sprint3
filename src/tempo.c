#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timeseries.h"
#include "validation.h"

// Constants
// ---------

#define HELP "Usage: tempo SUBCOMMAND [options]\n\
Displays information about a timeseries.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  describe: describes the timeseries\n\
  help: shows this message\n\
  interpolate: list the interpolations of the timeseries\n\
    -s|--step DURATION: the duration of each interpolation step\n\
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

// Subcommands
// -----------

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
 * Retrieves the interpolation step from argc and argv
 *
 * @param argc  The argument count
 * @param argv  The argument values
 * @return      The interpolation step
 */
unsigned int interpolation_step(int argc, char* argv[]) {
  if (argc == 2)
    return 1;
  if (argc >= 3 && strcmp(argv[2], "-s") != 0 && strcmp(argv[2], "--step") != 0)
    report_wrong_option("-s or --step");
  if (argc >= 5)
    report_too_many_arguments(argv[1]);
  unsigned int step;
  const char* string_duration = argv[3];
  char unit;
  if (sscanf(string_duration, "%u%c", &step, &unit) != 2 ||
      (unit != 's' && unit != 'm' && unit != 'h'))
    report_invalid_format_duration(string_duration);
  if (unit == 'm')
    step *= 60;
  else if (unit == 'h')
    step *= 3600;
  return step;
}

/**
 * Runs the 'interpolate' subcommand
 *
 * @param step  The interpolation step (in seconds)
 */
void run_interpolate(unsigned int step) {
  struct Timeseries timeseries;
  timeseries_initialize_from_stdin(&timeseries);
  timeseries_print_interpolations(&timeseries, step);
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
    if (strcmp(subcommand, "describe") == 0) {
      check_if_too_many_arguments(argc, "describe");
      run_describe();
    } else if (strcmp(subcommand, "help") == 0) {
      check_if_too_many_arguments(argc, "help");
      printf(HELP);
    } else if (strcmp(subcommand, "interpolate") == 0) {
      unsigned int step = interpolation_step(argc, argv);
      run_interpolate(step);
    } else if (strcmp(subcommand, "show") == 0) {
      check_if_too_many_arguments(argc, "show");
      run_show();
    } else {
      report_unrecognized_subcommand(subcommand);
    }
    return 0;
  }
  fprintf(stderr, "error: subcommand is mandatory\n");
  return 1;
}
