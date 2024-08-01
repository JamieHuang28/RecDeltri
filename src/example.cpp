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

int main() {
  Vertex v1(62.3);
  Vertex v2(19.3);
  Vertex v3(7.1);
  Vertex v4(45.2);

  Edge e1(1);
  Edge e2(2);
  Edge e3(3);
  Edge e4(4);

  e1.tail_vertex = &v1;
  e2.head_vertex = &v1;
  e2.tail_vertex = &v2;
  e3.head_vertex = &v1;
  e3.tail_vertex = &v3;
  e4.head_vertex = &v1;
  e4.tail_vertex = &v4;

  v1.edges.push_back(&e1);
  v1.edges.push_back(&e2);
  v1.edges.push_back(&e3);
  v1.edges.push_back(&e4);

  v2.edges.push_back(&e2);
  v3.edges.push_back(&e3);
  v4.edges.push_back(&e4);

  Vertex *root_v = &v1;
  Edge *root_e = &e1;

  skeletonPyramid(root_v, root_e, 0);
  return 0;
}