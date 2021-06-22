#pragma once

#include <list>           // std::list
#include <memory>         // std::shared_ptr

#include "app_util.h"
#include "Vec2.h"

/***********************************************************
* This class is the base for any two-dimensional object
***********************************************************/
class Simplex
{
public:
  friend SimplexContainer;

  using SimplexListIterator 
    = std::list< std::unique_ptr<Simplex> >::iterator;

  Simplex() = default;

  /*--------------------------------------------------------
  | Ostream of simplex
  --------------------------------------------------------*/
  virtual std::ostream& print(std::ostream& os) const = 0;

  /*--------------------------------------------------------
  | Setters
  --------------------------------------------------------*/

  /*--------------------------------------------------------
  | Getters
  --------------------------------------------------------*/
  virtual const Vec2d& xy() const = 0;

protected:
  SimplexListIterator list_pos_ { nullptr };

}; // Simplex

/***********************************************************
* Simplex ostream operator overload
***********************************************************/
static std::ostream& operator<<(std::ostream& os, const Simplex& s)
{
  return s.print(os);
}

