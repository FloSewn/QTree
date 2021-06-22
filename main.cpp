#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <cassert>
#include <iomanip>   

#include <raylib.h>

#include "SimplexContainer.h"
#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"
#include "Quad.h"
#include "QTree.h"


/***********************************************************
* The main function
***********************************************************/
int main()
{

  /*
  auto e1 = list.add( std::make_unique<Edge>( v1, v2 ) );
  auto e2 = list.add( std::make_unique<Edge>( v2, v3 ) );
  auto e3 = list.add( std::make_unique<Edge>( v3, v4 ) );
  auto e4 = list.add( std::make_unique<Edge>( v4, v1 ) );

  auto t1 = list.add( std::make_unique<Triangle>( v1,v2,v3 ));
  auto t2 = list.add( std::make_unique<Triangle>( v2,v3,v4 ));

  auto q1 = list.add( std::make_unique<Quad>( v1,v2,v3,v4 ) );

  list.remove( e1 );
  list.remove( t2 );
  */

  /*
  for (const auto& s_ptr : list)
  {
    const auto& s = *s_ptr;
    std::cout << s << "\n";
  }
  */

  /*********************************************************
  * QTree
  *********************************************************/
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

  return EXIT_SUCCESS;

}
