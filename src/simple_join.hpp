#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <iostream>
#include <list> 

namespace simple_join {

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

//-----------------------------------------------------------------------------
// Pretty-print a CGAL polygon.
//
template<class Kernel, class Container>
std::vector<double> print_polygon (const CGAL::Polygon_2<Kernel, Container>& P)
{
  std::vector<double> coords;
  typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator  vit;

  std::cout << "[ " << P.size() << " vertices:";
  for (vit = P.vertices_begin(); vit != P.vertices_end(); ++vit) {
    double x, y;
    x = CGAL::to_double(vit->x());
    y = CGAL::to_double(vit->y());
    coords.push_back(x);
    coords.push_back(y);
  }
  std::cout << " ]" << std::endl;

  return coords;
}

//-----------------------------------------------------------------------------
// Pretty-print a polygon with holes.
//
template<class Kernel, class Container>
void print_polygon_with_holes
    (const CGAL::Polygon_with_holes_2<Kernel, Container>& pwh)
{
  if (! pwh.is_unbounded())
  {
    std::cout << "{ Outer boundary = ";
    print_polygon (pwh.outer_boundary());
  }
  else
    std::cout << "{ Unbounded polygon." << std::endl;

  typename CGAL::Polygon_with_holes_2<Kernel,Container>::
                                             Hole_const_iterator  hit;
  unsigned int                                                     k = 1;

  std::cout << "  " << pwh.number_of_holes() << " holes:" << std::endl;
  for (hit = pwh.holes_begin(); hit != pwh.holes_end(); ++hit, ++k)
  {
    std::cout << "    Hole #" << k << " = ";
    print_polygon (*hit);
  }
  std::cout << " }" << std::endl;

  return;
}

std::vector<double> simpleJoin (const std::vector<double> &p1, const std::vector<double> &p2)
{
  // Construct the two input polygons.
  Polygon_2 P;
  for (int i = 0; i < p1.size(); i+=2) {
    P.push_back (Point_2 (p1[i], p1[i+1]));
  }

  std::cout << "P = "; print_polygon (P);
  if (P.is_clockwise_oriented())
    P.reverse_orientation();

  Polygon_2 Q;
  for (int i = 0; i < p2.size(); i+=2) {
    Q.push_back (Point_2 (p2[i], p2[i+1]));
  }
  if (Q.is_clockwise_oriented())
    Q.reverse_orientation();

  std::cout << "Q = "; print_polygon (Q);

  // Compute the union of P and Q.
  Polygon_with_holes_2 unionR;

  std::vector<double> result;
  if (CGAL::join (P, Q, unionR)) {
    std::cout << "The union: ";
    print_polygon_with_holes (unionR);
  } else
    std::cout << "P and Q are disjoint and their union is trivial."
              << std::endl;

  return print_polygon(unionR.outer_boundary());
}

}