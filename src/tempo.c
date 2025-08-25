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
// A fixed datetime
#define DATETIME "2025-09-01T09:00:00"

/**
 * Runs the 'describe' subcommand
 */
void run_describe(void) {
  int num_lines = 0;
  char line[LINE_MAX_LENGTH];
  while (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    ++num_lines;
  }
  printf("Range: [%s, %s]\n", DATETIME, DATETIME);
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
    printf("%s 10\n", DATETIME);
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
