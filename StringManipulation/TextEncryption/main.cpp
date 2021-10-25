#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include <iostream>
#include <cmath>

using std::string;
using std::cout;
using std::endl;
using std::cin;

/**
 * An English text needs to be encrypted using the following encryption scheme. 
 * First, the spaces are removed from the text. Let be the length of this text. 
 * Then, characters are written into a grid, whose rows and columns have the following constraints:
 * 
 * floor(sqrt(L)) <= row <= colum <= ceil(sqrt(L))
 *
 * For example, the sentence,
 * s = if man was meant to stay on the ground god would have given us roots
 *
 * after removing spaces is 54 characters long. Sqrt(54) is between 7 and 8,
 * so it is written in the form of a grid with 7 rows and 8 columns.
 *
 * ifmanwas  
 * meanttos          
 * tayonthe  
 * groundgo  
 * dwouldha  
 * vegivenu  
 * sroots
 *
 * - Ensure that rows * colums >= L
 * - If multiple grids satisfy the above conditions, choose the one with the minimum area, i.e. rows * colums.
 *
 * The encoded message is obtained by displaying the characters in a column, inserting a space,
 * and then displaying the next column and inserting a space, and so on.
 * For example, the encoded message for the above rectangle is:
 *
 * imtgdvs fearwer mayoogo anouuio ntnnlvt wttddes aohghn sseoau
 *
 * You will be given a message to encode and print.
 *
 * Function Description:
 *
 * Complete the encryption function in the editor below. It should return a single string composed as described.
 * encryption has the following parameter(s):
 * s: a string to encrypt
 *
 * Input Format:
 * One line of text, the string 
 *
 * Constraints:
 * 1 <= |s| <= 81
 * s is comprised only of characters in the range ascii[a-z].
 *
 * Output Format:
 * Print the encoded message on one line as described.
 *
 *
 * Sample Input#0:
 * haveaniceday
 *
 * Sample Output#0:
 * hae and via ecy
 *
 * Explanation#0:
 * L is 12, Sqrt(12) is between 3 and 4. 
 * Rewritten with 3 rows and 4 columns:
 * have
 * anic
 * eday
 *
 * Sample Input#1:
 * feedthedog    
 *
 * Sample Output#1:
 * fto ehg ee dd
 *
 * Explanation#1:
 * L is 10, Sqrt(10) is between 3 and 4. 
 * Rewritten with 3 rows and 4 columns:
 * feed
 * thed
 * og
 *
 * Sample Input#2:
 * chillout
 *
 * Sample Output#2:
 * clu hlt io
 *
 * Explanation#2:
 * L is 8, Sqrt(8) is between 2 and 3. 
 * Rewritten with 3 rows and 3 columns (2 * 3 = 6 < 8, so we have to use 3 * 3)
 * chi
 * llo
 * ut
 */

// Complete the encryption function below.
string encryption(string s) {
  string encodedString;
  // get rid of all spaces.
  s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
  size_t L = s.size();

  auto sqrt_L = std::sqrt(L);
  size_t row = std::floor(sqrt_L);
  size_t column = std::ceil(sqrt_L);

  if ((row * column) < L) {
    row++;
  }

  for (size_t colIndex = 0; colIndex < column; colIndex++) {
    for (size_t rowIndex = 0; rowIndex < row; rowIndex++) {
      auto index = rowIndex * column + colIndex;
      if (index >= L) {
        break;
      }
      encodedString += s[index];
    }
    encodedString += ' ';
  }
  return encodedString;
}

int main() {
  string s;
  getline(cin, s);

  string result = encryption(s);

  cout << "Given String: " << s << endl;
  cout << "Encrypted String: " << result << endl;

  return 0;
}
