/**
 * https://www.youtube.com/watch?v=RORuwHiblPc
 */

#pragma once

#include <vector>
#include <string>

class TextEditor {
public:
  void addText(const std::string& text);
  
  void addTexts(const std::vector<std::string>& texts) {
    for (const auto& text : texts) {
      addText(text);
    }
  }

  // returns no of words.
  size_t wordCount() const {
    return mWords.size();
  }

  // Displays the text with a justification on a window of given width.
  void justifiedDisplay(size_t width) const;

  // Display every thing in a line.
  void display() const;

private:
  std::vector<std::string> mWords;
};

