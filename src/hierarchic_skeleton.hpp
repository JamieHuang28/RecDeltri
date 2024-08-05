#include <deque>
#include <vector>
#include <algorithm>
#include <memory>

class EdgeInterface;
typedef std::shared_ptr<EdgeInterface> EdgePtr;
class VertexInterface {
public:
  virtual ~VertexInterface() {}
  virtual std::deque<EdgePtr> &edges() = 0;
  virtual double &deltaR() = 0;
};
typedef std::shared_ptr<VertexInterface> VertexPtr;

class EdgeInterface {
public:
  virtual ~EdgeInterface() {}
  virtual std::vector<EdgePtr> &head_edges() = 0;
  virtual std::vector<EdgePtr> &tail_edges() = 0;
  virtual VertexPtr &head_vertex() = 0;
  virtual VertexPtr &tail_vertex() = 0;
  virtual int &idx() = 0;
};

typedef std::pair<EdgePtr, double> R_tab_elem;
bool operator<(const R_tab_elem &lhs, const R_tab_elem &rhs) {
  return lhs.second > rhs.second; // inverse order
}

EdgePtr VRot(VertexPtr v_i, EdgePtr e_ref) {
  if (v_i->edges().empty()) {
    return nullptr;
  }

  EdgePtr e = v_i->edges().front();
  while (e != e_ref) {
    v_i->edges().pop_front();
    v_i->edges().push_back(e);
    e = v_i->edges().front();
  } // unsafe, need to check if e_ref is in v_i->edges() elsewhere endless loop
  v_i->edges().pop_front();
  return v_i->edges().front();
}

class SkeletonPyramid {
public:
  ~SkeletonPyramid() {}
  static void skeletonPyramid(VertexPtr v_i, EdgePtr e_cur, int h);
  static std::vector<EdgePtr> &skeleton() {
    return skeleton_;
  }
private:
  static const int MAX_H = 2;
  static std::vector<EdgePtr> skeleton_;
};
std::vector<EdgePtr> SkeletonPyramid::skeleton_{};

void SkeletonPyramid::skeletonPyramid(VertexPtr v_i, EdgePtr e_cur, int h) {
  EdgePtr e_ref = e_cur;
  if (h < MAX_H) {
    skeleton_.push_back(e_ref);
  }
  std::vector<R_tab_elem> R_tab;
  e_cur = VRot(v_i, e_ref);
  while (e_cur != e_ref && e_cur != nullptr) {
    printf("e_cur: %d\n", e_cur->idx());
    double r = std::numeric_limits<double>::max();
    if (e_cur->head_vertex() == v_i) {
      r = e_cur->tail_vertex()->deltaR();
    } else {
      r = e_cur->head_vertex()->deltaR();
    }
    R_tab.push_back(std::make_pair(e_cur, r));
    e_cur = VRot(v_i, e_cur);
  }

  std::sort(R_tab.begin(), R_tab.end());
  for (auto &elem : R_tab) {
    EdgePtr e_cur = elem.first;
    if (e_cur->head_vertex() == v_i) {
      skeletonPyramid(e_cur->tail_vertex(), e_cur, h);
    } else {
      skeletonPyramid(e_cur->head_vertex(), e_cur, h);
    }

    if (elem.second != std::numeric_limits<double>::max()) {
      h = h + 1;
    }
  }
}