#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <random>
#include <algorithm>
#include <cassert>

#ifdef CPP_17

// gcc -lstdc++fs -std=c++17 cleanup_disk.cpp

#include <filesystem>

namespace fs = std::filesystem;

void getFileList(const std::string& path, std::vector<std::string>& list) {
  list.clear();
  for (const auto& entry : fs::directory_iterator(path)) {
    list.push_back(entry.path());
  }
}

#else

bool isDirectory(const char* path) {
  struct stat s;
  if (stat(path, &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      return true;
    }
  }
  return false;
}

void getFileListRecursively(const std::string& path, std::vector<std::string>& list) {
  DIR* dir = opendir(path.c_str());
  if (dir != nullptr) {
    struct dirent* ent = nullptr;
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != nullptr) {
      if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
        continue;
      }
      std::string fullPath = path + "/" + ent->d_name;
      if (isDirectory(fullPath.c_str())) {
        getFileListRecursively(fullPath, list);
      } else {
        list.push_back(fullPath);
      }
    }
    closedir(dir);
  } else {
    std::cout << "could not open directory: " << path << std::endl;
  }
}

void getFileList(const std::string& path, std::vector<std::string>& list) {
  list.clear();
  getFileListRecursively(path, list);
}
#endif

void clearFileContents(const std::string& path, const std::vector<uint64_t>& data) {
  FILE* file = fopen(path.c_str(), "r+");
  if (file == nullptr) {
    std::cout << "Failed to open: " << path << std::endl;
    return;
  }

  fseek(file, 0L, SEEK_END);
  auto fileSize = ftell(file);
  fseek(file, 0L, SEEK_SET);

  std::cout << "Clearing: " << path << " with size: " << fileSize << std::endl;
  long offset = 0;
  auto copySize = data.size();
  while (offset < fileSize) {
    auto remaining = fileSize - offset;
    size_t toCopy = (remaining < copySize) ? remaining : copySize;
    fwrite(data.data(), 1, toCopy, file);
    offset += toCopy;
  }
  
  fclose(file);
}

void generateData(std::vector<uint64_t>& data) {
  std::iota(data.begin(), data.end(), 0);
  std::shuffle(data.begin(), data.end(), std::mt19937{ std::random_device{}() });
}

int main() {
  const size_t MB = 1024 * 1024;
  std::vector<uint64_t> data(MB / sizeof(uint64_t));
  generateData(data);

  std::string path = "/Users/gshetty/work/new_codebase";
  std::cout << "Enther the path which you want to clear up: ";
  std::cin >> path;
  std::vector<std::string> list;
  getFileList(path, list);

  for (const auto item : list) {
    clearFileContents(item, data);
  }
  return 0;
}
