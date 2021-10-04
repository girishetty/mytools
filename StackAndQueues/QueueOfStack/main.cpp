/*
 *
 *
 * Implement a first in first out (FIFO) queue using only two stacks. The implemented queue should support all the functions of a normal queue (push, peek, pop, and empty).
 *
 * Implement the MyQueue class:
 *   - void push(int x) Pushes element x to the back of the queue.
 *   - int pop() Removes the element from the front of the queue and returns it.
 *   - int peek() Returns the element at the front of the queue.
 *   - boolean empty() Returns true if the queue is empty, false otherwise.
 *
 * Notes:
 * You must use only standard operations of a stack, which means only push to top, peek/pop from top, size, and is empty operations are valid.
 * Depending on your language, the stack may not be supported natively.
 * You may simulate a stack using a list or deque (double-ended queue) as long as you use only a stack's standard operations.
 */

#include <stack>
#include <iostream>

class MyQueue {
  std::stack<int> pushStack;
  std::stack<int> popStack;

  void fillPopStack() {
    while (!pushStack.empty()) {
      popStack.push(pushStack.top());
      pushStack.pop();
    }
  }

public:
  /** Initialize your data structure here. */
  MyQueue() { }

  /** Push element x to the back of queue. */
  void push(int x) {
    pushStack.push(x);
  }

  /** Removes the element from in front of queue and returns that element. */
  int pop() {
    auto item = peek();
    popStack.pop();
    return item;
  }

  /** Get the front element. */
  int peek() {
    if (popStack.empty()) {
      //Move all elements from pushStack to popStack
      fillPopStack();
    }
    return popStack.top();
  }

  /** Returns whether the queue is empty. */
  bool empty() {
    return pushStack.empty() && popStack.empty();
  }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */

int main() {
  return 0;
}
