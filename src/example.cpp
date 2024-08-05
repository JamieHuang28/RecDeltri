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
  Vertex(double deltaR);
  ~Vertex();
  std::deque<EdgePtr> &edges() {
    return edges_;
  }
  double &deltaR() {
    return deltaR_;
  }
private:
  std::deque<EdgePtr> edges_;
  double deltaR_;
};

Vertex::Vertex(double deltaR) : deltaR_(deltaR)
{
}
Vertex::~Vertex()
{
}

class Edge : public EdgeInterface
{
public:
  Edge(int idx);
  ~Edge();
  std::vector<EdgePtr> &head_edges() {
    return head_edges_;
  }
  std::vector<EdgePtr> &tail_edges() {
    return tail_edges_;
  }
  VertexPtr &head_vertex() {
    return head_vertex_;
  }
  VertexPtr &tail_vertex() {
    return tail_vertex_;
  }
  int &idx() {
    return idx_;
  }
private:
  std::vector<EdgePtr> head_edges_;
  std::vector<EdgePtr> tail_edges_;
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
  Vertex v1(62.3);
  Vertex v2(19.3);
  Vertex v3(7.1);
  Vertex v4(45.2);

  VertexPtr v1_ptr = std::make_shared<Vertex>(v1);
  VertexPtr v2_ptr = std::make_shared<Vertex>(v2);
  VertexPtr v3_ptr = std::make_shared<Vertex>(v3);
  VertexPtr v4_ptr = std::make_shared<Vertex>(v4);

  Edge e1(1);
  Edge e2(2);
  Edge e3(3);
  Edge e4(4);

  EdgePtr e1_ptr = std::make_shared<Edge>(e1);
  EdgePtr e2_ptr = std::make_shared<Edge>(e2);
  EdgePtr e3_ptr = std::make_shared<Edge>(e3);
  EdgePtr e4_ptr = std::make_shared<Edge>(e4);

  e1_ptr->tail_vertex() = v1_ptr;
  e2_ptr->head_vertex() = v1_ptr;
  e2_ptr->tail_vertex() = v2_ptr;
  e3_ptr->head_vertex() = v1_ptr;
  e3_ptr->tail_vertex() = v3_ptr;
  e4_ptr->head_vertex() = v1_ptr;
  e4_ptr->tail_vertex() = v4_ptr;

  v1_ptr->edges().push_back(e1_ptr);
  v1_ptr->edges().push_back(e2_ptr);
  v1_ptr->edges().push_back(e3_ptr);
  v1_ptr->edges().push_back(e4_ptr);

  v2_ptr->edges().push_back(e2_ptr);
  v3_ptr->edges().push_back(e3_ptr);
  v4_ptr->edges().push_back(e4_ptr);

  skeletonPyramid(v1_ptr, e1_ptr, 0);
  for (const EdgePtr &edge : skeleton) {
    std::cout << edge->idx() << std::endl;
  }
  return 0;
}