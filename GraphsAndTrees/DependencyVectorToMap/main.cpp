/*
 * Given a dependency list for a bunch of modules (that dependence on other module(s),
 * Calculate order in which modules need to be installed and print them in that order.
 *
 * For example:  Sample Input: {(C D) (A, B), (B, C) (B, D)}
 *  which means:
 *      - module C depends on D
 *      - module A depends on B
 *      - module B depends on C
 *      - module B depends on D
 *
 * To install module A, the dependency list that needs to be installed is in the order of: D C B A
 *
 * To install module B, the dependency list that needs to be installed is in the order of: D C B
 *
 * To install module C, the dependency list that needs to be installed is in the order of: D C
 *
 * To install module D, the dependency list that needs to be installed is in the order of: D
 *
 */

#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

/**
 * A vector of pairs where the first depends on the second.
 * ( (A, B), (B, C) ) means A depends on B, B depends on C.
 */
typedef std::vector<std::pair<std::string, std::string>> dependency_vector;

/**
 * Walking through the map to print the dependency list
 *
 * @param module: The module the user wants to install.
 * @param map: A map of module dependencies
 * @param visited: A set of visited modules (so far)
 */
void walk_map(const std::string& module,
              const std::map<std::string, std::set<std::string>>& map,
              std::set<std::string>& visited) {
  // Check if we have already visited this module
  if (visited.find(module) != visited.end()) {
    return;
  }

  // Mark this module as visited
  visited.insert(module);

  // Check if its a leaf node
  auto found = map.find(module);
  if (found == map.end()) {
    // Not foumd, which means its a leaf node. So print it
    std::cout << module << " ";
    return;
  }

  // For each children under this module, traverse again
  for (const auto& item : found->second) {
    walk_map(item, map, visited);
  }

  // Now that we have visited all children for module, print it
  std::cout << module << " ";
}

/**
 * Calculate order in which modules need to be installed
 * and print them in that order.
 *
 * @param module: The module the user wants to install.
 * @param dependencies: A vector of module dependencies
 */
void process(const std::string& module, const dependency_vector& dependencies) {   
  // Create a map (like an adjucency list) of list from the dependency 
  std::map<std::string, std::set<std::string>> map;
  for (auto&& i : dependencies) {
    std::cout << i.first << " depends on " << i.second << std::endl;
    auto found = map.find(i.first);
    if (found != map.end()) {
      found->second.insert(i.second);
    } else {
      map.insert({i.first, {i.second}});
    }
  }

  // Walk through the map to print the dependencies.
  std::cout << "Dependency List is: ";
  std::set<std::string> visited;
  walk_map(module, map, visited);
  std::cout << std::endl;
}

/**
 * This function is done for you. It reads in a dependency_vector and
 * the module to install. Then it calls process(module, dependencies)
 */
int main() {
  dependency_vector dependencies;
  // Populate the dependencies vector from stdin.
  for (std::string line; std::getline(std::cin, line);) {
    std::istringstream iss(line);
    std::vector<std::string> words;
    while(iss) {
      std::string word;
      iss >> word;
      if (!word.empty())
        words.push_back(word);
    }
    if (words.size() == 2) {
      dependencies.push_back(make_pair(words[0], words[1]));
    } else if (words.size() == 1) {
      process(words[0], dependencies);
    } else if (words.size() == 0) {
      break;
    }
  }
  return 0;
}
