#include <stdio.h>
#include <string.h>

// Constants
// ---------

#define HELP "Usage: tempo SUBCOMMAND\n\
Displays information about a timeseries.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  describe: describes the timeseries\n\
  help: shows this message\n"

// The maximum length of a read line
#define LINE_MAX_LENGTH 100
// The length of a datetime
#define DATETIME_LENGTH 19

// Types
// -----

// A timeseries
struct Timeseries {
  // The start datetime
  char start_datetime[DATETIME_LENGTH + 1];
};

// Functions
// ---------

/**
 * Set the start datetime of the timeseries from stdin
 *
 * @param timeseries  The timeseries to set
 */
void set_start_datetime_from_stdin(struct Timeseries* timeseries) {
  char line[LINE_MAX_LENGTH];
  if (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    strncpy(timeseries->start_datetime, line, DATETIME_LENGTH);
  }
}

/**
 * Runs the 'describe' subcommand
 */
void run_describe(void) {
  int num_lines = 0;
  struct Timeseries timeseries;
  set_start_datetime_from_stdin(&timeseries);
  char line[LINE_MAX_LENGTH];
  while (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    ++num_lines;
  }
  printf("Range: [%s, %s]\n",
         timeseries.start_datetime, timeseries.start_datetime);
  printf("Size: %d\n", num_lines);
}

/**
 * Runs the 'show' subcommand
 */
void run_show(void) {
  int num_lines = 0;
  struct Timeseries timeseries;
  set_start_datetime_from_stdin(&timeseries);
  char line[LINE_MAX_LENGTH];
  while (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    ++num_lines;
  }
  if (num_lines == 1)
    printf("%s 10\n", timeseries.start_datetime);
}

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
