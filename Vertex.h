#pragma once

#include <list>

#include "Simplex.h"
#include "Vec2.h"
#include "app_util.h"

/***********************************************************
* This class represents a two-dimensional vertex
***********************************************************/
class Vertex : public Simplex
{
public:
  Vertex() 
  : Simplex(), xy_{0.0,0.0} {}
  Vertex(float xs, float ys) 
  : Simplex(), xy_{xs,ys} {}
  Vertex(const Vec2d& p) 
  : Simplex(), xy_{p} {}

  ~Vertex() = default;
  
  /*--------------------------------------------------------
  | Getters
  --------------------------------------------------------*/
  const Vec2d& xy() const override { return xy_; }

  /*--------------------------------------------------------
  | Setters
  --------------------------------------------------------*/
  void xy(const Vec2d& xy) { xy_ = xy; }

  /*--------------------------------------------------------
  | Ostream of vertex
  --------------------------------------------------------*/
  std::ostream& print(std::ostream& os) const override
  {
    return os << "V: " << xy();
  }

private: 
  Vec2d xy_;

}; // Vertex
