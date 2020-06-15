/**
 * https://www.youtube.com/watch?v=RORuwHiblPc
 */

#include <iostream>
#include "TextEditor.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

int main(int argc, char* argv[]) {
  TextEditor editor;
  size_t lineWidth = 40;

  if (argc > 1) {
    cout << "Enter sentences. Enter a blank line for end of input" << endl;
    while (true) {
      string a;
      getline(cin, a);

      if (a.empty()) {
        break;
      }
      editor.addText(a);
    }
    cout << "Enter line width: ";
    cin >> lineWidth;
  } else {
    // default inputs
    editor.addText("Girish Shetty loves his family.");
    editor.addText("His wife's name is Soumya Shetty.");
    editor.addText("His daughter's name is Ira G Shetty.");
    editor.addText("His mother's name is Sumathi V Shetty.");
    editor.addText("His sister's name is Asha Shetty.");
    editor.addText("His brothes names are Harish Shetty and Prathish Shetty.");
  }

  cout << "No of words: " << editor.wordCount() << endl;
  editor.display();
  editor.justifiedDisplay(lineWidth);
  return 0;
}
