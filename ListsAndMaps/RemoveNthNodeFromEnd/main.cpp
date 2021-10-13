/*
 * https://leetcode.com/problems/remove-nth-node-from-end-of-list/
 *
 * Given the head of a linked list, remove the nth node from the end of the list and return its head.
 *
 * Input: head = [1,2,3,4,5], n = 2
 * Output: [1,2,3,5]
 *
 * Input: head = [1], n = 1
 * Output: []
 * Example 3:
 * 
 * Input: head = [1,2], n = 1
 * Output: [1]
 */

#include <vector>
#include <stack>
#include <iostream>

/**
 * Definition for singly-linked list.
 */
struct Node {
  Node(int x = 0, Node* next = nullptr) : val_(x), next_(next) {}
  int val_;
  Node* next_;
};

Node* removeNthFromEnd(Node* head, int n) {
  Node* node = head;
  std::stack<Node*> stack;

  for (; node != nullptr; node = node->next_) {
    stack.push(node);
  }
  
  while (!stack.empty() && n > 0) {
    node = stack.top();
    stack.pop();
    n--;
  }
  
  if (node == head) {
    if (node == nullptr) {
      std::cout << "Invalid List" << std::endl;
      return nullptr;
    }
    Node* new_head = head->next_;
    delete head;
    return new_head;
  }

  Node* prev = stack.top();
  prev->next_ = node->next_;
  delete node;
  
  return head;
}

void displayList(const char* what, const Node* node) {
  std::cout << what << "[ ";
  for (; node != nullptr; node = node->next_) {
    std::cout << node->val_ << " ";
  }
  std::cout << "]" << std::endl;
}

void deleteLinkedList(const Node* node) {
  for (; node != nullptr; ) {
    const Node* todelete = node;
    node = node->next_;
    delete todelete;
  }
}

Node* createListFromVector(const std::vector<int>& list) {
  Node* head = nullptr;
  Node* prev = nullptr;

  for (auto item : list) {
    Node* node = new Node(item);
    if (prev == nullptr) {
      head = node;
    } else {
      prev->next_ = node;
    }
    prev = node;
  }

  return head;
}

void testRemoveNthFromEnd(const std::vector<int>& list, int n) {
  Node* head = createListFromVector(list);
  displayList("Initial List", head);
  head = removeNthFromEnd(head, n);
  std::cout << "Removed " << n << "th element from the Last" << std::endl;
  displayList("Updated List", head);
  deleteLinkedList(head);
}

int main() {
  testRemoveNthFromEnd({1,2,3,4,5}, 2);
  testRemoveNthFromEnd({1}, 1);
  testRemoveNthFromEnd({1, 2}, 2);
  testRemoveNthFromEnd({1, 2}, 1);
  return 0;
}
