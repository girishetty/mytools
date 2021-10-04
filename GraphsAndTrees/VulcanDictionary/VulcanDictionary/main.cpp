#include <iostream>
#include <fstream>
#include "DictionaryReader.h"

using namespace std;

int main() {
    const char* dictionaryFile = "dictionary_sample.txt";

    DictionaryReader reader(dictionaryFile);
    reader.DisplayVulcanAlphabets();

    return 0;
}

