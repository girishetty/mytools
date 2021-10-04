/*
 * https://leetcode.com/problems/copy-list-with-random-pointer/
 *
 * A linked list of length n is given such that each node contains an additional random pointer, which could point to any node in the list, or null.
 *
 * Construct a deep copy of the list. The deep copy should consist of exactly n brand new nodes, where each new node has its value set to the value
 * of its corresponding original node. Both the next and random pointer of the new nodes should point to new nodes in the copied list such that
 * the pointers in the original list and copied list represent the same list state. None of the pointers in the new list should point to nodes in the original list.
 *
 * For example, if there are two nodes X and Y in the original list, where X.random --> Y, then for the corresponding two nodes x and y in the copied list, x.random --> y.
 *
 * Return the head of the copied linked list.
 *
 * The linked list is represented in the input/output as a list of n nodes. Each node is represented as a pair of [val, random_index] where:
 *  - val: an integer representing Node.val
 *  - random_index: the index of the node (range from 0 to n-1) that the random pointer points to, or null if it does not point to any node.
 *
 * Your code will only be given the head of the original linked list.
 *
 * Example#1:
 *  Input: head = [[7,null],[13,0],[11,4],[10,2],[1,0]]
 *  Output: [[7,null],[13,0],[11,4],[10,2],[1,0]]
 *
 * Example#2:
 *  Input: head = [[3,null],[3,0],[3,null]]
 *  Output: [[3,null],[3,0],[3,null]]
 */

#include <vector>
#include <iostream>
#include <unordered_map>

class Node {
public:
  int val = 0;
  Node* next = nullptr;
  Node* random = nullptr;

  Node(int _val, Node* n = nullptr, Node* r = nullptr) : val(_val), next(n), random(r) {}
};

class Solution {
private:
  std::unordered_map<Node*, Node*> maps;

  inline Node* createCopy(Node* node) {
    if (node == nullptr) {
      return nullptr;
    }
    auto found = maps.find(node);
    if (found != maps.end()) {
      return found->second;
    }
    Node* new_node = new Node(node->val);
    maps[node] = new_node;
    return new_node;
  }

public:
  Node* copyRandomList(Node* head) {
    if (head == nullptr) {
      return nullptr;
    }

    Node* new_head = createCopy(head);

    for (Node* node = head; node != nullptr; node = node->next) {
      Node* new_node = createCopy(node);
      new_node->next = createCopy(node->next);
      new_node->random = createCopy(node->random);
    }

    return new_head;
  }
};

class Problem {
private:
  std::unordered_map<int, Node*> maps;

  inline Node* createNode(int index, int val) {
    if (index == -1) {
      return nullptr;
    }
    auto found = maps.find(index);
    if (found != maps.end()) {
      return found->second;
    }
    Node* new_node = new Node(val);
    maps[index] = new_node;
    return new_node;
  }

public:
  Node* createLinkedList(const std::vector<std::vector<int>>& list) {
    if (list.empty()) {
      return nullptr;
    }

    Node* head = createNode(0, list[0][0]);
    Node* node = head;
    for (int i = 1; i < list.size(); i++) {
      node->next = createNode(i, list[i][0]);
      node->random = createNode(list[i][1], 0);
      node = node->next;
    }

    return head;
  }
};

void testDeepCopyLinkedList(const std::vector<std::vector<int>>& list) {
  Problem p;
  Solution s;
  Node* head1 = p.createLinkedList({{7,-1},{13,0},{11,4},{10,2},{1,0}});
  Node* copy_head1 = s.copyRandomList(head1);

  while (head1 != nullptr && copy_head1 != nullptr) {
    if (head1->val == copy_head1->val &&
        ((head1->random == nullptr && copy_head1->random == nullptr) ||
         (head1->random != nullptr && copy_head1->random != nullptr &&
          head1->random->val == copy_head1->random->val))) {
      head1 = head1->next;
      copy_head1 = copy_head1->next;
    } else {
      break;
    }
  }

  if (head1 == nullptr && copy_head1 == nullptr) {
    std::cout << "Deep Copy of Linked List Success" << std::endl;
  } else {
    std::cout << "Deep Copy of Linked List Failed" << std::endl;
  }
}

int main() {
  testDeepCopyLinkedList({{7,-1},{13,0},{11,4},{10,2},{1,0}});
  return 0;
}
