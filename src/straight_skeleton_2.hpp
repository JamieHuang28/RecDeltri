#pragma once

#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Polygon_2.h>
#include<CGAL/create_straight_skeleton_2.h>
// #include "print.h"
#include<boost/shared_ptr.hpp>
#include <cassert>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K ;
typedef K::Point_2                   Point ;
typedef CGAL::Polygon_2<K>           Polygon_2 ;
typedef CGAL::Straight_skeleton_2<K> Ss ;
typedef boost::shared_ptr<Ss> SsPtr ;

template<class K>
std::pair<double, double> print_point ( CGAL::Point_2<K> const& p )
{
  return {p.x(), p.y()};
}

template<class K>
std::vector<double> print_straight_skeleton( CGAL::Straight_skeleton_2<K> const& ss )
{
  std::vector<double> result;
  typedef CGAL::Straight_skeleton_2<K> Ss ;

  typedef typename Ss::Vertex_const_handle     Vertex_const_handle ;
  typedef typename Ss::Halfedge_const_handle   Halfedge_const_handle ;
  typedef typename Ss::Halfedge_const_iterator Halfedge_const_iterator ;

  Halfedge_const_handle null_halfedge ;
  Vertex_const_handle   null_vertex ;

  std::cout << "Straight skeleton with " << ss.size_of_vertices()
            << " vertices, " << ss.size_of_halfedges()
            << " halfedges and " << ss.size_of_faces()
            << " faces" << std::endl ;

  for ( Halfedge_const_iterator i = ss.halfedges_begin(); i != ss.halfedges_end(); ++i )
  {
    std::pair<double, double> pt_coord;
    pt_coord = print_point(i->opposite()->vertex()->point()) ;
    result.push_back(pt_coord.first);
    result.push_back(pt_coord.second);
    pt_coord = print_point(i->vertex()->point());
    result.push_back(pt_coord.first);
    result.push_back(pt_coord.second);
    std::cout << " " << ( i->is_bisector() ? "bisector" : "contour" ) << std::endl;
  }
  return result;
}

std::vector<double> skeleton(const std::vector<double> &coords)
{
  Polygon_2 poly ;
  for (size_t i = 0; i < coords.size(); i += 2)
  {
    poly.push_back(Point(coords[i], coords[i + 1]));
    std::cout << " " << coords[i] << " " << coords[i + 1] << std::endl;
  }
  // poly.push_back( Point(-1,-1) ) ;
  // poly.push_back( Point(0,-12) ) ;
  // poly.push_back( Point(1,-1) ) ;
  // poly.push_back( Point(12,0) ) ;
  // poly.push_back( Point(1,1) ) ;
  // poly.push_back( Point(0,12) ) ;
  // poly.push_back( Point(-1,1) ) ;
  // poly.push_back( Point(-12,0) ) ;
  // make poly counterclockwise oriented
  if ( poly.is_clockwise_oriented() )
    poly.reverse_orientation() ;
  assert(poly.is_counterclockwise_oriented());
  std::cout << "Polygon is " << ( poly.is_simple() ? "" : "not " ) << "simple" << std::endl ;
  // You can pass the polygon via an iterator pair
  SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());
  // // Or you can pass the polygon directly, as below.
  // // To create an exterior straight skeleton you need to specify a maximum offset.
  // double lMaxOffset = 5 ;
  // SsPtr oss = CGAL::create_exterior_straight_skeleton_2(lMaxOffset, poly);
  return print_straight_skeleton(*iss);
}