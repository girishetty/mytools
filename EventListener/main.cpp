#include <mutex>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>

using namespace std::chrono_literals; 

// Listener interface.
class Listener {
public:
  Listener() {}
  virtual ~Listener() {}
  virtual void onEventOccured(size_t eventCount) = 0;
};

// Class that implements event driver. Or a service.
class EventDriver {
public:
  EventDriver() : mThread(&EventDriver::startEventDriver, this) {
  }

  ~EventDriver() {
    stop();
  }

  void stop() {
    if (mRunning) {
      mRunning = false;
      mEventCondition.notify_all();
      mThread.join();
    }
  }

  void addListener(std::shared_ptr<Listener> listener) {
    std::lock_guard<std::mutex> lock(mLock);
    mListener.push_back(listener);
  }

  void removeListener(std::shared_ptr<Listener> listener) {
    std::lock_guard<std::mutex> lock(mLock);
    auto found = std::find_if(mListener.begin(), mListener.end(), [&](const std::weak_ptr<Listener> item) {
      auto listener_ = item.lock();
      return listener_ == listener;
    });
    if (found != mListener.end()) {
      mListener.erase(found);
    }
  }

private:
  bool getEvent() {
    std::unique_lock<std::mutex> lock(mLock);
    // Generate an event for every 500ms or exit if the thread is asked to stop.
    auto notRunning = mEventCondition.wait_for(lock, 500ms, [&]() {
      return !mRunning; });

    if (!notRunning) {
      // Still running. Increment the event count.
      mEventCount++;
    }
    return mRunning;
  }

  void startEventDriver() {
    while (mRunning) {
      if (getEvent()) {
        notifyEvent();
      }
    }
  }

  void notifyEvent() {
    std::lock_guard<std::mutex> lock(mLock);
    for (auto it = mListener.begin(); it != mListener.end(); ) {
      auto listener = (*it).lock();
      if (listener) {
        listener->onEventOccured(mEventCount);
        it++;
      } else {
        // remove the clients that doesn't exist anymore.
        it = mListener.erase(it);
      }
    }
  }

private:
  bool        mRunning = true;
  size_t      mEventCount = 0;
  std::mutex  mLock;
  std::thread mThread;
  std::condition_variable mEventCondition;
  std::vector<std::weak_ptr<Listener>> mListener;
};

class EventImplementer {
private:
  class EventListener : public Listener {
  public:
    EventListener(EventImplementer& parent) : mParent(parent) {}

  private:
    void onEventOccured(size_t eventCount) override {
      mParent.onEventOccured(eventCount);
    }

  private:
    EventImplementer& mParent;
  };

public:
  EventImplementer(const std::string& name,
                   std::shared_ptr<EventDriver> driver) :
    mName(name),
    mDriver(driver) {
    mListener = std::make_shared<EventListener>(*this);
    mDriver->addListener(mListener);
  }

  ~EventImplementer() {
    mDriver->removeListener(mListener);
  }

private:
  friend class EventListener;

  void onEventOccured(size_t eventCount) {
    std::cout << "Name: " << mName << " Event# " << eventCount << std::endl;
  }

private:
  std::string                    mName;
  std::shared_ptr<EventDriver>   mDriver;
  std::shared_ptr<EventListener> mListener;
};


int main() {
  // Create the event driver.
  auto driver = std::make_shared<EventDriver>();

  // Set up couple of event listeners.
  auto imp1 = std::make_shared<EventImplementer>("1", driver);
  auto imp2 = std::make_shared<EventImplementer>("2", driver);

  // Let this run for 10 seconds.
  std::cout << "Sleeping 10 seconds..." << std::endl;
  std::this_thread::sleep_for(10s);

  //Now event loop should stop.
  std::cout << "Done Sleeping 10 seconds..." << std::endl;
}
