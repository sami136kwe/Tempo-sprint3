#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timeseries.h"
#include "validation.h"

#include "json_parser.h"

// Constants
// ---------

#define HELP "Usage: tempo SUBCOMMAND [options]\n\
Displays information about a timeseries.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  describe: describes the timeseries\n\
    -J|--from-json: read timeseries from JSON format\n\
  help: shows this message\n\
  interpolate: list the interpolations of the timeseries\n\
    -J|--from-json: read timeseries from JSON format\n\
    -s|--step DURATION: the duration of each interpolation step\n\
  show: list the observations of the timeseries\n\
    -J|--from-json: read timeseries from JSON format\n\
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
           VALUE is an integer\n\
\n\
When using -J|--from-json, the input must be a JSON object with:\n\
  - \"origin\": a string with format YYYY-mm-DDTHH:MM:SS\n\
  - \"observations\": an array of objects with \"offset\" and \"value\" fields\n"

// Subcommands
// -----------

/**
 * Checks if the JSON option is present in argv
 *
 * @param argc          The argument count
 * @param argv          The argument values
 * @param start_index   The index to start searching from
 * @return              1 if the JSON option is present, 0 otherwise
 */
int has_json_option(int argc, char* argv[], int start_index) {
  for (int i = start_index; i < argc; i++) {
    if (strcmp(argv[i], "-J") == 0 || strcmp(argv[i], "--from-json") == 0) {
      return 1;
    }
  }
  return 0;
}

/**
 * Runs the 'describe' subcommand
 */
void run_describe(int from_json) {
  struct Timeseries timeseries;
  if (from_json) {
    json_parse_timeseries_from_stdin(&timeseries);
  } else {
    timeseries_initialize_from_stdin(&timeseries);
  }
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
unsigned int interpolation_step(int argc, char* argv[], int* from_json) {
  unsigned int step = 1;
  *from_json = 0;
  
  int i = 2;
  while (i < argc) {
    if (strcmp(argv[i], "-J") == 0 || strcmp(argv[i], "--from-json") == 0) {
      *from_json = 1;
      i++;
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--step") == 0) {
      if (i + 1 >= argc) {
        report_wrong_option("-s or --step requires an argument");
      }
      const char* string_duration = argv[i + 1];
      char unit;
      if (sscanf(string_duration, "%u%c", &step, &unit) != 2 ||
          (unit != 's' && unit != 'm' && unit != 'h')) {
        report_invalid_format_duration(string_duration);
      }
      if (unit == 'm') {
        step *= 60;
      } else if (unit == 'h') {
        step *= 3600;
      }
      i += 2;
    } else {
      report_wrong_option("-s/--step or -J/--from-json");
    }
  }
  
  return step;
}


/**
 * Runs the 'interpolate' subcommand
 *
 * @param step  The interpolation step (in seconds)
 */
void run_interpolate(unsigned int step, int from_json) {
  struct Timeseries timeseries;
  if (from_json) {
    json_parse_timeseries_from_stdin(&timeseries);
  } else {
    timeseries_initialize_from_stdin(&timeseries);
  }
  timeseries_print_interpolations(&timeseries, step);
  timeseries_delete(&timeseries);
}

/**
 * Runs the 'show' subcommand
 */
void run_show(int from_json) {
  struct Timeseries timeseries;
  if (from_json) {
    json_parse_timeseries_from_stdin(&timeseries);
  } else {
    timeseries_initialize_from_stdin(&timeseries);
  }
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
  if (argc < 2) {
    report_subcommand_is_mandatory();
  }
  const char* subcommand = argv[1];
  if (strcmp(subcommand, "describe") == 0) {
    int from_json = has_json_option(argc, argv, 2);
    if (from_json && argc > 3) {
      report_too_many_arguments("describe");
    } else if (!from_json && argc > 2) {
      report_too_many_arguments("describe");
    }
    run_describe(from_json);
  } else if (strcmp(subcommand, "help") == 0) {
    check_if_too_many_arguments(argc, "help");
    printf(HELP);
  } else if (strcmp(subcommand, "interpolate") == 0) {
    int from_json;
    unsigned int step = interpolation_step(argc, argv, &from_json);
    run_interpolate(step, from_json);
  } else if (strcmp(subcommand, "show") == 0) {
    int from_json = has_json_option(argc, argv, 2);
    if (from_json && argc > 3) {
      report_too_many_arguments("show");
    } else if (!from_json && argc > 2) {
      report_too_many_arguments("show");
    }
    run_show(from_json);
  } else {
    report_unrecognized_subcommand(subcommand);
  }
  
  return 0;
}
