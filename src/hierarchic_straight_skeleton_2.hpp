#include "straight_skeleton_2.hpp"
#include "hierarchic_skeleton.hpp"

class EdgeAdaptor;
class VertexAdaptor : public VertexInterface {
public:
  VertexAdaptor(SsPtr ss, Ss::Vertex_const_handle handle) : ss_(ss), handle_(handle) {
    if (handle_ != nullptr) {
      deltaR_ = handle_->time();
    } else {
      printf("handle_ is nullptr\n");
      deltaR_ = 0;
    }
  }
  ~VertexAdaptor() {}
  std::deque<EdgePtr> edges();
  double &deltaR() {
    return deltaR_;
  }
  const double &x() {
    return handle_->point().x();
  }
  const double &y() {
    return handle_->point().y();
  }
  bool operator==(const VertexInterface &rhs) const {
    const VertexAdaptor *rhs_ptr = dynamic_cast<const VertexAdaptor *>(&rhs);
    if (rhs_ptr) {
      return handle_ == rhs_ptr->handle_;
    }
    printf("dynamic_cast failed\n");
    return false;
  }
private:
  SsPtr ss_;
  Ss::Vertex_const_handle handle_;
  double deltaR_;
};

class EdgeAdaptor : public EdgeInterface {
public:
  EdgeAdaptor(SsPtr ss, Ss::Halfedge_const_handle halfedge, int idx) : ss_(ss), halfedge_(halfedge), idx_(idx) {
    head_vertex_ = std::make_shared<VertexAdaptor>(ss_, halfedge_->vertex());
    tail_vertex_ = std::make_shared<VertexAdaptor>(ss_, halfedge_->opposite()->vertex());
  }
  ~EdgeAdaptor() {}
  VertexPtr &head_vertex() {
    return head_vertex_;
  }
  VertexPtr &tail_vertex() {
    return tail_vertex_;
  }
  int &idx() {
    return idx_;
  }
  bool operator==(const EdgeInterface &rhs) const {
    const EdgeAdaptor *rhs_ptr = dynamic_cast<const EdgeAdaptor *>(&rhs);
    if (rhs_ptr) {
      return halfedge_ == rhs_ptr->halfedge_ || halfedge_ == rhs_ptr->halfedge_->opposite();
    }
    return false;
  }
private:
  SsPtr ss_;
  Ss::Halfedge_const_handle halfedge_;
  VertexPtr head_vertex_;
  VertexPtr tail_vertex_;
  int idx_;
};

std::deque<EdgePtr> VertexAdaptor::edges() {
  printf("VertexAdaptor::edges\n");
  std::deque<EdgePtr> edges;
  for ( Ss::Halfedge_const_iterator i = ss_->halfedges_begin(); i != ss_->halfedges_end(); ++i ) {
    if (i->is_bisector() && i->vertex() == handle_) {
      printf("e: (%f, %f)(%f, %f)\n", i->opposite()->vertex()->point().x(), i->opposite()->vertex()->point().y(),
      i->vertex()->point().x(), i->vertex()->point().y());
      edges.emplace_back(new EdgeAdaptor(ss_, i, 0));
    }
  }
  return edges;
}

void createGraph(SsPtr ss, EdgePtr &root_e, VertexPtr &root_v, bool is_inverse = false) {
  // find the vertex with the maximum time
  Ss::Vertex_const_handle max_vertex = nullptr;
  double max_time = 0;
  for ( Ss::Vertex_const_iterator i = ss->vertices_begin(); i != ss->vertices_end(); ++i ) {
    if (i->time() > max_time) {
      max_time = i->time();
      max_vertex = i;
    }
  }
  Ss::Halfedge_const_handle max_halfedge = max_vertex->halfedge();
  if (is_inverse) {
    max_halfedge = max_halfedge->opposite();
    max_vertex = max_halfedge->vertex();
  }
  root_v = std::make_shared<VertexAdaptor>(ss, max_vertex);
  root_e = std::make_shared<EdgeAdaptor>(ss, max_halfedge, 0);
}

std::vector<double> hierarchicStraightSkeleton(const std::vector<double> &coords, int max_h, double min_r)
{
  Polygon_2 poly ;
  for (size_t i = 0; i < coords.size(); i += 2)
  {
    poly.push_back(Point(coords[i], coords[i + 1]));
    std::cout << " " << coords[i] << " " << coords[i + 1] << std::endl;
  }
  
  // make poly counterclockwise oriented
  if ( poly.is_clockwise_oriented() )
    poly.reverse_orientation() ;
  assert(poly.is_counterclockwise_oriented());

  // You can pass the polygon via an iterator pair
  SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());

  EdgePtr root_e = nullptr;
  VertexPtr root_v = nullptr;
  createGraph(iss, root_e, root_v);
  SkeletonPyramid::setMaxh(max_h);
  SkeletonPyramid::setMinR(min_r);
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
  return result;
}