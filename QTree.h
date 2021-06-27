#pragma once

#include <list>      // std::list
#include <array>     // std::array
#include <stdexcept> // std::runtime_error
#include <iomanip>   // std::setprecision
#include <iostream>  // std::to_string

#include "Vec2.h"
#include "Simplex.h"
#include "SimplexContainer.h"

using namespace Vec2Geom;

/***********************************************************
* This class refers to a quad tree for 2D simplices
*
*
*                  scale_
*         |<-------------------->| (upright_)
*         *----------------------*--
*         |                      | ^
*         |                      | |
*         |                      | |
*         |       (center_)      | |
*         |           *          | | scale_
*         |                      | |
*         |                      | |
*         |                      | |
*         |                      | v
*         *----------------------*--
*  (lowleft_)
*
***********************************************************/
template <typename T>
class QTree
{
public:
  using List   = std::list<T*>;
  using Array  = std::array<QTree<T>*,4>; 
  using Vector = std::vector<T*>;

  /*-------------------------------------------------------- 
  | Constructor
  --------------------------------------------------------*/
  QTree(const Vec2d&  center, 
        double        scale, 
        int           max_item,
        int           max_depth, 
        int           depth=0,
        QTree<T>*     parent=nullptr)
  : center_     { center    }
  , scale_      { scale     }
  , max_item_   { max_item  }
  , max_depth_  { max_depth }
  , depth_      { depth     }
  , parent_     { parent    }
  {  
    halfscale_ = 0.5 * scale_;

    Vec2d hsv  = { halfscale_, halfscale_ };
    lowleft_   = center_ - hsv;
    upright_   = center_ + hsv;

  } // QTree()

  /*-------------------------------------------------------- 
  | Destructor
  --------------------------------------------------------*/
  ~QTree() 
  {
    if ( split_ )
      for ( auto child : children_ )
        if ( child ) delete child;

  } // ~QTree() 

  /*-------------------------------------------------------- 
  | Getters
  --------------------------------------------------------*/
  int size() const { return n_items_; }
  bool split() const { return split_; }
  const List& items() const { return items_; }
  const Array& children() const { return children_; }
  double scale() const { return scale_; }
  const Vec2d& center() const { return center_; }

  /*--------------------------------------------------------
  | Return the total number of qtrees
  --------------------------------------------------------*/
  int n_leafs( int n = 0 ) const 
  {
    if ( !split_ ) n++;

    if ( split_ )
      for ( auto child : children_ )
        if ( child ) 
          n = child->n_leafs( n );

    return n;

  } /* n_leafs() */

  /*-------------------------------------------------------- 
  | Get items within bounding box
  --------------------------------------------------------*/
  bool get_items(const Vec2d& lowleft, 
                 const Vec2d& upright,
                 Vector& found)
  {
    bool overlap = rect_overlap( lowleft_, upright_,  
                                 lowleft,  upright  );
                                 
    if ( !overlap ) return false;

    int n_found = 0;

    if ( split_ )
    {
      for ( auto child : children_ )
        if ( child ) 
          child->get_items( lowleft, upright, found );
    }
    else
    {
      for ( auto item : items_ )
        if ( in_on_rect(item->xy(), lowleft, upright) )
        {
          found.push_back( item );
          ++n_found;
        }
    }

    return (n_found > 0);

  } /* get_items() */

  /*-------------------------------------------------------- 
  | Add a new item to the tree
  --------------------------------------------------------*/
  bool add(T* item)
  {
    // Check if item is located within this quad
    if (   !item 
       ||  !in_on_rect(item->xy(), lowleft_, upright_) )
      return false;

    // If quad is splitted, pass item to children
    if ( split_ ) 
    {
      if ( pass_children( item ) )
        return true;
      else
        throw std::runtime_error(
            "Failed to add item to QTree.");
    }

    // Add item to this quad
    items_.push_back( item );

    // Increase total number of items in entire tree
    add_item_number( 1 );

    // Split this quad if maxmium item number is reached
    if ( items_.size() > max_item_ )
    {
      if ( depth_ < max_depth_ ) 
      {
        if ( split_qtree() )
          return true;
        else 
          throw std::runtime_error(
              "Failed to distribute items "
              "among QTree children.");
      }
    }

    return true;

  } /* add() */

  /*-------------------------------------------------------- 
  | Remove an item from the tree
  --------------------------------------------------------*/
  bool remove(T* item)
  {
    if (   !item 
       ||  !in_on_rect(item->xy(), lowleft_, upright_) )
      return false;

    bool removed = false;

    if ( split_ )
    {
      for ( auto child : children_ )
        if ( child && ( removed = child->remove( item ) ) )
          break;

      if ( removed )
        if ( n_items_ <= max_item_ )
          if ( !merge() )
            throw std::runtime_error(
                "Failed to merge QTree structure.");
    }
    else
    {
      int orig_size = items_.size();
      items_.remove( item );
      int new_size = items_.size();
      removed = new_size < orig_size;

      if ( removed ) add_item_number( -1 );
    }

    return removed;

  } /* remove() */

protected:

  /*-------------------------------------------------------- 
  | Add to total number of items in entire tree 
  --------------------------------------------------------*/
  void add_item_number( int i ) 
  {
    n_items_ += i;

    if ( parent_ ) 
      parent_->add_item_number( i );

  } /* add_item_number() */

private:

  /*-------------------------------------------------------- 
  | Merge QTree children
  --------------------------------------------------------*/
  bool merge()
  {
    for ( auto child : children_ )
      if ( !child || child->split() )
        return false;

    for ( auto child : children_ )
      if ( child )
        for ( auto item : child->items() )
          items_.push_back( item );

    for ( auto child : children_ )
    {
      delete child;
      child = nullptr;
    }

    split_ = false;

    return true;

  } /* merge() */

  /*-------------------------------------------------------- 
  | Pass an item to children
  --------------------------------------------------------*/
  bool pass_children(T* item)
  {
    bool passed = false;

    for (auto child : children_)
      if ( child && ( passed = child->add( item ) ) )
        return true;

    return false;
  }

  /*-------------------------------------------------------- 
  | Split this quad
  --------------------------------------------------------*/
  bool split_qtree() 
  { 
    int ch_depth = depth_ + 1;

    double h = 0.5 * halfscale_;

    // Centroids of children
    Vec2d c0 = { center_.x + h,  center_.y + h };
    Vec2d c1 = { center_.x - h,  center_.y + h };
    Vec2d c2 = { center_.x - h,  center_.y - h };
    Vec2d c3 = { center_.x + h,  center_.y - h };

    // Child quad: NORTH-EAST (NE)
    children_[0] = new QTree<T> 
    { c0, halfscale_, max_item_, max_depth_, ch_depth, this};
                               
    // Child quad: NORTH-WEST (NW)
    children_[1] = new QTree<T> 
    { c1, halfscale_, max_item_, max_depth_, ch_depth, this};

    // Child quad: SOUTH-WEST (SW)
    children_[2] = new QTree<T> 
    { c2, halfscale_, max_item_, max_depth_, ch_depth, this};

    // Child quad: SOUTH-EAST (SE)
    children_[3] = new QTree<T> 
    { c3, halfscale_, max_item_, max_depth_, ch_depth, this};

    // Distribute items among children
    if ( !distribute_items() )
      return false;

    split_ = true;

    // Check that quad is empty
    if ( items_.size() > 0 )
      return false;

    return true;
    
  } /* split_qtree() */

  /*-------------------------------------------------------- 
  | Distribute items to children qtrees
  --------------------------------------------------------*/
  bool distribute_items()
  {
    while ( items_.size() > 0 )
    {
      auto item = items_.back();

      bool passed = false;

      for (auto child : children_)
      {
        if ( child && ( passed = child->add(item) ) )
        {
          add_item_number( -1 );
          break;
        }
      }

      if (!passed)
        return false;

      items_.pop_back();
    }

    return true;

  } /* distribute_items() */

  /*--------------------------------------------------------
  | Attributes
  --------------------------------------------------------*/
  Vec2d      center_   { 0.0, 0.0 };
  Vec2d      lowleft_  { 0.0, 0.0 };
  Vec2d      upright_  { 0.0, 0.0 };

  double     scale_     { 0.0 };
  double     halfscale_ { 0.0 };
  int        max_item_  { 0 };
  int        max_depth_ { 0 };

  bool       split_     { false };
  int        depth_     { 0 };
  int        n_items_   { 0 };

  List       items_;
  Array      children_  { nullptr };
  QTree<T>*  parent_    { nullptr };



}; // QTree

/***********************************************************
* Stream to std::cout
***********************************************************/
template<typename T>
std::ostream& operator<<(std::ostream& os, const QTree<T>& qt)
{
  if ( qt.split() )
  {
    for ( auto child : qt.children() )
      if ( child ) os << *child;
  }
  else
  {
    os << std::setprecision(5) << std::fixed
       << qt.center().x << ","
       << qt.center().y << ","
       << qt.scale()    << ","
       << qt.size()     << "\n";
  }

  return os;
  
}


