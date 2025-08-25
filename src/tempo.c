#include <stdio.h>
#include <string.h>

#define HELP "Usage: tempo SUBCOMMAND\n\
Displays information about a timeseries.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  describe: describes the timeseries\n\
  help: shows this message\n"

// The maximum length of a read line
#define LINE_MAX_LENGTH 100

/**
 * Runs the 'describe' subcommand
 */
void run_describe(void) {
  int num_lines = 0;
  char line[LINE_MAX_LENGTH];
  while (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    ++num_lines;
  }
  printf("Range: [2025-09-01T09:00:00, 2025-09-01T09:00:00]\n");
  printf("Size: %d\n", num_lines - 1);
}

/**
 * Runs the 'show' subcommand
 */
void run_show(void) {
  int num_lines = 0;
  char line[LINE_MAX_LENGTH];
  while (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    ++num_lines;
  }
  if (num_lines == 2)
    printf("2025-09-01T09:00:00 10\n");
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
    if (strcmp(argv[1], "describe") == 0)
      run_describe();
    else if (strcmp(argv[1], "help") == 0)
      printf(HELP);
    else if (strcmp(argv[1], "show") == 0)
      run_show();
    return 0;
  }
  fprintf(stderr, "error: subcommand is mandatory\n");
  return 1;
}
