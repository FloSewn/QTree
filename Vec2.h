#pragma once
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <float.h>

/***********************************************************
* Vec2d Class
***********************************************************/
template <class T>
class Vec2
{
  T e[2] { T{}, T{} };

public:
  T& x = e[0];
  T& y = e[1];

  // Construct
  Vec2() {}
  Vec2(T e0, T e1) : e{e0,e1} {}

  // Copy 
  Vec2(const Vec2<T>& v) : e{ v.x,v.y } {}
  Vec2<T>& operator=(const Vec2<T>& v) 
  { x=v.x; y=v.y; }

  // Move
  Vec2(Vec2<T>&& v) : e{v.x,v.y} {}
  Vec2<T>& operator=(Vec2<T>&& v) 
  { x=v.x; y=v.y; }

  // Vector access
  T operator[](int i) const 
  { 
    if (i == 0) return x;
    if (i == 1) return y;
    else throw std::runtime_error("Invalid access to Vec2");
  } 
  T& operator[](int i) 
  { 
    if (i == 0) return x;
    if (i == 1) return y;
    else throw std::runtime_error("Invalid access to Vec2");
  } 

  // Vector negation
  Vec2<T> operator-() const { return Vec2(-x,-y); }

  // Vector-vector addition
  Vec2<T>& operator+=(const Vec2<T> &v)
  { x += v.x; y += v.y; return *this; }

  // Vector-vector substraction
  Vec2<T>& operator-=(const Vec2<T> &v)
  { y -= v.x; y -= v.y; return *this; }

  // Vector-vector multiplication
  Vec2<T>& operator*=(const Vec2<T> &v)
  { x *= v.x; y *= v.y; return *this; }

  // Vector-vector division
  Vec2<T>& operator/=(const Vec2<T> &v)
  { x /= v.x; y /= v.y; return *this; }

  // Vector-scalar addition
  Vec2<T>& operator+=(const T t)
  { x += t; y += t; return *this; }

  // Vector-scalar substraction
  Vec2<T>& operator-=(const T t)
  { x -= t; y -= t; return *this; }

  // Vector-scalar multiplication
  Vec2<T>& operator*=(const T t)
  { x *= t; y *= t; return *this; }

  // Vector-scalar division
  Vec2<T>& operator/=(const T t)
  { return *this *= 1./t; }

  // Estimate vector length
  double length() const 
  { return std::sqrt(length_squared()); }

  // Estimate squared vector length
  double length_squared() const 
  { return x*x + y*y; }

  // Return true if each vector-value is close to zero 
  bool near_zero_values(const T s=DBL_EPSILON) const
  { return (fabs(x) < s) && (fabs(y) < s); }

  // Return true if vector-length is close to zero 
  bool near_zero_length(const T s=DBL_EPSILON) const
  { return (length() < s); }

};

/***********************************************************
* Vec2 utility functions
***********************************************************/

// Output 
template <typename T>
inline std::ostream& operator<<(std::ostream &out, 
                                const Vec2<T> &v)
{ return out << '(' << v.x << ',' << v.y << ')'; }

// Equality
template <typename T>
inline bool operator==(const Vec2<T>& a, const Vec2<T>& b)
{ return (a-b).near_zero_values(); }

// Un-Equality
template <typename T>
inline bool operator!=(const Vec2<T>& a, const Vec2<T>& b)
{ return !(a==b); }

// Vector-Vector addition
template <typename T>
inline Vec2<T> operator+(const Vec2<T> &u,const Vec2<T> &v)
{ return Vec2<T>(u.x+v.x, u.y+v.y); }

// Vector-Vector substraction
template <typename T>
inline Vec2<T> operator-(const Vec2<T> &u,const Vec2<T> &v)
{ return Vec2<T>(u.x-v.x, u.y-v.y); }

// Vector-Vector multiplication
template <typename T>
inline Vec2<T> operator*(const Vec2<T> &u,const Vec2<T> &v)
{ return Vec2<T>(u.x*v.x, u.y*v.y); }

// Vector-Vector division
template <typename T>
inline Vec2<T> operator/(const Vec2<T> &u,const Vec2<T> &v)
{ return Vec2<T>(u.x/v.x, u.y/v.y); }


// Vector-scalar addition
template <typename T>
inline Vec2<T> operator+(const Vec2<T> &u, const T v)
{ return Vec2<T>(u.x+v, u.y+v); }
template <typename T>
inline Vec2<T> operator+(const T v, const Vec2<T> &u)
{ return Vec2<T>(u.x+v, u.y+v); }

// Vector-scalar substraction
template <typename T>
inline Vec2<T> operator-(const Vec2<T> &u, const T v)
{ return Vec2<T>(u.x-v, u.y-v); }
template <typename T>
inline Vec2<T> operator-(const T v, const Vec2<T> &u)
{ return Vec2<T>(v-u.x, v-u.y); }

// Vector-scalar multiplication
template <typename T>
inline Vec2<T> operator*(const Vec2<T> &u, const T v)
{ return Vec2<T>(u.x*v, u.y*v); }
template <typename T>
inline Vec2<T> operator*(const T v, const Vec2<T> &u)
{ return Vec2<T>(u.x*v, u.y*v); }

// Vector-scalar division
template <typename T>
inline Vec2<T> operator/(const Vec2<T> &u, const T v)
{ return Vec2<T>(u.x/v, u.y/v); }
template <typename T>
inline Vec2<T> operator/(const T v, const Vec2<T> &u)
{ return Vec2<T>(v/u.x, v/u.y); }


// Scalar product
template <typename T>
inline T dot(const Vec2<T> &u, const Vec2<T> &v)
{ return u.x * v.x + u.y * v.y; }
// Cross product
template <typename T>
inline T cross(const Vec2<T> &u, const Vec2<T> &v)
{ return u.x * v.y - u.y * v.x; }

// Unit-vector
template <typename T>
inline Vec2<T> unit_vector(const Vec2<T>& u)
{ return u / u.length(); }


/***********************************************************
* integer / double vector aliases
***********************************************************/
using Vec2i = Vec2<int>;
using Vec2d = Vec2<double>;
using Vec2f = Vec2<float>;

/***********************************************************
* Vec2 geometry functions
***********************************************************/
namespace Vec2Geom
{

enum class Orientation
{
  CW,     // Clockwise
  CCW,    // Counter-Clockwise
  CL      // Colinear
};

static constexpr double geometry_small = 1.0E-17;

/*----------------------------------------------------------
| Min() / Max() functions
----------------------------------------------------------*/
template <typename T>
static inline T minimum(T a, T b) { return a < b ? a : b; }
template <typename T>
static inline T maximum(T a, T b) { return a > b ? a : b; }
template <typename T>
static inline Vec2<T> bbox_min(const Vec2<T>& a, const Vec2<T>& b)
{ return Vec2<T> { minimum(a[0],b[0]), minimum(a[1], b[1]) }; }
template <typename T>
static inline Vec2<T> bbox_max(const Vec2<T>& a, const Vec2<T>& b)
{ return Vec2<T> { maximum(a[0],b[0]), maximum(a[1], b[1]) }; }

/*----------------------------------------------------------
| Check for orientation of three points (p, q, r)
----------------------------------------------------------*/
template <typename T>
static inline Orientation orientation(const Vec2<T>& p,
                                 const Vec2<T>& q,
                                 const Vec2<T>& r)
{
  T area2 = (p[0]-r[0]) * (q[1]-r[1]) 
          - (q[0]-r[0]) * (p[1]-r[1]);
  
  if ( ( area2*area2 ) < geometry_small )
    return Orientation::CL;

  if ( area2 > 0)
    return Orientation::CCW;
  
  return Orientation::CW;
}

/*----------------------------------------------------------
| Check if point r lies to the left of segment (p,q) 
----------------------------------------------------------*/
template <typename T>
static inline bool is_left(const Vec2<T>& p,
                           const Vec2<T>& q,
                           const Vec2<T>& r)
{
  if (orientation(p,q,r) == Orientation::CCW)
    return true;
  return false;
}

/*----------------------------------------------------------
| Check if point r lies to the left of segment (p,q) o
| or on the segment
----------------------------------------------------------*/
template <typename T>
static inline bool is_lefton(const Vec2<T>& p,
                             const Vec2<T>& q,
                             const Vec2<T>& r)
{
  if (orientation(p,q,r) == Orientation::CW)
    return false;

  return true;
}

/*----------------------------------------------------------
| Check if point r lies within a segment (p,q) 
----------------------------------------------------------*/
template <typename T>
static inline bool in_segment(const Vec2<T>& p,
                              const Vec2<T>& q,
                              const Vec2<T>& r)
{
  if (orientation(p,q,r) != Orientation::CL)
    return false;

  //Vec2<T> min_bb = bbox_min(p, q);
  //Vec2<T> max_bb = bbox_min(p, q);

  const Vec2<T> d_qp  = q-p;
  const Vec2<T> d_rp  = r-p;
  const T t = dot(d_rp, d_qp) / d_qp.length_squared();

  if ( t > 0 && t < 1)
    return true;
  
  return false;
}

/*----------------------------------------------------------
| Check if point r lies within a segment (p,q) or on
| its endings 
----------------------------------------------------------*/
template <typename T>
static inline bool in_on_segment(const Vec2<T>& p,
                                 const Vec2<T>& q,
                                 const Vec2<T>& r)
{
  if (orientation(p,q,r) != Orientation::CL)
    return false;

  const Vec2<T> d_qp  = q-p;
  const Vec2<T> d_rp  = r-p;
  const T t = dot(d_rp, d_qp) / d_qp.length_squared();

  if ( t >= 0 && t <= 1)
    return true;
  
  return false;
}

/*----------------------------------------------------------
| Check if two lines (p1,q1) and (p2,q2) intersect
| 
| * Returns true, if segments intersect at any point but
|   their edges
| * Returns true, if one line contains a part of the other
| * Returns false, if both lines share both end points
| * Returns false in all other cases
----------------------------------------------------------*/
template <typename T>
static inline bool line_intersection(const Vec2<T>& p1,
                                     const Vec2<T>& q1,
                                     const Vec2<T>& p2,
                                     const Vec2<T>& q2)
{
  Orientation o1 = orientation(p1, q1, p2);
  Orientation o2 = orientation(p1, q1, q2);
  Orientation o3 = orientation(p2, q2, p1);
  Orientation o4 = orientation(p2, q2, q1);

  if (  ( (o1 == Orientation::CCW && o2 == Orientation::CW ) ||
          (o1 == Orientation::CW  && o2 == Orientation::CCW) ) 
     && ( (o3 == Orientation::CCW && o4 == Orientation::CW ) ||
          (o3 == Orientation::CW  && o4 == Orientation::CCW) ) )
  {
    return true;
  }

  // (p1,q1) and p2 are collinear and p2 lies on segment (p1,q1)
  if ( (o1 == Orientation::CL) && in_segment(p1,q1,p2) )
    return true;

  // (p1,q1) and q2 are collinear and q2 lies on segment (p1,q1)
  if ( (o2 == Orientation::CL) && in_segment(p1,q1,q2) )
    return true;

  // (p2,q2) and p1 are collinear and p1 lies on segment (p2,q2)
  if ( (o3 == Orientation::CL) && in_segment(p2,q2,p1) )
    return true;

  // (p2,q2) and q1 are collinear and q1 lies on segment (p2,q2)
  if ( (o4 == Orientation::CL) && in_segment(p2,q2,q1) )
    return true;

  return false;
}


/*--------------------------------------------------------
| Check if two rectangles a & b overlap. The rectangles
| are defined by their lower left and upper right 
| corners.
--------------------------------------------------------*/
inline bool rect_overlap(const Vec2d& a_lowleft, 
                         const Vec2d& a_upright,
                         const Vec2d& b_lowleft,
                         const Vec2d& b_upright)
{
  return (  (a_lowleft.x <= b_upright.x)
         && (b_lowleft.x <= a_upright.x)
         && (a_lowleft.y <= b_upright.y)
         && (b_lowleft.y <= a_upright.y) );

} /* rect_overlap() */

/*--------------------------------------------------------
| Check if a vertex v is inside or on the edge of a 
| rectangle. The rectangle is defined by its lower 
| left and upper right corners.
--------------------------------------------------------*/
inline bool in_on_rect(const Vec2d& v,
                       const Vec2d& lowleft,
                       const Vec2d& upright)
{
  return (  (v.x >= lowleft.x) && (v.y >= lowleft.y)
         && (v.x <= upright.x) && (v.y <= upright.y) );
}

/*--------------------------------------------------------
| Check if a vertex v is inside of a 
| rectangle. The rectangle is defined by its lower 
| left and upper right corners.
--------------------------------------------------------*/
inline bool in_rect(const Vec2d& v,
                    const Vec2d& lowleft,
                    const Vec2d& upright)
{
  return (  (v.x > lowleft.x) && (v.y > lowleft.y)
         && (v.x < upright.x) && (v.y < upright.y) );
}

}; // Namespace Vec2Geom
