#include "read.h"
#include "grid.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static const char *filename;
static FILE *f;
static unsigned lineno;
static char buffer[4096];

static void error(const char *fmt, ...)
{
  char errbuf[10000];
  va_list ap;
  va_start(ap, fmt);
  snprintf(errbuf, sizeof(errbuf), "%s:%u: error: %s\n", filename, lineno, fmt);
  vfprintf(stderr, errbuf, ap);
  va_end(ap);
  exit(1);
}


static void readLine() {
  do {
    if (!fgets(buffer, sizeof(buffer), f)) {
      error("premature end of file");
    }
    ++lineno;
  } while (buffer[0] == '\n' ||
      (buffer[0] == ';' && buffer[1] == ';'));
}



grid *readMap(const char * fname)
{
  filename = fname;
  lineno = 0;
  f = fopen(filename, "r");
  if (!f) {
    fprintf(stderr, "%s: error: failed to open for some reason\n", filename);
    exit (1);
  }

  // read the header
  unsigned x, y, z;
  readLine();
  if (3 != sscanf(buffer, "%u %u %u", &x, &y, &z)) {
    error("didn't read three unsigneds in the header");
  }

  grid *g = new grid(point(x, y, z));

  // read the maps, one by one
  for (unsigned k = 0; k < z; ++k) {
    for (unsigned j = 0; j < y; ++j) {
      readLine();
      for (unsigned i = 0; i < x; ++i) {
        if (!buffer[i]) {
          error("premature end of line in column %u", i);
        }
        if (buffer[i] == ' ' || buffer[i] == '.' || buffer[i] == '1') {
          g->dig(point(i, j, k));
        }
      }
    }
  }

  fclose(f);
  return g;
}
