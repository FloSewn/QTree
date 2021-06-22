#pragma once

#include <list>

#include "Simplex.h"
#include "app_util.h"

/***********************************************************
* This class represents a two-dimensional vertex
***********************************************************/
class Edge : public Simplex
{
public:
  Edge(const Vertex* v1, const Vertex* v2) 
  : Simplex()
  , v1_ {v1}
  , v2_ {v2}
  { xy_ = 0.5 * (v1_->xy() + v2_->xy()); }

  ~Edge() = default;

  /*--------------------------------------------------------
  | Getters
  --------------------------------------------------------*/
  const Vec2d& xy() const override { return xy_; }

  /*--------------------------------------------------------
  | Ostream of edge
  --------------------------------------------------------*/
  std::ostream& print(std::ostream& os) const override
  {
    return os << *v1_ << " -> " << *v2_;
  }

private:

  const Vertex* v1_;
  const Vertex* v2_;

  Vec2d xy_ {0.0, 0.0};

}; // Edge
