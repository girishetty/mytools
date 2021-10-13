/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/98/design/562/
 *
 * Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.
 * Implement the MinStack class:
 *   - MinStack() initializes the stack object.
 *   - void push(int val) pushes the element val onto the stack.
 *   - void pop() removes the element on the top of the stack.
 *   - int top() gets the top element of the stack.
 *   - int getMin() retrieves the minimum element in the stack.
 */
#include <iostream>
#include <vector>

using namespace std;

const size_t kDefaultStackSize = 100;

class Stack {

public:
    Stack() : iMin(INT_MAX) {
        iList.reserve(kDefaultStackSize);
    }

    void push(int aVal) {
        if (aVal <= iMin) {
            //Push the last min element first
            iList.push_back(iMin);
            iMin = aVal;
        }

        iList.push_back(aVal);
    }

    bool pop(int* pVal) {
        bool bSuccess = false;

        if (iList.size() != 0) {
            bSuccess = true;
            *pVal = internal_pop();
            if (*pVal == iMin) {
                //Pop the next smaller element
                iMin = internal_pop();
            }
        }

        return bSuccess;
    }

    bool peek(int* pVal) const {
        bool bSuccess = false;

        if (iList.size() != 0) {
            bSuccess = true;
            *pVal = iList.back();
        }

        return bSuccess;
    }

    int min() const {
        return iMin;
    }

    //1 2 2 2 5 4 2 3 3
    void display() const {
        int index = iList.size() - 1;
        int tempMin = iMin;
        int val = 0;
        cout<<"Top < ";
        for (; index > 0; index--) {
           val = iList[index];
           cout<<val<<" ";
           if (val == tempMin) {
               index--;
               tempMin = iList[index];
           }
        }
        cout<<"> Bottom  Min: "<<iMin<<endl;
    }

private:
    int internal_pop() {
        int val = iList.back();
        iList.pop_back();
        return val;
    }

private:
    int         iMin;
    vector<int> iList;
};

int main() {
    int index = 0;
    int item = 0;
    int array[6] = {3, 2, 4, 5, 2, 1};
    Stack s;

    s.display();
    for (index = 0; index < 6; index++) {
        s.push(array[index]);
        s.display();
    }

    for (index = 0; index < 6; index++) {
        s.pop(&item);
        s.display();
    }

    return 0;
}
