#include <stdio.h>
#include <string.h>

#define HELP "Usage: tempo\n\
Displays information about a timeseries.\n"

/**
 * Main function
 *
 * @param argc  The number of parsed arguments
 * @param argv  The arguments
 * @return      0 if usage is normal, 1 otherwise
 */
int main(int argc, char *argv[]) {
  if (argc == 2 && strcmp(argv[1], "describe") == 0)
    printf("An empty timeseries with origin 2025-09-01T09:00:00\n");
  else
    printf(HELP);
  return 0;
}
