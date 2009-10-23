#ifndef grid_HEADER
#define grid_HEADER

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "point.h"

#include <assert.h>
#include <stdlib.h>

struct grid {
  grid(point maxcorner)
    : maxcorner(maxcorner), count(0) {
      passable = (bool*)calloc(maxcorner[0] * maxcorner[1] * maxcorner[2],
          sizeof(bool));
    }
  ~grid() { free(passable); }

  bool isPassable(point p) const {
    ++count;
    return inBounds(p) && passable[index(p)];
  }

  void dig(point p) {
    passable[index(p)] = true;
  }

  unsigned max(unsigned i) const {
    return maxcorner[i];
  }

  void zeroCount() { count = 0; }
  unsigned getCount() const { return count; }

  private:
  bool inBounds(point p) const {
    for (unsigned i = 0; i < 3; ++i) {
      if (p[i] >= maxcorner[i]) { return false; }
    }
    return true;
  }
  unsigned index(point p) const {
    assert(inBounds(p));
    return p[0] + maxcorner[0] * p[1]
      + maxcorner[0] * maxcorner[1] * p[2];
  }

  point maxcorner;
  bool *passable; // 1 => passable
  mutable unsigned count;
};

#endif
