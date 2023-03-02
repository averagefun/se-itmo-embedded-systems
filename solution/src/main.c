/// @file 
/// @brief Main application file

#include <stdio.h>

/// Application name string
#define APP_NAME "section-extractor"

/// @brief Print usage test
/// @param[in] f File to print to (e.g., stdout)
void usage(FILE *f)
{
  fprintf(f, "Usage: " APP_NAME " <in_file> <section_name> <out_file>\n");
}

/// @brief Application entry point
/// @param[in] argc Number of command line arguments
/// @param[in] argv Command line arguments
/// @return 0 in case of success or error code
int main(int argc, char** argv)
{
  (void) argc; (void) argv; // supress 'unused parameters' warning

  usage(stdout);

  return 0;
}
