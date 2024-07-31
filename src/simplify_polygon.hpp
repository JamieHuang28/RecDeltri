#include <CGAL/Polyline_simplification_2/simplify.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

namespace PS = CGAL::Polyline_simplification_2;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef PS::Stop_below_count_ratio_threshold Stop;
typedef PS::Squared_distance_cost            Cost;

Polygon_2 simplifyPolygon(const Polygon_2 &polygon, double ratio)
{
  Cost cost;
  return PS::simplify(polygon, cost, Stop(ratio));
}

std::vector<double> simplifyPolylines(const std::vector<double> &polylines, double ratio) {
  Polygon_2 poly_raw;
  for (size_t i = 0; i < polylines.size(); i += 2)
  {
    poly_raw.push_back(K::Point_2(polylines[i], polylines[i + 1]));
  }
  Polygon_2 poly = simplifyPolygon(poly_raw, ratio);
  std::vector<double> result;
  for (auto it = poly.vertices_begin(); it != poly.vertices_end(); ++it)
  {
    result.push_back(it->x());
    result.push_back(it->y());
  }
  return result;
}