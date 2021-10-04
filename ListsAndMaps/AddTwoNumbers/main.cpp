/*
 * https://leetcode.com/problems/add-two-numbers/
 *
 * You are given two non-empty linked lists representing two non-negative integers.
 * The digits are stored in reverse order, and each of their nodes contains a single digit.
 * Add the two numbers and return the sum as a linked list.
 * 
 * You may assume the two numbers do not contain any leading zero, except the number 0 itself.
 *
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
    inline ListNode* sumUpAndCreateNode(ListNode* node, int val, int& carry) {
        int sum = val + carry;
        if (sum > 9) {
            carry = 1;
            sum = sum - 10;
        } else {
            carry = 0;
        }
        node->next = new ListNode(sum);
        return node->next;
    }
    
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* res = new ListNode(0);
        ListNode* resHead = res;
        
        int carry = 0;
        for (; l1 != nullptr && l2 != nullptr; l1 = l1->next, l2 = l2->next) {
            res = sumUpAndCreateNode(res, l1->val + l2->val, carry);
        }
        
        ListNode* rem = (l1 != nullptr) ? l1 : l2;
        for (; rem != nullptr;  rem = rem->next) {
            res = sumUpAndCreateNode(res, rem->val, carry);
        }
        
        if (carry > 0) {
            res->next = new ListNode(carry);
        }
        
        if (resHead->next != nullptr) {
            res = resHead->next;
            delete resHead;
        } else {
            res = resHead;
        }
        return res;
    }
};
