#pragma once

#include <deque>
#include <vector>
#include <algorithm>
#include <memory>

class EdgeInterface;
typedef std::shared_ptr<EdgeInterface> EdgePtr;
class VertexInterface {
public:
  virtual ~VertexInterface() {}
  virtual std::deque<EdgePtr> edges() = 0;
  virtual double &deltaR() = 0;
  virtual const double &x() = 0;
  virtual const double &y() = 0;
  virtual bool operator==(const VertexInterface &rhs) const = 0;
};
typedef std::shared_ptr<VertexInterface> VertexPtr;

class EdgeInterface {
public:
  virtual ~EdgeInterface() {}
  virtual VertexPtr &head_vertex() = 0;
  virtual VertexPtr &tail_vertex() = 0;
  virtual int &idx() = 0;
  virtual bool operator==(const EdgeInterface &rhs) const = 0;
};

typedef std::pair<EdgePtr, double> R_tab_elem;
bool operator<(const R_tab_elem &lhs, const R_tab_elem &rhs) {
  return lhs.second > rhs.second; // inverse order
}

EdgePtr VRot(VertexPtr v_i, EdgePtr e_ref) {
  std::deque<EdgePtr> edges = v_i->edges();
  printf("VRot: edges.size(): %lu\n", edges.size());
  if (edges.empty()) {
    return nullptr;
  }

  EdgePtr e = edges.front();
  size_t i = 0;
  while (!(*e == *e_ref)) {
    if (++i == edges.size()) {
      printf("VRot: edge not found\n");
      return nullptr;
    }
    edges.pop_front();
    edges.push_back(e);
    e = edges.front();
  }
  edges.pop_front();
  edges.push_back(e);
  return edges.front();
}

class SkeletonPyramid {
public:
  ~SkeletonPyramid() {}
  static void skeletonPyramid(VertexPtr v_i, EdgePtr e_cur, int h);
  static std::vector<EdgePtr> &skeleton() {
    return skeleton_;
  }
private:
  static const int MAX_H = 5;
  static std::vector<EdgePtr> skeleton_;
};
std::vector<EdgePtr> SkeletonPyramid::skeleton_{};

void SkeletonPyramid::skeletonPyramid(VertexPtr v_i, EdgePtr e_cur, int h) {
  if (v_i == nullptr || e_cur == nullptr) {
    return;
  }
  EdgePtr e_ref = e_cur;
  printf("h: %d\n", h);
  if (h < MAX_H) {
    skeleton_.push_back(e_ref);
  }
  std::vector<R_tab_elem> R_tab;
  e_cur = VRot(v_i, e_ref);
  while (e_cur != nullptr && !(*e_cur == *e_ref)) {
    printf("e_cur: %d\n", e_cur->idx());
    double r = std::numeric_limits<double>::max();
    if (*e_cur->head_vertex() == *v_i) {
      r = e_cur->tail_vertex()->deltaR();
    } else {
      r = e_cur->head_vertex()->deltaR();
    }
    R_tab.push_back(std::make_pair(e_cur, r));
    e_cur = VRot(v_i, e_cur);
  }

  if (R_tab.empty()) {
    return;
  }
  std::sort(R_tab.begin(), R_tab.end());
  for (auto &elem : R_tab) {
    EdgePtr e_cur = elem.first;
    if (*e_cur->head_vertex() == *v_i) {
      skeletonPyramid(e_cur->tail_vertex(), e_cur, h);
    } else {
      skeletonPyramid(e_cur->head_vertex(), e_cur, h);
    }

    if (elem.second != std::numeric_limits<double>::max()) {
      h = h + 1;
    }
  }
}