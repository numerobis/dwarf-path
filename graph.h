/* Copyright Benoit Hudson 2009 */
#ifndef graph_HEADER
#define graph_HEADER

#ifdef HAVE_CONFIG_H
 #include <config.h>
#endif

#include <boost/iterator/iterator_facade.hpp>
#include <list>


#include "grid.h"


/** This is actually just a signature.  Don't use it.
 * Algorithms that read a graph can take anything that matches
 * this signature.  AdjacencyList and GridGraph both match.
 */
struct GraphSignature {
  // Whatever type we want.  Must have operator< and operator==.
  typedef int vertex;
  typedef vertex *iterator;

  iterator begin(vertex) const;
  iterator end(vertex) const;
  int weight(vertex);
};

/** This is an abstract graph. */
struct AdjacencyList {
  struct vertex_data;
  typedef const vertex_data *vertex;

  struct vertex_data {
  private:
    friend struct AdjacencyList;
    vertex_data(): weight(0) { }
    vertex_data(int weight): weight(weight) { }

    int getWeight() const { return weight; }

    typedef std::list<vertex>::const_iterator iterator;
    typedef std::list<vertex>::const_iterator const_iterator;

    iterator begin() const { return N.begin(); }
    iterator end() const { return N.end(); }

    private:
    std::list<vertex> N; // or a hash?
    int weight;
  };

  public:
  typedef vertex_data::iterator iterator;
  typedef vertex_data::const_iterator const_iterator;

  iterator begin(vertex v) const { return v->begin(); }
  iterator end(vertex v) const { return v->end(); }

  int weight(vertex v) const {
    return v->getWeight();
  }

  void addNeighbour(vertex v) {
    const_cast<vertex_data*>(v) -> N.push_back(v);
  }
};

struct GridGraph {
  GridGraph (const grid *grid) : grid_(grid) { }

  typedef point vertex;
  static const unsigned dim = 3;

  struct iterator
  : public boost::iterator_facade< iterator, vertex,
             boost::bidirectional_traversal_tag, vertex >
  {
    static const unsigned maxoffset = 3 * 3 * 3; // 3 ** dim

    iterator (const grid *g, point p) : grid_(g), p_(p), i_(0) {
      increment();
    }
    iterator () : i_(maxoffset) { }

    point dereference() const {
      unsigned offset = i_;
      point p;
      assert (offset < maxoffset);
      /* this could be a 26-case switch statement instead of a loop with
       * division in it (case 0 is never activated). */
      for (unsigned j = 0; j < dim; ++j) {
        switch (offset % 3) {
          case 0: p.set(j, p_[j]); break;
          case 1: p.set(j, p_[j] + 1); break;
          case 2: p.set(j, p_[j] - 1); break;
        }
        offset /= 3;
      }
      return p;
    }

    void increment() {
      do {
        ++i_; // skip the current, go to the next
      } while ( (i_ < maxoffset) // next that is past-end or passable
          && (!grid_->isPassable(dereference())));
    }

    void decrement() {
      do {
        --i_;
      } while ( (i_ > 0) // 0 is self, skip it.
          && (!grid_->isPassable(dereference())));
    }

    bool equal (const iterator & other) const {
      return i_ == other.i_;
    }


    private:
    const grid *grid_;
    const point p_;
    unsigned i_;
  };

  iterator begin(vertex v) const { return iterator(grid_, v); }
  iterator end(vertex) const { return iterator(); }
  int weight(vertex) const { return 1; }

  private:
  const grid * grid_;
};


#endif
