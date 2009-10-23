#ifndef astar_HEADER
#define astar_HEADER

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "point.h"
#include "graph.h"
#include "heuristics.h"

#include <vector>

template <class Graph, class Heuristic>
struct AStar {
  typedef typename Graph::vertex vertex;
  typedef typename Graph::iterator iterator;

  AStar(const Graph *G): G_(G) { }

  unsigned path (vertex s, vertex t) {
    /* TODO!  use a hashtable and fib heap instead of vector! */
    std::vector<entry> fringe, visited;
    typedef typename std::vector<entry>::iterator fringe_iterator;

    fringe.push_back(entry(s, 0));
    while (!fringe.empty()) {

      /* Get the min-weight element off the fringe. */
      unsigned best_i = 0;
      for (unsigned i = 1; i < fringe.size(); ++i) {
        if (fringe[i].value(t) < fringe[best_i].value(t)) {
          best_i = i;
        }
      }
      entry e = fringe[best_i];
      fringe[best_i] = fringe.back();
      fringe.pop_back();

      /* if it's the destination, congratuations, we win a prize! */
      if (e.v_ == t) { return e.cost_; }

      /* mark it visited if not already visited */
      if (std::find(visited.begin(), visited.end(), e) == visited.end()) {
        visited.push_back(e);
      } else {
        continue;
      }

      // relax neighbours
      iterator end = G_->end(e.v_);
      for (iterator nit = G_->begin(e.v_); nit != end; ++nit) {
        vertex neigh = *nit;
        entry eneigh (neigh, e.cost_ + G_->weight(neigh));

        fringe_iterator it;

        // try to find neigh in the fringe
        it = std::find(fringe.begin(), fringe.end(), eneigh);
        if (it != fringe.end()) {
          /* it's here, we just haven't gotten there yet; decrease_key
             if applicable */
          if (eneigh.cost_ < (*it).cost_) {
            (*it).cost_ = eneigh.cost_;
          }
          continue;
        }

        // try to find ey in the visited set
        it = std::find(visited.begin(), visited.end(), eneigh);
        if (it != visited.end()) {
          /* it's here; if we have a new, better path, then
             we seach from that path again.  Otherwise just toss ey. */
          if (eneigh.cost_ < (*it).cost_) {
            // try again with the new, shorter path
            fringe.push_back(eneigh);
            (*it).cost_ = eneigh.cost_;
          }
          continue;
        }

        /* ey was found neither in the fringe nor in visited; add it
           to the fringe */
        fringe.push_back(eneigh);
      } // end loop over neighbours
    } // end loop over fringe
    return std::numeric_limits<int>::max();
  } // end function path()

  private:
  struct entry {
    vertex v_;
    unsigned cost_;

    entry(vertex v, unsigned cost) : v_(v), cost_(cost) { }

    unsigned value(vertex t) {
      return cost_ + Heuristic() (v_, t);
    }

    bool operator == (const entry& other) const {
      return v_ == other.v_;
    }
  };

  const Graph *G_;
};

#endif
