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

  hierarchicStraightSkeletonFromPoly(poly, 1, 1.0);
  return 0;
}
