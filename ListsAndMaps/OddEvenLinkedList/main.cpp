/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/107/linked-list/784/
 *
 * Given the head of a singly linked list, group all the nodes with odd indices together
 * followed by the nodes with even indices, and return the reordered list.
 *
 * The first node is considered odd, and the second node is even, and so on.
 * Note that the relative order inside both the even and odd groups should remain as it was in the input.
 * You must solve the problem in O(1) extra space complexity and O(n) time complexity.
 *
 * Example#1:
 *    Input: {1,2,3,4,5}
 *    Output: {1,3,5,2,4}
 *
 * Example#2:
 *    Input: {2,1,3,5,6,4,7}
 *    Output: {2,3,6,7,1,5,4}
 *
 */

#include <vector>
#include <iostream>

/**
 * Definition for singly-linked list.
 */
struct Node {
  Node(int x = 0, Node* next = nullptr) : val_(x), next_(next) {}
  int val_;
  Node* next_;
};

Node* oddEvenList(Node* head) {
  if (head == nullptr || head->next_ == nullptr || head->next_->next_ == nullptr) {
    return head;
  }
  Node* odd = head;
  Node* even = head->next_;
  Node* evenHead = even;

  while (even != nullptr) {
    odd->next_ = even->next_;
    if (odd->next_ != nullptr) {
      odd = odd->next_;
    }
    even->next_ = odd->next_;
    even = even->next_;
  }

  odd->next_ = evenHead;
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

void testOddEvenList(const std::vector<int>& list) {
  Node* l1 = createListFromVector(list);

  displayList("Input", l1);
  Node* oddEven = oddEvenList(l1);
  displayList("Odd Even List", oddEven);
  deleteLinkedList(oddEven);
}

int main() {
  testOddEvenList({1, 2, 3, 4, 5});
  testOddEvenList({2,1,3,5,6,4,7});

  return 0;
}
