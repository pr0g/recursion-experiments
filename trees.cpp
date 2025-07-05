#include <format>
#include <iostream>
#include <vector>

struct node_t {
  std::string data;
  std::vector<node_t*> children;
};

void preorder_traversal_recursive(node_t* node) {
  if (node == nullptr) {
    return;
  }
  std::cout << std::format("{}\n", node->data);
  for (const auto child : node->children) {
    preorder_traversal_recursive(child);
  }
}

void preorder_traversal_iterative_recursive(node_t* node) {
  enum class return_address_e { before, recursive };
  struct frame_t {
    return_address_e return_address;
    node_t* node;
    int child_index = 0;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{.return_address = return_address_e::before, .node = node});
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.node == nullptr) {
        call_stack.pop();
        continue;
      }
      std::cout << std::format("{}\n", top.node->data);
      if (top.child_index < top.node->children.size()) {
        top.return_address = return_address_e::recursive;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .node = top.node->children[top.child_index]});
        top.child_index++;
      } else {
        call_stack.pop();
      }
    } else if (top.return_address == return_address_e::recursive) {
      if (top.child_index < top.node->children.size()) {
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .node = top.node->children[top.child_index]});
        top.child_index++;
      } else {
        call_stack.pop();
      }
    }
  }
}

void postorder_traversal_recursive(node_t* node) {
  if (node == nullptr) {
    return;
  }
  for (const auto child : node->children) {
    postorder_traversal_recursive(child);
  }
  std::cout << std::format("{}\n", node->data);
}

// mostly only relevant for binary trees
void inorder_traversal_recursive(node_t* node) {
  if (node == nullptr) {
    return;
  }
  for (int i = 0; i < std::min<int>(1, node->children.size()); i++) {
    inorder_traversal_recursive(node->children[i]);
  }
  std::cout << std::format("{}\n", node->data);
  for (int i = 1; i < node->children.size(); i++) {
    inorder_traversal_recursive(node->children[i]);
  }
}

void reverse_inorder_traversal(node_t* node) {
  if (node == nullptr) {
    return;
  }
  for (int i = 1; i < node->children.size(); i++) {
    reverse_inorder_traversal(node->children[i]);
  }
  std::cout << std::format("{}\n", node->data);
  for (int i = 0; i < std::min<int>(1, node->children.size()); i++) {
    reverse_inorder_traversal(node->children[i]);
  }
}

void depth_first_search(
  node_t* node, const std::function<void(const node_t* node)>& fn) {
  if (node == nullptr) {
    return;
  }
  fn(node);
  for (auto child : node->children) {
    depth_first_search(child, fn);
  }
}

std::vector<std::string> depth_first_search_preorder(node_t* node) {
  if (node == nullptr) {
    return std::vector<std::string>{};
  }
  std::vector<std::string> names;
  if (node->data.size() == 8) {
    names.push_back(node->data);
  }
  for (auto child : node->children) {
    const auto next_names = depth_first_search_preorder(child);
    names.insert(names.end(), next_names.begin(), next_names.end());
  }
  return names;
}

std::vector<std::string> depth_first_search_postorder(node_t* node) {
  if (node == nullptr) {
    return std::vector<std::string>{};
  }
  std::vector<std::string> names;
  for (auto child : node->children) {
    const auto next_names = depth_first_search_postorder(child);
    names.insert(names.end(), next_names.begin(), next_names.end());
  }
  if (node->data.size() == 8) {
    names.push_back(node->data);
  }
  return names;
}

int tree_depth(const node_t* node) {
  if (node == nullptr) {
    return 0;
  }
  int d = 0;
  for (const auto* child : node->children) {
    d = std::max(d, tree_depth(child) + 1);
  }
  return d;
}

void deepen_tree_recursive(node_t* node) {
  if (node == nullptr) {
    return;
  }
  for (const auto child : node->children) {
    deepen_tree_recursive(child);
  }
  // leak, no clean-up currently...
  if (node->children.empty()) {
    node_t* child = new node_t();
    child->data = node->data + " child";
    node->children.push_back(child);
  }
}

int main(int argc, char** argv) {
  {
    //             A
    //           /   \
    //          B     C
    //          |    / \
    //          D   E   F
    //             / \
    //            G  H

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

    std::cout << "recursive:\n";
    preorder_traversal_recursive(&root);
    std::cout << '\n';
    std::cout << "iterative:\n";
    preorder_traversal_iterative_recursive(&root);
    std::cout << '\n';
    std::cout << "recursive:\n";
    postorder_traversal_recursive(&root);
    std::cout << '\n';
    std::cout << "recursive:\n";
    inorder_traversal_recursive(&root);
    std::cout << '\n';
    std::cout << "recursive:\n";
    reverse_inorder_traversal(&root);
    std::cout << '\n';
    std::cout << "recursive:\n";
    deepen_tree_recursive(&root); // add new child nodes to tree
    preorder_traversal_recursive(&root);
  }

  std::cout << '\n';

  {
    node_t root = {.data = "Alice"};
    node_t node2 = {.data = "Bob"};
    node_t node3 = {.data = "Darya"};
    node_t node4 = {.data = "Caroline"};
    node_t node5 = {.data = "Eve"};
    node_t node6 = {.data = "Fred"};
    node_t node7 = {.data = "Gonzalo"};
    node_t node8 = {.data = "Hadassah"};

    root.children = {&node2, &node4};
    node2.children = {&node3};
    node4.children = {&node5, &node6};
    node5.children = {&node7, &node8};

    {
      std::vector<std::string> eightLetterNames;
      depth_first_search(&root, [&eightLetterNames](const node_t* node) {
        if (node->data.size() == 8) {
          eightLetterNames.push_back(node->data);
        }
      });

      for (const auto& eightLetterName : eightLetterNames) {
        std::cout << std::format("Name: {}\n", eightLetterName);
      }
    }

    std::cout << '\n';

    {
      std::vector<std::string> eightLetterNames =
        depth_first_search_preorder(&root);
      for (const auto& eightLetterName : eightLetterNames) {
        std::cout << std::format("Name: {}\n", eightLetterName);
      }
    }

    std::cout << '\n';

    {
      std::vector<std::string> eightLetterNames =
        depth_first_search_postorder(&root);
      for (const auto& eightLetterName : eightLetterNames) {
        std::cout << std::format("Name: {}\n", eightLetterName);
      }
    }

    std::cout << '\n';

    std::cout << std::format("Tree depth is {}\n", tree_depth(&root));
  }

  return 0;
}
