// Implement a class that keeps track of N most visited web sites.
//
// OnNavigate method is called every time a user navigates to a URL.
//
// GetMostVisited method is called to get the result.

#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <iostream>
#include <algorithm>
#include <unordered_map>

class WebsitesTracking {
  const size_t kDefaultPageLimit = 1000;

public:
  void setPageLimit(size_t pages) {
    page_limit_ = pages;
  }

  // OnNavigate method is called every time a user navigates to a URL.
  void OnNavigate(const std::string& url) {
    if (url_maps_.size() == page_limit_) {
      //Evict Least Frequently Used item from the map.
      evictLFU();
    }

    auto found = url_maps_.find(url);
    if (found != url_maps_.end()){
      found->second++;
    } else {
      url_maps_.insert({url, 1});
    }
  }

  // GetMostVisited method is called to get the result.
  std::vector<std::string> GetMostVisited(const size_t k) {
    return GetMostVisitedPages(k);
  }

  void display() const {
    std::cout << "URL Map" << std::endl;
    for (const auto& item : url_maps_) {
      std::cout << "Visited: " << item.second << " ==> URL: " << item.first << std::endl;
    }
    std::cout << "====================================================" << std::endl;
  }

private:
  struct PageCount {
    PageCount(size_t count, const std::string& url) : count_(count), url_(url) {}
    PageCount(const PageCount& me) : count_(me.count_), url_(me.url_) {}
    PageCount& operator=(WebsitesTracking::PageCount&& me) {
      url_ = me.url_;
      count_ = me.count_;
    }

    friend bool operator> (const PageCount& lhs, const PageCount& rhs) {
      return lhs.count_ > rhs.count_;
    }

    size_t count_;
    std::string url_;
  };

  typedef std::priority_queue<PageCount, std::vector<PageCount>, std::greater<PageCount>> VisitedQueue;

  // GetMostVisitedPages that returns k most visited pages as a min priority queue
  void GetMostVisitedPages(const size_t k, VisitedQueue& queue) {
    for (auto& item : url_maps_) {
      if (queue.size() == k) {
        // Get rid of the least visited page from the queue.
        queue.pop();
      }
      queue.push(PageCount(item.second, item.first));
    }
  }

  // GetMostVisitedPages that returns k most visited pages as a vector
  std::vector<std::string> GetMostVisitedPages(const size_t k) {
    VisitedQueue queue;
    GetMostVisitedPages(k, queue);

    std::vector<std::string> result;
    while (!queue.empty()) {
      result.push_back(queue.top().url_);
      queue.pop();
    }

    // reverse it so that most visited comes first
    std::reverse(result.begin(), result.end());
    return std::move(result);
  }

  // Implement an algorithm that removes the least visited page from the Map of URLs
  void evictLFU() {
    auto size = url_maps_.size();
    if (size == page_limit_) {
      VisitedQueue queue;
      GetMostVisitedPages(size, queue);
      // Now evict the least visited page from our Map of URLs, which is on top of the queue
      url_maps_.erase(queue.top().url_);
    }
  }

private:
  std::unordered_map<std::string, size_t> url_maps_;
  size_t page_limit_ = kDefaultPageLimit;
};

int main() {
  
  std::unique_ptr<WebsitesTracking> tracker = std::make_unique<WebsitesTracking>();
  tracker->OnNavigate("www.roku.com");
  tracker->OnNavigate("www.roku.com");
  tracker->OnNavigate("www.roku.com");
  tracker->OnNavigate("www.google.com");
  tracker->OnNavigate("www.facebook.com");
  tracker->OnNavigate("www.facebook.com");
  tracker->OnNavigate("www.magicleap.com");

  tracker->display();

  auto result = tracker->GetMostVisited(3);

  for (const auto& url : result) {
    std::cout << url << std::endl;
  }

  return 0;
}
