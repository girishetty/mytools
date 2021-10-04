
#include <algorithm>
#include <string>
#include <iterator>
#include <iostream>
#include <vector>
#include <queue>

const char* names[] = {"Girish", "ABC", "DEF", "Shetty"};
const bool  status[] = {true, false, true, false};

struct NameStatus {
    std::string mName;
    bool        mStatus;
};

struct Info {
    std::string mName;
    uint8_t*    mData = nullptr;
};

void testQueue(const std::string& name) {
  std::queue<std::string> myQueue;
  myQueue.push(name);
  auto str = myQueue.front(); // its same as ==> const std::String& str = myQueue.front();
  std::cout << str << std::endl;

  std::string str_copy = myQueue.front(); // Copies the front element to a fresh object
  myQueue.pop(); // From this point, str is a dangling reference
  if (str != name) {
    std::cout << "Reference Bad" << std::endl;
  }
  if (str_copy == name) {
    std::cout << "Copy Good" << std::endl;
  }
}

void vector_sample_2(const std::string& findMe) {
    std::vector<NameStatus> list;

    size_t index = 0;
    for (auto name : names) {
        NameStatus info;
        info.mName = name;
        info.mStatus = status[index];
        list.push_back(info);
        index++;
    }

    auto it = std::find_if(list.begin(), list.end(), [&](const NameStatus& item) {
        return item.mName == findMe;
    });

    if (it != list.end()) {
        auto index = std::distance(list.begin(), it);
        std::cout << findMe << " is found at: " << index << std::endl;
    } else {
        std::cout << findMe << " is Not found!" << std::endl;
    }
}

void vector_sample_1(const std::string& findMe) {
    std::vector<std::string> list(names, std::end(names));

    auto index = std::distance(list.begin(), std::find(list.begin(), list.end(), findMe));
    if (index >= list.size()) {
        std::cout << findMe << " is Not found! " << index << std::endl;
    } else {
        std::cout << findMe << " is found at: " << index << std::endl;
    }
}

inline ssize_t getSampleRateTableIndex(uint32_t sampleRate) {
    static const std::vector<uint32_t> kSampleRateTable { 96000, 88200, 64000, 48000, 44100, 32000,
                                                          24000, 22050, 16000, 12000, 11025, 8000 };
    auto it = std::find(kSampleRateTable.begin(), kSampleRateTable.end(), sampleRate);
    ssize_t index = -1;
    if (it != kSampleRateTable.end()) {
        index = std::distance(kSampleRateTable.begin(), it);
    }
    return index;
}

void createVector(const char* p, size_t count) {
    std::vector<char> array(p, p + count);
    std::cout << "array: " << array.size() << std::endl;
    array.push_back('S');

    for (auto it = array.begin(); it != array.end();) {
        std::cout << *it << " ";
        it = array.erase(it);
    }
    std::cout << std::endl;
    std::cout << "array: " << array.size() << std::endl;
}

void StringToFloat(const std::string& s) {
    auto found = s.find("volume ");
    if (found == 0) {
        auto s1 = s.substr(7);
        auto found = s1.find_first_of("01");
        if (found != std::string::npos) {
            s1.erase(0, found);
        }
        auto f = std::stof(s1);
        std::cout << "String: " << s << " : Float: " << f << std::endl;
    }
}

int main() {
#if 0
    vector_sample_1("Girish");
    vector_sample_1("Shetty");
    vector_sample_1("NotFound");
    vector_sample_2("Girish");
    vector_sample_2("Shetty");
    vector_sample_2("NotFound");
#endif
    std::cout << getSampleRateTableIndex(96000) << " : " << getSampleRateTableIndex(100) << " : "
              << getSampleRateTableIndex(8000) << " : " << getSampleRateTableIndex(32000) << std::endl;

    createVector("Girish", 6);
    createVector(nullptr, 0);

    StringToFloat("volume    0.8");
    testQueue("Magic Leap");
    return 0;
}

