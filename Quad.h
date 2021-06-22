#pragma once
#include <raylib.h>

#include "Vec2.h"
#include "app_util.h"
#include "Facet.h"

/***********************************************************
* A simple triangle
***********************************************************/
class Quad : public Facet
{
public:

  Quad() {}

  Quad(const Vertex* v1, const Vertex* v2, 
       const Vertex* v3, const Vertex* v4) 
    : v1_ {v1}, v2_ {v2}
    , v3_ {v3}, v4_ {v4}
  { 0.25*(v1_->xy() + v2_->xy() + v3_->xy() + v4_->xy()); }

  ~Quad() = default;

  /*--------------------------------------------------------
  | Getters
  --------------------------------------------------------*/
  const Vec2d& xy() const override { return xy_; }

  /*--------------------------------------------------------
  | Ostream of triangle
  --------------------------------------------------------*/
  std::ostream& print(std::ostream& os) const override
  {
    return os << *v1_ << " -> " << *v2_ << " -> " 
              << *v3_ << " -> " << *v4_;
  }

private:

  const Vertex* v1_;
  const Vertex* v2_;
  const Vertex* v3_;
  const Vertex* v4_;

  Vec2d xy_ { 0.0f, 0.0f };

}; // Quad
