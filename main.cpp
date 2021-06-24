#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>   
#include <cmath>
#include <chrono>

#include "SimplexContainer.h"
#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"
#include "Quad.h"
#include "QTree.h"


double benchmark(int n, int r)
{
  using Clock = std::chrono::high_resolution_clock;
  using Second = std::chrono::duration<double, std::ratio<1> >;
  using Timepoint = std::chrono::time_point<Clock>;

  SimplexContainer vertices {};
  QTree<Vertex> qtree { {0.0, 0.0}, 150.0, 32, 8 };

  constexpr double a  =  2.5;
  constexpr double b  = -3.4;
  constexpr double c  =  6.5;
  constexpr double t0 =  0.0;
  constexpr double t1 =  5.0 * M_PI;
  const double dt = (t1-t0) / static_cast<double>(n);

  Timepoint time_0 = Clock::now();

  /*--------------------------------------------------------
  | Create nodes
  --------------------------------------------------------*/
  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    const double x = (a + b * t) * cos(t) + c * sin(40.*t);
    const double y = (a + b * t) * sin(t) + c * cos(40.*t);

    vertices.add( std::make_unique<Vertex>( x, y) );
  }

  // -------------------- Timing ---------------------------
  Timepoint time_1 = Clock::now();
  // -------------------------------------------------------

  /*--------------------------------------------------------
  | Add nodes to qtree
  --------------------------------------------------------*/
  for ( const auto& v_ptr : vertices )
    qtree.add( reinterpret_cast<Vertex*>( v_ptr.get() ) );

  // -------------------- Timing ---------------------------
  Timepoint time_2 = Clock::now();
  // -------------------------------------------------------

  /*--------------------------------------------------------
  | Find objects within rectangle using qtree
  --------------------------------------------------------*/
  int item_count_qt = 0;

  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    const double x = (a + b * t) * cos(t) + c * sin(40.*t);
    const double y = (a + b * t) * sin(t) + c * cos(40.*t);

    std::vector<Vertex*> items;
    qtree.get_items( {x-5.0, y-5.0}, {x+5.0,y+5.0}, items );

    item_count_qt += items.size();
  }

  // -------------------- Timing ---------------------------
  Timepoint time_3 = Clock::now();
  // -------------------------------------------------------

  /*--------------------------------------------------------
  | Find objects within rectangle using brute force
  --------------------------------------------------------*/
  int item_count_bf = 0;

  for (int i = 0; i < n; i++)
  {
    const double t = t0 + i*dt;
    const double x = (a + b * t) * cos(t) + c * sin(40.*t);
    const double y = (a + b * t) * sin(t) + c * cos(40.*t);

    for ( const auto& v_ptr : vertices )
    {
      const auto& v = *v_ptr;
      Vec2d xy = v.xy();

      bool in_rect = Vec2Geom::in_on_rect(
          xy, {x-5.0,y-5.0}, {x+5.0,y+5.0} );

      if ( in_rect )
        ++item_count_bf;
    }
  }

  // -------------------- Timing ---------------------------
  Timepoint time_4 = Clock::now();
  // -------------------------------------------------------

  /*--------------------------------------------------------
  | Assert that qtree found all items
  --------------------------------------------------------*/
  assert( item_count_qt == item_count_bf );

  /*--------------------------------------------------------
  | Randomly remove r vertices 
  --------------------------------------------------------*/


  /*--------------------------------------------------------
  | User output
  --------------------------------------------------------*/
  double delta_1 = std::chrono::duration_cast<Second>(
      time_1 - time_0).count();
  double delta_2 = std::chrono::duration_cast<Second>(
      time_2 - time_1).count();
  double delta_3 = std::chrono::duration_cast<Second>(
      time_3 - time_2).count();
  double delta_4 = std::chrono::duration_cast<Second>(
      time_4 - time_3).count();

  std::cout << "----------------------------------\n";
  std::cout << "Benchmark n=" << n << " r=" << r << "\n";
  std::cout << "SimplexContainer initialization  : " 
            << delta_1 << "s \n";
  std::cout << "QTree initialization             : " 
            << delta_2 << "s \n";
  std::cout << "QTree search                     : " 
            << delta_3 << "s \n";
  std::cout << "BruteForce search                : " 
            << delta_4 << "s \n";
  std::cout << "QTree speedup                    : "
            << delta_4 / delta_3 << "\n";


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

  /*********************************************************
  * QTree
  *********************************************************
  QTree<Vertex> qtree { {6.0, 6.0}, 12.0, 2, 4 };

  SimplexContainer vertices {};

  auto v1  = vertices.add( std::make_unique<Vertex>( 2.0,  2.0) );
  auto v2  = vertices.add( std::make_unique<Vertex>(10.0,  8.0) );
  auto v3  = vertices.add( std::make_unique<Vertex>( 8.0,  4.0) );
  auto v4  = vertices.add( std::make_unique<Vertex>( 3.0,  9.0) );
  auto v5  = vertices.add( std::make_unique<Vertex>( 4.0,  4.0) );
  auto v6  = vertices.add( std::make_unique<Vertex>(10.0,  2.0) );
  auto v7  = vertices.add( std::make_unique<Vertex>( 8.0, 10.0) );
  auto v8  = vertices.add( std::make_unique<Vertex>( 1.0, 11.0) );
  auto v9  = vertices.add( std::make_unique<Vertex>( 1.0,  5.0) );
  auto v10 = vertices.add( std::make_unique<Vertex>( 7.0,  1.0) );
  auto v11 = vertices.add( std::make_unique<Vertex>(10.0,  1.0) );
  auto v12 = vertices.add( std::make_unique<Vertex>(11.0,  1.0) );

  qtree.add( v1 );
  qtree.add( v2 );

  // Split
  qtree.add( v3 );
  assert( qtree.split() == true );
  qtree.add( v4 );
  qtree.add( v5 );
  qtree.add( v6 );
  qtree.add( v7 );
  qtree.add( v8 );

  // Split
  qtree.add( v9  );
  assert( qtree.children()[2]->split() == true );

  // Split
  qtree.add( v10 );
  assert( qtree.children()[3]->split() == true );
  qtree.add( v11 );

  // Split
  qtree.add( v12 );
  assert( qtree.children()[3]->children()[3]->split()==true );

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

  // Find objects 
  std::vector<Vertex*> v_found;
  qtree.get_items( {7.0,7.0}, {10.0,10.0}, v_found );

  std::cout << "# Found objects: ";
  for (auto v : v_found)
    std::cout << *v << "  ";
  std::cout << "\n";


  // Merge 
  qtree.remove( v12 );
  assert( qtree.children()[3]->children()[3]->split()==false );
  qtree.remove( v11 );

  // Merge
  qtree.remove( v10 );
  assert( qtree.children()[3]->split() == false );

  // Merge
  qtree.remove( v9 );
  assert( qtree.children()[2]->split() == false );
  */
  benchmark(100, 0);

  benchmark(340, 0);

  benchmark(1000, 0);

  benchmark(10000, 0);

  //benchmark(100000, 0);


  return EXIT_SUCCESS;

}
