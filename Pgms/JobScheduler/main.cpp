#include <map>
#include <mutex>
#include <chrono>
#include <thread>
#include <algorithm>
#include <functional>
#include <condition_variable>

class JobManager {
public:

  explicit JobManager() {
    mRunning = true;
    mWorkerThread = std::thread([&]() {
      Worker();
    });
  }

  ~JobManager() {
    Stop();
  }

  void QueueJob(std::chrono::steady_clock::time_point time_to_run,
                std::function<void(void)> job) {
    {
      std::lock_guard<std::mutex> lock(lock_);
      jobList_.insert(std::pair<std::chrono::steady_clock::time_point,
                                std::function<void(void)> > (time_to_run, job));
    }
    jobListWaker_.notify_one();
  }

  void Stop() {
    std::lock_guard<std::mutex> lock(lock_);
    if (mRunning) {
      mRunning = false;
      jobListWaker_.notify_all();
      mWorkerThread.join();
    }
  }

private:

  void Worker() {
    while (mRunning) {

      // Wait for the job queue to have at least one job.
      waitForJobs();
      if (mRunning) {
        // Get the job for current time.
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto job = getJobAtThisTime(now);
  
        if (mRunning && job != nullptr) {
          // Execute the job.
          job();
        }
      }
    }
  }

  // This function waits as long as the job list is empty.
  inline void waitForJobs() {
    std::unique_lock<std::mutex> lock(lock_);
    if (jobList_.empty()) {
      jobListWaker_.wait(lock);
    }
  }

  // This function waits as long as the job list is empty.
  std::function<void(void)> getJobAtThisTime(const std::chrono::steady_clock::time_point& time) {
    std::lock_guard<std::mutex> lock(lock_);
    std::function<void(void)> job = nullptr;

    //auto found = std::lower_bound(jobList_.begin(), jobList_.end(), time);
    auto found = jobList_.lower_bound(time);
    if (found != jobList_.end()){
      job = found->second;
      jobList_.erase(found);
    }

    return job;
  }

private:
  bool mRunning = false;
  std::mutex lock_;
  std::thread mWorkerThread;
  std::condition_variable jobListWaker_;
  std::map<const std::chrono::steady_clock::time_point, std::function<void(void)> > jobList_;
};

int main() {
}
