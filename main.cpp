#include "grid.h"
#include "read.h"
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// search methods go here
#include "astar.h"

point randomPassablePoint(const grid *g) {
  unsigned x[point::dim];
  do {
    for (unsigned i = 0; i < point::dim; ++i) {
      x[i] = random() % g->max(0);
    }
  } while (!g->isPassable(point(x[0], x[1], x[2])));
  return point(x[0], x[1], x[2]);
}


int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "USAGE: %s filename\n"
        "Reads 'filename' to get the map.\n"
        "Format is: one line with XX YY ZZ\n"
        "Followed by one page for each z value.\n"
        "Each page has one line for each y value.\n"
        "Each line has XX characters.  1, dot, and space are dug out;\n"
        "everything else is a wall.\n", argv[0]);
    exit (1);
  }

  char *filename = argv[1];
  grid *g = readMap(filename);

#if 0
  // spit the file out; TODO: do something useful for a change!
  printf("%u %u %u\n", g->max(0), g->max(1), g->max(2));
  for (unsigned z = 0; z < g->max(2); ++z) {
    printf("\n;; level %u\n\n", z);
    for (unsigned y = 0; y < g->max(1); ++y) {
      for (unsigned x = 0; x < g->max(0); ++x) {
        putchar(g->canPass(point(x, y, z)) ? '.' : '#');
      }
      putchar('\n');
    }
  }
#endif

  // test my implementation of A* with a euclidean heuristic
  // TODO: choose a better set of start/end points than random!
  uint64_t Nastar = 0;
  GridGraph graph (g);
  AStar<GridGraph,L1Heuristic> astar (&graph);

  for (unsigned i = 0; i < 500; ++i) {
    point s = randomPassablePoint(g);
    point t = randomPassablePoint(g);

    g->zeroCount();
    astar.path(s, t);
    Nastar += g->getCount();
    g->zeroCount();
    /* another search method here... */
  }
  printf("A* read %llu cells\n", Nastar);

  return 0;
}
