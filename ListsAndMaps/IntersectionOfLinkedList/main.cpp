/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/107/linked-list/785/
 *
 * Given the heads of two singly linked-lists headA and headB, return the node at which the
 * two lists intersect. If the two linked lists have no intersection at all, return null.
 *
 * Note that the linked lists must retain their original structure after the function returns.
 *
 * Example#1:
 *    Input: listA = [4,1,8,4,5], listB = [5,6,1,8,4,5]
 *    Output: Intersected at '1'
 *
 * Example#2:
 *    Input: listA = [1,9,1,2,4], listB = [3,2,4]
 *    Output: Intersected at '2'
 *
 * Example#3:
 *    Input: listA = [1,2,4], listB = [3,5]
 *    Output: No intersection
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

int getLength(const Node* node) {
  int len = 0;
  for (; node != nullptr; node = node->next_, len++);

  return len;
}
    
const Node* moveList(const Node* node, int steps) {
  for (; node != nullptr && steps > 0; node = node->next_, steps--);

  return node;
}
    
const Node* getIntersectionNode(const Node* headA, const Node* headB) {
  int lenA = getLength(headA);
  int lenB = getLength(headB);

  int diff = lenA - lenB;
  if (diff > 0) {
    headA = moveList(headA, diff);
  } else if (diff < 0) {
    headB = moveList(headB, -diff);
  }

  for (; headA != headB; headA = headA->next_, headB = headB->next_);

  return headA;
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

void testGetIntersectionNode(const std::vector<int>& list1, const std::vector<int>& list2, int intersect) {
  Node* l1 = createListFromVector(list1);
  Node* l2 = createListFromVector(list2);

  Node* common = l1;
  for (; common != nullptr && intersect > 0; intersect--, common = common->next_);

  Node* l2End = l2;
  for (; l2End->next_ != nullptr; l2End = l2End->next_);
  l2End->next_ = common;

  displayList("Number#1", l1);
  displayList("Number#2", l2);
  const Node* intersection = getIntersectionNode(l1, l2);
  if (intersection != nullptr) {
    std::cout << "List 1 and List 2 intersect at: " << intersection->val_ << std::endl;
  } else {
    std::cout << "List 1 and List 2 deosn't intersect" << std::endl;
  }
  deleteLinkedList(l1);
  l2End->next_ = nullptr;
  deleteLinkedList(l2);
}

int main() {
  testGetIntersectionNode({4,1,8,4,5}, {5,6}, 1);
  testGetIntersectionNode({1,9,1,2,4}, {3}, 3);
  testGetIntersectionNode({1,2,4}, {3,5}, 3);

  return 0;
}
