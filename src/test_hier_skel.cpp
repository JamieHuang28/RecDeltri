#include "hierarchic_straight_skeleton_2.hpp"
#include <CGAL/IO/WKT.h>

int main(int argc, char* argv[])
{
  std::ifstream ifs( (argc==1)?"../skeleton.wkt":argv[1]);
  Polygon_2 poly ;
  CGAL::read_polygon_WKT(ifs, poly);

  // make poly counterclockwise oriented
  if ( poly.is_clockwise_oriented() )
    poly.reverse_orientation() ;
  assert(poly.is_counterclockwise_oriented());

  // You can pass the polygon via an iterator pair
  SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());

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
