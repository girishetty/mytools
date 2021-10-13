/*
 * https://leetcode.com/problems/add-two-numbers/
 *
 * You are given two non-empty linked lists representing two non-negative integers.
 * The digits are stored in reverse order, and each of their nodes contains a single digit.
 * Add the two numbers and return the sum as a linked list.
 * 
 * You may assume the two numbers do not contain any leading zero, except the number 0 itself.
 *
 * Example#1:
 *   Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
 *   Output: [8,9,9,9,0,0,0,1]
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

inline Node* sumUpAndCreateNode(Node* node, int val, int& carry) {
  int sum = val + carry;
  if (sum > 9) {
    carry = 1;
    sum = sum - 10;
  } else {
    carry = 0;
  }
  node->next_ = new Node(sum);
  return node->next_;
}

Node* addTwoNumbers(Node* l1, Node* l2) {
  Node* res = new Node(0);
  Node* resHead = res;

  int carry = 0;
  for (; l1 != nullptr && l2 != nullptr; l1 = l1->next_, l2 = l2->next_) {
    res = sumUpAndCreateNode(res, l1->val_ + l2->val_, carry);
  }

  Node* rem = (l1 != nullptr) ? l1 : l2;
  for (; rem != nullptr;  rem = rem->next_) {
    res = sumUpAndCreateNode(res, rem->val_, carry);
  }

  if (carry > 0) {
    res->next_ = new Node(carry);
  }

  if (resHead->next_ != nullptr) {
    res = resHead->next_;
    delete resHead;
  } else {
    res = resHead;
  }

  return res;
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

void testAddTwoNumbers(const std::vector<int>& list1, const std::vector<int>& list2) {
  Node* l1 = createListFromVector(list1);
  Node* l2 = createListFromVector(list2);
  displayList("Number#1", l1);
  displayList("Number#2", l2);
  Node* addedList = addTwoNumbers(l1, l2);
  displayList("Added Number", addedList);
  deleteLinkedList(addedList);
  deleteLinkedList(l1);
  deleteLinkedList(l2);
}

int main() {
  testAddTwoNumbers({2, 4, 3}, {5, 6, 4});
  testAddTwoNumbers({9, 9, 9, 9, 9, 9, 9}, {9, 9, 9, 9});

  return 0;
}
