#include <iostream>
#include <string>
#include <map>

/**
 * The naming convention for SubRip (.srt) files is:
 * 
 * filename.[language code]_[country code].srt
 *
 * Language and country codes are case sensitive. Language needs to be in lower case and
 * country needs to be in upper case. For example, a United States English file would
 * look like: filename.en_US.srt.
 */
inline std::string getLanguageInfo(const std::string fileName) {
  std::string langCountryCode;
  auto posEnd = fileName.find_last_of('.');
  if (posEnd != std::string::npos) {
    auto posBegin = fileName.find_last_of('.', posEnd - 1);
    if (posBegin != std::string::npos) {
      langCountryCode = fileName.substr(posBegin + 1, posEnd - posBegin - 1);
      auto pos = langCountryCode.find('_');
      auto langCode = langCountryCode.substr(0, pos);
      std::cout << "Language Code: " << langCode << std::endl;
    }
  }

  return langCountryCode;
}

int main() {
  auto lang = getLanguageInfo("file:///data/native/filename.[language code]_[country code].srt");
  std::cout << "Language: " << lang << std::endl;

  lang = getLanguageInfo("file:///data/native/filename.eng_USA.srt");
  std::cout << "Language: " << lang << std::endl;

  lang = getLanguageInfo("file:///data/native/filename.de_gen.srt");
  std::cout << "Language: " << lang << std::endl;

  lang = getLanguageInfo("file:///data/native/filename.[language code].srt");
  std::cout << "Language: " << lang << std::endl;

  lang = getLanguageInfo("file:///data/native/filename.eng.srt");
  std::cout << "Language: " << lang << std::endl;

  lang = getLanguageInfo("file:///data/native/filename.de.srt");
  std::cout << "Language: " << lang << std::endl;
}
