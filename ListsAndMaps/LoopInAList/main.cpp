/*
 *
 * C++ program to detect loop in a linked list
 *
 */

#include <iostream>

// Link list node
struct Node {
  Node(int d = 0, Node* n = nullptr) : data(d), next(n) {}
  int data;
  Node* next;
};

void push(Node** head_ref, int new_data) {
  if (head_ref) {
    *head_ref = new Node(new_data, *head_ref);
  }
}

// Print all the nodes.
// An optional argument that limits how many nodes we can print
void printList(const Node* node, int node_count = -1) {
  std::cout << "The List is: ";

  while (node != nullptr) {
    std::cout << node->data << " ";
    node = node->next;
    node_count--;
    if (node_count == 0) {
      std::cout << std::endl << "Breaking the print loop..";
      break;
    }
  }

  std::cout << std::endl;
}

// Returns true if there is a loop in linked list
// else returns false.
bool IsThereALoopInTheLL(const Node* node) {
  const Node* fast = node;
  const Node* slow = node;

  while (fast != nullptr && slow != nullptr) {
    slow = slow->next;
    if (fast->next != nullptr) {
      fast = fast->next->next;
    } else {
      fast = fast->next;
    }

    if (fast == slow) {
      return true;
    }
  }

  return false;
}

int main() {
  Node* root = nullptr;
  push(&root, 1); // 1 -> null
  Node* loop = root;
  push(&root, 2); // 2 -> 1 -> null
  push(&root, 3); // 3 -> 2 -> 1 -> null
  push(&root, 4); // 4 -> 3 -> 2 -> 1 -> null

  // Print all the nodes.
  printList(root);

  // Now, lets introduce a loop, the last node points to the first
  loop->next = root; // 4 -> 3 -> 2 -> 1 -> 4

  if (!IsThereALoopInTheLL(root)) {
    std::cout << "Failed to detect the loop!" << std::endl;
    printList(root);
  } else {
    std::cout << "There is a loop in the LL" << std::endl;
    printList(root, 10);
  }

  return 0;
}
