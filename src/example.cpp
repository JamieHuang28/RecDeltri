#include <iostream>
#include <string>
#include "hierarchic_skeleton.hpp"

// struct Data {
//   int level;
// };

// class BinaryTree {
// public:
//   BinaryTree(const std::string &name) : name_(name) {
//     std::cout << "BinaryTree constructor" << std::endl;
//   }
//   ~BinaryTree() {
//     std::cout << "BinaryTree destructor" << std::endl;
//   }
//   void execute() {
//     std::cout << name_ << ": " << data.level << std::endl;
//   }
//   BinaryTree *left;
//   BinaryTree *right;
//   Data data;
// private:
//   std::string name_;
// };

// void precedenceWiseTraversal(BinaryTree *root, int level = 0) {
//   root->data.level = level;
//   root->execute();
//   std::vector<BinaryTree *> childrens;
//   if (root->left) {
//     childrens.push_back(root->left);
//   }
//   if (root->right) {
//     childrens.push_back(root->right);
//   }
//   for (auto &child : childrens) {
//     precedenceWiseTraversal(child, level);
//     level++;
//   }
// }

// void testBinaryTree() {
//   BinaryTree *root;
//   root = new BinaryTree("root");
//   root->left = new BinaryTree("left");
//   root->right = new BinaryTree("right");
//   root->left->left = new BinaryTree("left-left");
//   root->left->right = new BinaryTree("left-right");
//   root->right->left = new BinaryTree("right-left");
//   root->right->right = new BinaryTree("right-right");

//   precedenceWiseTraversal(root);
// }

class Vertex : public VertexInterface {
public:
  Vertex(double deltaR, double x, double y, std::vector<EdgePtr> edges);
  ~Vertex();
  std::deque<EdgePtr> edges() {
    return edges_;
  }
  double &deltaR() {
    return deltaR_;
  }
  const double &x() {
    return x_;
  }
  const double &y() {
    return y_;
  }
  bool operator==(const VertexInterface &rhs) const {
    const Vertex *rhs_ptr = dynamic_cast<const Vertex *>(&rhs);
    if (rhs_ptr) {
      bool result = x_ == rhs_ptr->x_ && y_ == rhs_ptr->y_;
      if (result) {
        printf("(%f, %f) == (%f, %f)\n", x_, y_, rhs_ptr->x_, rhs_ptr->y_);
      } else {
        printf("(%f, %f) != (%f, %f)\n", x_, y_, rhs_ptr->x_, rhs_ptr->y_);
      }
      return x_ == rhs_ptr->x_ && y_ == rhs_ptr->y_;
    }
    printf("dynamic_cast failed\n");
    return false;
  }
private:
  std::deque<EdgePtr> edges_;
  double deltaR_;
  double x_;
  double y_;
  friend class Edge;
};

Vertex::Vertex(double deltaR, double x, double y, std::vector<EdgePtr> edges) : deltaR_(deltaR), x_(x), y_(y)
{
  std::copy(edges.begin(), edges.end(), std::back_inserter(edges_));
}
Vertex::~Vertex()
{
}

class Edge : public EdgeInterface
{
public:
  Edge(int idx);
  ~Edge();
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
    const Edge *rhs_ptr = dynamic_cast<const Edge *>(&rhs);
    if (rhs_ptr) {
      return idx_ == rhs_ptr->idx_;
    }
    return false;
  }
private:
  VertexPtr head_vertex_;
  VertexPtr tail_vertex_;
  int idx_;
};

Edge::Edge(int idx) : idx_(idx)
{
}

Edge::~Edge()
{
}

int main() {
  EdgePtr e1_ptr = std::make_shared<Edge>(1);
  EdgePtr e2_ptr = std::make_shared<Edge>(2);
  EdgePtr e3_ptr = std::make_shared<Edge>(3);
  EdgePtr e4_ptr = std::make_shared<Edge>(4);

  VertexPtr v0_ptr = std::make_shared<Vertex>(0.0, 0.0, 0.0, std::vector<EdgePtr>({e1_ptr}));
  VertexPtr v1_ptr = std::make_shared<Vertex>(62.3, 0.0, 1.0, std::vector<EdgePtr>({e1_ptr, e2_ptr, e3_ptr, e4_ptr}));
  VertexPtr v2_ptr = std::make_shared<Vertex>(19.3, 1.0, 1.0, std::vector<EdgePtr>({e2_ptr}));
  VertexPtr v3_ptr = std::make_shared<Vertex>(7.1, 2.0, 2.0, std::vector<EdgePtr>({e3_ptr}));
  VertexPtr v4_ptr = std::make_shared<Vertex>(45.2, 2.0, 1.0, std::vector<EdgePtr>({e4_ptr}));

  e1_ptr->head_vertex() = v0_ptr;
  // v0_ptr->edges().push_back(e1_ptr);
  e1_ptr->tail_vertex() = v1_ptr;
  // v1_ptr->edges().push_back(e1_ptr);

  e2_ptr->head_vertex() = v1_ptr;
  // v1_ptr->edges().push_back(e2_ptr);
  e2_ptr->tail_vertex() = v2_ptr;
  // v2_ptr->edges().push_back(e2_ptr);

  e3_ptr->head_vertex() = v1_ptr;
  // v1_ptr->edges().push_back(e3_ptr);
  e3_ptr->tail_vertex() = v3_ptr;
  // v3_ptr->edges().push_back(e3_ptr);
  
  e4_ptr->head_vertex() = v1_ptr;
  // v1_ptr->edges().push_back(e4_ptr);
  e4_ptr->tail_vertex() = v4_ptr;
  // v4_ptr->edges().push_back(e4_ptr);

  SkeletonPyramid::skeletonPyramid(v1_ptr, e1_ptr, 0);
  for (const EdgePtr &edge : SkeletonPyramid::skeleton()) {
    std::cout << edge->idx() << std::endl;
  }
  return 0;
}