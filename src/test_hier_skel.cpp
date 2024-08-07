#include "hierarchic_straight_skeleton_2.hpp"
#include <CGAL/IO/WKT.h>

Polygon_2 buildRectangularPolygon(double x, double y, double w, double h) {
  Polygon_2 poly;
  poly.push_back(Point(x, y));
  poly.push_back(Point(x+w, y));
  poly.push_back(Point(x+w, y+h));
  poly.push_back(Point(x, y+h));
  return poly;
}

int main(int argc, char* argv[])
{
  Polygon_2 poly ;
  if (argc == 1) {
    poly = buildRectangularPolygon(0, 0, 2, 1);
  } else {
    std::ifstream ifs(argv[1]);
    CGAL::read_polygon_WKT(ifs, poly);
  }

  // make poly counterclockwise oriented
  if ( poly.is_clockwise_oriented() )
    poly.reverse_orientation() ;
  assert(poly.is_counterclockwise_oriented());

  // You can pass the polygon via an iterator pair
  SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());
  // print all vertices
  for ( Ss::Vertex_const_iterator i = iss->vertices_begin(); i != iss->vertices_end(); ++i ) {
    printf("vertex: (%f, %f)\n", i->point().x(), i->point().y());
  }

  EdgePtr root_e = nullptr;
  VertexPtr root_v = nullptr;
  createGraph(iss, root_e, root_v);
  SkeletonPyramid::skeletonPyramid(root_v, root_e, 0);

  std::vector<double> result;
  for (const EdgePtr &edge : SkeletonPyramid::skeleton()) {
    result.push_back(edge->head_vertex()->x());
    result.push_back(edge->head_vertex()->y());
    result.push_back(edge->tail_vertex()->x());
    result.push_back(edge->tail_vertex()->y());
  }

  createGraph(iss, root_e, root_v, true);
  SkeletonPyramid::skeletonPyramid(root_v, root_e, 0);
  for (const EdgePtr &edge : SkeletonPyramid::skeleton()) {
    result.push_back(edge->head_vertex()->x());
    result.push_back(edge->head_vertex()->y());
    result.push_back(edge->tail_vertex()->x());
    result.push_back(edge->tail_vertex()->y());
  }

  return 0;
}
