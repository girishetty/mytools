/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/93/linked-list/771/
 *
 * Merge two sorted linked lists and return it as a sorted list. The list should be made by splicing together the nodes of the first two lists.
 *
 * 
 *
 * Example 1:
 *  Input: l1 = [1,2,4], l2 = [1,3,4]
 *  Output: [1,1,2,3,4,4]
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

Node* mergeTwoLists(Node* l1, Node* l2) {
  if (l1 == nullptr) {
    return l2;
  }

  if (l2 == nullptr) {
    return l1;
  }

  Node* nodeHead = nullptr;
  if (l1->val_ <= l2->val_) {
    nodeHead = l1;
    l1 = l1->next_;
  } else {
    nodeHead = l2;
    l2 = l2->next_;
  }

  Node* node = nodeHead;
  while (l1 != nullptr && l2 != nullptr) {
    if (l1->val_ <= l2->val_) {
      node->next_ = l1;
      l1 = l1->next_;
    } else {
      node->next_ = l2;
      l2 = l2->next_;
    }
    node = node->next_;
  }
  if (l1 != nullptr) {
    node->next_ = l1;
  } else {
    node->next_ = l2;
  }

  return nodeHead;
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

void testMergeLinkedList(const std::vector<int>& list1, const std::vector<int>& list2) {
  Node* l1 = createListFromVector(list1);
  Node* l2 = createListFromVector(list2);
  displayList("List#1", l1);
  displayList("List#2", l2);
  Node* mergedHead = mergeTwoLists(l1, l2);
  displayList("Merged List", mergedHead);
  deleteLinkedList(mergedHead);
}

int main() {
  testMergeLinkedList({1, 2, 4}, {1, 3, 4});
  testMergeLinkedList({-11, -1, 0, 1, 4, 7, 9, 15}, {1, 3, 4, 6, 8, 10, 11, 20, 23, 99, 100});

  return 0;
}
