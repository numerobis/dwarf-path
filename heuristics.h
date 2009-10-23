/* Copyright Benoit Hudson 2009 */
#ifndef heuristics_HEADER
#define heuristics_HEADER

#ifdef HAVE_CONFIG_H
 #include <config.h>
#endif

/*
 * Interface is
 * struct H {
 *   unsigned operator (point, point) const;
 * };
 */

#include "point.h"

struct L1Heuristic {
  unsigned operator () (point a, point b) const {
    unsigned sum = 0;
    for (unsigned i = 0; i < point::dim; ++i) {
      sum += abs(a[i] - b[i]);
    }
    return sum;
  }
};

#endif
