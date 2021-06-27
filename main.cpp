#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>   
#include <cmath>
#include <cstdlib>

#include "SimplexContainer.h"
#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"
#include "Quad.h"
#include "QTree.h"
#include "Timer.h"



class SpiralFunction
{
public:
  SpiralFunction(double a, double b, double c)
  : a_ { a }, b_ { b }, c_ { c } {}

  inline Vec2d eval( const double t ) const
  {
    double x = (a_ + b_ * t) * cos(t) + c_ * sin(40.*t);
    double y = (a_ + b_ * t) * sin(t) + c_ * cos(40.*t);
    return { x,y };
  }

  inline void eval( const double t, double& x, double& y)
  {
    x = (a_ + b_ * t) * cos(t) + c_ * sin(40.*t);
    y = (a_ + b_ * t) * sin(t) + c_ * cos(40.*t);
  }

private:
  double a_  { 0.0 };
  double b_  { 0.0 };
  double c_  { 0.0 };


}; // BFunction


double benchmark(int n, int r)
{
  SimplexContainer vertices {};
  QTree<Vertex> qtree { {0.0, 0.0}, 150.0, 32, 8 };
  Timer timer {};
  SpiralFunction fun( 2.5, -3.4, 6.5);

  constexpr double t0 =  0.0;
  constexpr double t1 =  5.0 * M_PI;
  const double dt = (t1-t0) / static_cast<double>(n);

  double x, y;
  int item_count_qt, item_count_bf;

  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------

  
  // Create nodes
  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    fun.eval(t, x, y);
    vertices.add( std::make_unique<Vertex>( x, y ) );
  }

  
  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------


  // Add nodes to qtree
  for ( const auto& v_ptr : vertices )
    qtree.add( reinterpret_cast<Vertex*>( v_ptr.get() ) );


  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------


  // Find objects within rectangle using qtree
  item_count_qt = 0;

  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    fun.eval( t, x, y );

    std::vector<Vertex*> items;
    qtree.get_items( {x-5.,y-5.}, {x+5.,y+5.}, items );

    item_count_qt += items.size();
  }


  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------


  // Find objects within rectangle using brute force
  item_count_bf = 0;

  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    fun.eval( t, x, y );

    for ( const auto& v_ptr : vertices )
    {
      const auto& v = *v_ptr;
      Vec2d xy = v.xy();

      bool in_rect = Vec2Geom::in_on_rect( 
          xy, {x-5.,y-5.}, {x+5.,y+5.} );

      if ( in_rect )
        ++item_count_bf;
    }
  }


  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------


  // Assert that qtree found all items
  assert( item_count_qt == item_count_bf );


  // Randomly remove r vertices 
  std::srand(123);

  for (int i = 0; i < r; i++)
  {
    auto it = vertices.begin();
    int pick = (std::rand() % (n-i));
    std::advance(it, pick);

    Vertex* v = reinterpret_cast<Vertex*>( it->get() );

    qtree.remove( v );
    vertices.remove( v );

  }


  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------


  // -------------------------------------------------------
  // Repeate serach benchmark, to verify that removal 
  // of objects succeeded
  // -------------------------------------------------------
  
  // AGAIN: Find objects within rectangle using qtree 
  item_count_qt = 0;

  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    fun.eval( t, x, y );

    std::vector<Vertex*> items;
    qtree.get_items( {x-5.,y-5.}, {x+5.,y+5.}, items );

    item_count_qt += items.size();
  }


  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------


  // AGAIN: Find objects within rectangle using brute force
  item_count_bf = 0;

  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    fun.eval( t, x, y );

    for ( const auto& v_ptr : vertices )
    {
      const auto& v = *v_ptr;
      Vec2d xy = v.xy();

      bool in_rect = Vec2Geom::in_on_rect( 
          xy, {x-5.,y-5.}, {x+5.,y+5.} );

      if ( in_rect )
        ++item_count_bf;
    }
  }


  // -------------------- Timing ---------------------------
  timer.count();
  // -------------------------------------------------------


  // User output
  std::cout << "----------------------------------\n";
  std::cout << "Benchmark n=" << n << " r=" << r << "\n";
  std::cout << "SimplexContainer initialization  : " 
            << timer.delta(0) << "s \n";
  std::cout << "QTree initialization             : " 
            << timer.delta(1) << "s \n";
  std::cout << "QTree search                     : " 
            << timer.delta(2) << "s \n";
  std::cout << "BruteForce search                : " 
            << timer.delta(3) << "s \n";
  std::cout << "QTree speedup                    : "
            << timer.delta(3) / timer.delta(2) << "\n";
  std::cout << "Data removal                     : "
            << timer.delta(4) << "\n";
  std::cout << "QTree search after removal       : " 
            << timer.delta(5) << "s \n";
  std::cout << "BruteForce search after removal  : " 
            << timer.delta(6) << "s \n";
  std::cout << "QTree speedup after removal      : "
            << timer.delta(6) / timer.delta(5) << "\n";

  /*
  // Print out vertices
  std::cout << "VERTICES " << vertices.size() << "\n";
  for ( const auto& v_ptr : vertices )
  {
    const auto& v = *v_ptr;

    std::cout << std::setprecision(5) << std::fixed
      << v.xy().x << "," << v.xy().y << "\n";
  }

  // Print out qtree
  std::cout << "QTREE " << qtree.n_leafs() << "\n";
  std::cout << qtree;
  */



  return 0.0;
  
} /* benchmark() */


/***********************************************************
* The main function
***********************************************************/
int main()
{

  benchmark(100, 10);

  benchmark(340, 34);

  benchmark(1000, 100);

  benchmark(10000, 1000);

  benchmark(100000, 10000);


  return EXIT_SUCCESS;

}
