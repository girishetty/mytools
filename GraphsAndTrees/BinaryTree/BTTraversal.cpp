/*
 * Binary Tree Traversal
 *
 * Implement following traversal algorithms
 *   - inorder (left, node, right)
 *   - preorder (node, left, right)
 *   - postorder (left, right, node)
 *
 */

#include <stack>
#include <queue>
#include "BTTraversal.h"

class InOrderTraversal : public BTTraversal {
  const TreeNode* root = nullptr;

  void traverse(std::vector<int>& list, bool print) {
    std::stack<const TreeNode*> stack;
    auto node = root;

    while (node != nullptr || !stack.empty()) {
      while (node != nullptr) {
        stack.push(node);
        node = node->left;
      }
      node = stack.top();
      stack.pop();
      list.push_back(node->val);
      if (print) {
        std::cout << node->val << " ";
      }
      node = node->right;
    }
  }

  void traverse() override {
    std::vector<int> list;
    return traverse(list, true);
  }

  void traverse(std::vector<int>& list) override {
    list.clear();
    return traverse(list, false);
  }

public:
  InOrderTraversal(const TreeNode* node) : root(node) {}
};

class InOrderTraversalRec : public BTTraversal {
  const TreeNode* root = nullptr;

  void traverse(const TreeNode* node, std::vector<int>& list, bool print) {
    if (node == nullptr) {
      return;
    }
    traverse(node->left, list, print);
    list.push_back(node->val);
    if (print) {
      std::cout << node->val << " ";
    }
    traverse(node->right, list, print);
  }

  void traverse() override {
    std::vector<int> list;
    return traverse(root, list, true);
  }

  void traverse(std::vector<int>& list) override {
    list.clear();
    return traverse(root, list, false);
  }

public:
  InOrderTraversalRec(const TreeNode* node) : root(node) {}
};

class PreOrderTraversal : public BTTraversal {
  const TreeNode* root = nullptr;

  void traverse(std::vector<int>& list, bool print) {
    std::stack<const TreeNode*> stack;
    auto node = root;

    while (node != nullptr || !stack.empty()) {
      while (node != nullptr) {
        stack.push(node);
        list.push_back(node->val);
        if (print) {
          std::cout << node->val << " ";
        }
        node = node->left;
      }
      node = stack.top();
      stack.pop();
      node = node->right;
    }
  }

  void traverse() override {
    std::vector<int> list;
    return traverse(list, true);
  }

  void traverse(std::vector<int>& list) override {
    list.clear();
    return traverse(list, false);
  }

public:
  PreOrderTraversal(const TreeNode* node) : root(node) {}
};

class PreOrderTraversalRec : public BTTraversal {
  const TreeNode* root = nullptr;

  void traverse(const TreeNode* node, std::vector<int>& list, bool print) {
    if (node == nullptr) {
      return;
    }
    list.push_back(node->val);
    if (print) {
      std::cout << node->val << " ";
    }
    traverse(node->left, list, print);
    traverse(node->right, list, print);
  }

  void traverse() override {
    std::vector<int> list;
    return traverse(root, list, true);
  }

  void traverse(std::vector<int>& list) override {
    list.clear();
    return traverse(root, list, false);
  }

public:
  PreOrderTraversalRec(const TreeNode* node) : root(node) {}
};

class PostOrderTraversal : public BTTraversal {
  const TreeNode* root = nullptr;

  void traverse(std::vector<int>& list, bool print) {
    auto node = root;
    if (node == nullptr) {
      return;
    }

    std::stack<const TreeNode*> stack1;
    std::stack<const TreeNode*> stack2;
    stack1.push(node);

    while (!stack1.empty()) {
      // Pop from stack1 and push it to stack2
      node = stack1.top();
      stack1.pop();
      stack2.push(node);

      // Push left right of node to stack1
      if (node->left != nullptr) {
        stack1.push(node->left);
      }
      if (node->right != nullptr) {
        stack1.push(node->right);
      }
    }

    while (!stack2.empty()) {
      node = stack2.top();
      stack2.pop();
      list.push_back(node->val);
      if (print) {
        std::cout << node->val << " ";
      }
    }
  }

  void traverse() override {
    std::vector<int> list;
    return traverse(list, true);
  }

  void traverse(std::vector<int>& list) override {
    list.clear();
    return traverse(list, false);
  }

public:
  PostOrderTraversal(const TreeNode* node) : root(node) {}
};

class PostOrderTraversalRec : public BTTraversal {
  const TreeNode* root = nullptr;

  void traverse(const TreeNode* node, std::vector<int>& list, bool print) {
    if (node == nullptr) {
      return;
    }
    traverse(node->left, list, print);
    traverse(node->right, list, print);
    list.push_back(node->val);
    if (print) {
      std::cout << node->val << " ";
    }
  }

  void traverse() override {
    std::vector<int> list;
    return traverse(root, list, true);
  }

  void traverse(std::vector<int>& list) override {
    list.clear();
    return traverse(root, list, false);
  }

public:
  PostOrderTraversalRec(const TreeNode* node) : root(node) {}
};

class LevelOrderTraversal : public BTTraversal {
  const TreeNode* root = nullptr;

  void traverse(std::vector<int>& list, bool print) {
    auto node = root;
    if (node == nullptr) {
      return;
    }
    std::queue<const TreeNode*> queue;
    queue.push(node);

    while (!queue.empty()) {
      node = queue.front();
      queue.pop();
      list.push_back(node->val);
      if (print) {
        std::cout << node->val << " ";
      }
      // Push left right of node to queue
      if (node->left != nullptr) {
        queue.push(node->left);
      }
      if (node->right != nullptr) {
        queue.push(node->right);
      }
    }
  }

  void traverse() override {
    std::vector<int> list;
    return traverse(list, true);
  }

  void traverse(std::vector<int>& list) override {
    list.clear();
    return traverse(list, false);
  }

public:
  LevelOrderTraversal(const TreeNode* node) : root(node) {}
};

std::unique_ptr<BTTraversal> BTTraversal::createBTTraversal(BTTraversalType type, const TreeNode* root) {
  switch (type) {
  case InOrder:
    return std::make_unique<InOrderTraversal>(root);
  case InOrderRec:
    return std::make_unique<InOrderTraversalRec>(root);
  case PreOrder:
    return std::make_unique<PreOrderTraversal>(root);
  case PreOrderRec:
    return std::make_unique<PreOrderTraversalRec>(root);
  case PostOrder:
    return std::make_unique<PostOrderTraversal>(root);
  case PostOrderRec:
    return std::make_unique<PostOrderTraversalRec>(root);
  case LevelOrder:
    return std::make_unique<LevelOrderTraversal>(root);
  }

  return nullptr;
}
