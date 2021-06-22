#pragma once
#include <raylib.h>

#include "Vec2.h"
#include "app_util.h"
#include "Facet.h"

/***********************************************************
* A simple triangle
***********************************************************/
class Triangle : public Facet
{
public:

  Triangle() {}

  Triangle(const Vertex* v1, 
           const Vertex* v2, 
           const Vertex* v3) 
    : v1_ {v1}
    , v2_ {v2}
    , v3_ {v3}
  { (v1_->xy() + v2_->xy() + v3_->xy()) / 3.0; }

  ~Triangle() = default;

  /*--------------------------------------------------------
  | Getters
  --------------------------------------------------------*/
  const Vec2d& xy() const override { return xy_; }

  /*--------------------------------------------------------
  | Ostream of triangle
  --------------------------------------------------------*/
  std::ostream& print(std::ostream& os) const override
  {
    return os << *v1_ << " -> " << *v2_ << " -> " << *v3_;
  }

private:

  const Vertex* v1_;
  const Vertex* v2_;
  const Vertex* v3_;

  Vec2d xy_ { 0.0f, 0.0f };

}; // Triangle
