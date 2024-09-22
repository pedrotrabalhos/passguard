#include "clipboard.h"

#include <stdio.h>
#include <stdlib.h>

void clipboard_copy(const char *text) {
  // Open a pipe to xclip with the -selection clipboard option
  FILE *pipe = popen("xclip -selection clipboard", "w");
  if (pipe == NULL) {
    fprintf(stderr, "Failed to open pipe to xclip.\n");
    return;
  }

  // Write the text to xclip
  fprintf(pipe, "%s", text);

  // Close the pipe
  pclose(pipe);
}
