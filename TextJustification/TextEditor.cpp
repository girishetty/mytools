/**
 * https://www.youtube.com/watch?v=RORuwHiblPc
 */

#include <iomanip>
#include <sstream>
#include <iostream>
#include "TextEditor.h"

using std::cout;
using std::endl;
using std::setw;
using std::vector;
using std::string;
using std::istringstream;
using std::istream_iterator;

//#define _DEBUG

// Local utility for displaying array of words.
static void display(const vector<string>& words) {
  cout << "[ ";
  for (const auto& word : words) {
    cout << word << " ";
  }
  cout << "]";
}

// Local utility for splitting a sentence in words.
static vector<string> split(const string& text, char delimiter) {
  istringstream tokenStream(text);
#ifdef USE_DELIMITER
  string word;
  vector<string> words;

  while (getline(tokenStream, word, delimiter)) {
    words.push_back(word);
  }
#else
  vector<string> words(istream_iterator<string>{tokenStream},
                       istream_iterator<string>());
#endif
  return words;
}

void TextEditor::addText(const std::string& text) {
  auto words = split(text, ' ');
  if (!words.empty()) {
    mWords.insert(mWords.end(), words.begin(), words.end());
  }
}

void TextEditor::display() const {
  cout << "TextEditor: ";
  ::display(mWords);
  cout << endl;
}

const size_t INF = (size_t)-1;

size_t getWordsLength(const vector<string>& mWords, size_t wordBegin, size_t wordEnd) {
  size_t wordsLength = 0;
  if (wordBegin > wordEnd) {
    return INF;
  }
  // incurr for the spaces between the words.
  wordsLength += (wordEnd - wordBegin);
  for (; wordBegin <= wordEnd; wordBegin++) {
    wordsLength += mWords[wordBegin].size();
  }

  return wordsLength;
}

void TextEditor::justifiedDisplay(size_t width) const {
  size_t wordCount = mWords.size();
  // construct a table of wordCount * wordCount with accumulated length.
  vector<vector<size_t>> dp(wordCount, vector<size_t>(wordCount, INF));

  for (size_t wordBegin = 0; wordBegin < wordCount; wordBegin++) {
    for (size_t wordEnd = wordBegin; wordEnd < wordCount; wordEnd++) {
      // Length of all words from the range [wordBegin..wordEnd]
      size_t wordsLength = getWordsLength(mWords, wordBegin, wordEnd);
#ifdef _DEBUG
      cout << "wordBegin: " << wordBegin << " wordEnd: " << wordEnd << " wordsLength: " << wordsLength << endl;
#endif
      if (wordsLength > width) {
        // We can't fit this into a line of width size.
        break;
      }
      int difference = width - wordsLength;
      // Cost of writting words from range [wordBegin..wordEnd] in a line
      dp[wordBegin][wordEnd] = difference * difference;
    }
  }

#ifdef _DEBUG
  for (auto items : dp) {
    for (auto item : items) {
      cout << setw(22) << item;
    }
    cout << endl;
  }
#endif

  // Now lets build the minimum cost of each line based on the cost matrix (dp) we have constructed above.
  vector<size_t> mimCost(wordCount, 0);
  // And also a list that indicates the line breaks.
  vector<size_t> lineBreaks(wordCount, 0);
  size_t wordUntil = wordCount;
  size_t endIndex = wordCount - 1;
  for (int beginIndex = wordCount - 1; beginIndex >= 0; beginIndex--) {
    if (dp[beginIndex][endIndex] == INF) {
      // since the words from the range [beginIndex..endIndex] can't be there in one line,
      // we need to split them into multiple lines:
      // Start splitting the line from next word (until last word) and compute the
      // cost of such splitting: cost of this line + cost of already broken lines.
      // minCost[beginIndex] = min {dp[beginIndex][j] + minCost[beginIndex + 1],
      //                       where j in range [beginIndex..wordCount]
      size_t minC = INF;
      for (auto splitAfter = beginIndex; splitAfter < wordCount; splitAfter++) {
        if (dp[beginIndex][splitAfter] >= minC) {
          continue;
        }
        auto cost = dp[beginIndex][splitAfter] + mimCost[splitAfter + 1];
        if (cost < minC) {
          minC = cost;
          wordUntil = splitAfter + 1;
        }
      }
      // Cost of words from beginIndex to endIndex is when we break the line at wordUntil.
      mimCost[beginIndex] = minC;
      lineBreaks[beginIndex] = wordUntil;
    } else {
      // Cost of words from beginIndex to endIndex
      mimCost[beginIndex] = dp[beginIndex][endIndex];
      lineBreaks[beginIndex] = wordUntil;
    }
  }

#ifdef _DEBUG
  for (size_t beginIndex = 0; beginIndex < wordCount; beginIndex++) {
    cout << mimCost[beginIndex] << ":" << lineBreaks[beginIndex] << endl;
  }
#endif

  // Now we can dispay in the justified way with given width with a cost of mimCost[0]
  cout << "Contents of TextEditor in a line of width [" << width
       << "] in a Justified cost: " << mimCost[0] << endl;
  size_t startFrom = 0;
  size_t nextLineAtWord = 0;
  for (size_t startFrom = 0, nextLineAtWord = 0; nextLineAtWord < wordCount;) {
    nextLineAtWord = lineBreaks[startFrom];
    for (size_t index = startFrom; index < nextLineAtWord; index++) {
      cout << mWords[index] << " ";
    }
    cout << endl;
    startFrom = nextLineAtWord;
  }
}
