/*
 https://leetcode.com/problems/decode-string/

 Given an encoded string, return its decoded string.

 The encoding rule is: k[encoded_string], where the encoded_string
 inside the square brackets is being repeated exactly k times.
 Note that k is guaranteed to be a positive integer.

 You may assume that the input string is always valid; there are no
 extra white spaces, square brackets are well-formed, etc.

 Furthermore, you may assume that the original data does not contain
 any digits and that digits are only for those repeat numbers, k.
 For example, there will not be input like 3a or 2[4].

 Example 1:
    Input: s = "3[a]2[bc]"
    Output: "aaabcbc"

 Example 2:
    Input: s = "3[a2[c]]"
    Output: "accaccacc"

 Example 3:
    Input: s = "2[abc]3[cd]ef"
    Output: "abcabccdcdcdef"
*/

#include <stack>
#include <string>
#include <iostream>

std::string decodeString(const std::string s) {
  std::string decoded;
  std::string encoded;
  int number = 0;
  std::stack<int> numberStack;
  std::stack<std::string> stringStack;
        
  for (auto item : s) {
    if (std::isdigit(item)) {
      number = number * 10 + item - '0';
    } else if (item == '[') {
      // Save the number in the stack
      numberStack.push(number);
      number = 0;
      // Save the starting part of encoded string in the stack
      if (!encoded.empty()) {
        stringStack.push(encoded);
        encoded.clear();
      }
    } else if (item == ']') {
      std::string temp;
      auto times = numberStack.top();
      numberStack.pop();
      if (encoded.empty()) {
        encoded = stringStack.top();
        stringStack.pop();
      }
      for (;times != 0; times--) {
        temp += encoded;
      }
      // Now we can either add this decoded st
      int numberCount = numberStack.size();
      int strCount = stringStack.size();
      if (numberCount == 0) {
        decoded += temp;
        encoded.clear();
      } else {
        if (strCount == numberCount) {
          encoded = stringStack.top();
          stringStack.pop();
        } else {
          encoded.clear();
        }
        encoded += temp;
      }
    } else if (numberStack.empty()) {
      decoded += item;
    } else {
      encoded += item;
    }
  }
  
  return decoded;
}

void testDecodeString(const std::string s) {
  std::cout << "Encoded String: " << s << std::endl;
  std::cout << "Decoded String: " << decodeString(s) << std::endl;
}

int main() {
  testDecodeString("3[a]2[bc]");     // aaabcbc
  testDecodeString("3[a2[c]]");      // accaccacc
  testDecodeString("2[abc]3[cd]ef"); // abcabccdcdcdef
  testDecodeString("3[z]2[2[y]pq4[2[jk]e1[f]]]ef");
}

