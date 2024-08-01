#include <deque>
#include <vector>
#include <algorithm>

class Edge;

class Vertex {
public:
  Vertex(double deltaR);
  ~Vertex();
  std::deque<Edge*> edges;
  double deltaR;
  bool operator<(const Vertex& rhs) const {
    return deltaR < rhs.deltaR;
  }
};

Vertex::Vertex(double deltaR) : deltaR(deltaR)
{
}
Vertex::~Vertex()
{
}

class Edge
{
public:
  Edge(int idx);
  ~Edge();
  std::vector<Edge*> head_edges;
  std::vector<Edge*> tail_edges;
  Vertex* head_vertex;
  Vertex* tail_vertex;
  int idx;
};

Edge::Edge(int idx) : idx(idx)
{
}

Edge::~Edge()
{
}

typedef std::pair<Edge *, double> R_tab_elem;
bool operator<(const R_tab_elem &lhs, const R_tab_elem &rhs) {
  return lhs.second < rhs.second;
}

Edge *VRot(Vertex* v_i, Edge* e_ref) {
  if (v_i->edges.empty()) {
    return nullptr;
  }

  Edge *e = v_i->edges.front();
  while (e != e_ref) {
    v_i->edges.pop_front();
    v_i->edges.push_back(e);
    e = v_i->edges.front();
  } // unsafe, need to check if e_ref is in v_i->edges elsewhere endless loop
  v_i->edges.pop_front();
  return v_i->edges.front();
}

void skeletonPyramid(Vertex *v_i, Edge *e_cur, int h) {
  printf("h: %d\n", h);
  Edge *e_ref = e_cur;
  std::vector<R_tab_elem> R_tab;
  e_cur = VRot(v_i, e_ref);
  while (e_cur != e_ref && e_cur != nullptr) {
    printf("e_cur: %d\n", e_cur->idx);
    double r = std::numeric_limits<double>::max();
    if (e_cur->head_vertex == v_i) {
      r = e_cur->tail_vertex->deltaR;
    } else {
      r = e_cur->head_vertex->deltaR;
    }
    R_tab.push_back(std::make_pair(e_cur, r));
    e_cur = VRot(v_i, e_cur);
  }

  std::sort(R_tab.begin(), R_tab.end());
  for (auto &elem : R_tab) {
    Edge *e_cur = elem.first;
    if (e_cur->head_vertex == v_i) {
      skeletonPyramid(e_cur->tail_vertex, e_cur, h);
    } else {
      skeletonPyramid(e_cur->head_vertex, e_cur, h);
    }

    if (elem.second != std::numeric_limits<double>::max()) {
      h = h + 1;
    }
  }
}