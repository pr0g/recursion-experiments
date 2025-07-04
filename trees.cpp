#include <format>
#include <iostream>
#include <vector>

struct node_t {
  std::string data;
  std::vector<node_t*> children;
};

void preorder_traversal(node_t* node) {
  if (node == nullptr) {
    return;
  }
  std::cout << std::format("{}\n", node->data);
  for (const auto child : node->children) {
    preorder_traversal(child);
  }
}

void postorder_traversal(node_t* node) {
  if (node == nullptr) {
    return;
  }
  for (const auto child : node->children) {
    postorder_traversal(child);
  }
  std::cout << std::format("{}\n", node->data);
}

// mostly only relevant for binary trees
void inorder_traversal(node_t* node) {
  if (node == nullptr) {
    return;
  }
  for (int i = 0; i < std::min<int>(1, node->children.size()); i++) {
    inorder_traversal(node->children[i]);
  }
  std::cout << std::format("{}\n", node->data);
  for (int i = 1; i < node->children.size(); i++) {
    inorder_traversal(node->children[i]);
  }
}

int main(int argc, char** argv) {

  node_t root = {.data = "A"};
  node_t node2 = {.data = "B"};
  node_t node3 = {.data = "C"};
  node_t node4 = {.data = "D"};
  node_t node5 = {.data = "E"};
  node_t node6 = {.data = "F"};
  node_t node7 = {.data = "G"};
  node_t node8 = {.data = "H"};

  root.children = {&node2, &node3};
  node2.children = {&node4};
  node3.children = {&node5, &node6};
  node5.children = {&node7, &node8};

  preorder_traversal(&root);
  std::cout << '\n';
  postorder_traversal(&root);
  std::cout << '\n';
  inorder_traversal(&root);

  return 0;
}
