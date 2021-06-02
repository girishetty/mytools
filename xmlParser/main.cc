#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>
#include <libxml/xmlreader.h>
#include "mpd_parser.h"
#include "normalizer.h"

using std::cout;

// Use this if you are only interested in reading and printing each node/element
#define MPD_READER 0x01
// Use this if you want to normalize the mpd file
#define NORMALIZER 0x02
// Use this if you want to parse each and every node/element pertaining to MPD (DASH)
#define MPD_PARSER 0x04

static void mpdNodeReader(const std::string& xml, const std::string& url) {
  std::unique_ptr<xmlTextReader, void (*)(xmlTextReaderPtr)> reader(
      xmlReaderForMemory(xml.data(), xml.length(), url.c_str(), nullptr, 0),
      xmlFreeTextReader);

  int ret = xmlTextReaderRead(reader.get());
  const std::string text("text");
  while (ret == 1) {
    xmlNodePtr currentNode = xmlTextReaderCurrentNode(reader.get());
    if (text.compare((const char*)currentNode->name)) {
      std::cout << "Node name: " << currentNode->name << std::endl;
    }
    ret = xmlTextReaderRead(reader.get());
  }
}

static void readMPD(const char* mpdFileName, size_t options) {
  std::ifstream file(mpdFileName, std::ios::in|std::ios::ate);
  if (file.is_open()) {
    size_t size = file.tellg();
    char* content = new char[size+1];
    file.seekg (0, std::ios::beg);
    file.read (content, size);
    file.close();
    if (options & NORMALIZER) {
      NormalizeXml normalize(content);
      normalize.start("normalized.mpd");
    }
    if (options & MPD_READER) {
      mpdNodeReader(content, "no_url");
    }
    if (options & MPD_PARSER) {
      MediaPresentationDescriptionParser mpd("girish");
      mpd.Parse(content, "no_url");
    }
    delete [] content;
  } else {
    cout << "File : " << mpdFileName << " Can't Open!" << std::endl;
  }
}

int main(int argc, char *argv[]) {
  const char* mpdFileName = nullptr;
  size_t options = 0;

  for (size_t index = 1; index < argc; index++) {
    if (strncmp(argv[index], "-p", 2) == 0) {
      options |= MPD_PARSER;
    } else if (strncmp(argv[index], "-r", 2) == 0) {
      options |= MPD_READER;
    } else if (strncmp(argv[index], "-n", 2) == 0) {
      options |= NORMALIZER;
    } else {
      mpdFileName = argv[index];
    }
  }

  if (options == 0) {
    options = MPD_PARSER;
  }
  if (mpdFileName != nullptr) {
    readMPD(mpdFileName, options);
  }
  
  return 0;
}
