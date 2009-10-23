#ifndef point_HEADER
#define point_HEADER

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>

struct point {
  point() { } // uninitialized constructor

  point(unsigned x, unsigned y, unsigned z) {
    x_[0] = x;
    x_[1] = y;
    x_[2] = z;
  }
  static const unsigned dim = 3;

  unsigned operator[] (unsigned i) const {
    assert(i < dim);
    return x_[i];
  }
  void set (unsigned i, unsigned val) {
    assert(i < dim);
    x_[i] = val;
  }

  bool operator == (const point& other) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (x_[i] != other[i]) { return false; }
    }
    return true;
  }

  /* Arbitrary total ordering. */
  bool operator < (const point& other) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (x_[i] < other[i]) { return true; }
      if (x_[i] > other[i]) { return false; }
    }
    return false;
  }


  private:
  unsigned x_[dim];
};

#endif
