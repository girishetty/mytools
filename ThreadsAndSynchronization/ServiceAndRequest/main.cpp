
#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <queue>

using Clock = std::chrono::steady_clock;

class MyService {
public:
    MyService();
    ~MyService();
    void Request(const std::string& req);
    void Pause();
    void Resume();
    void Stop();

protected:
    void threadLoop();
    void processRequest(const std::string& req);
    inline void LogTime(const char* funName, const char* msg = "");

private:
    bool                    mThreadRunning = false;
    bool                    mPausedByUs = false;
    std::mutex              mLock;
    std::mutex              mWaitConditionLock;
    std::condition_variable mWaitCondition;
    std::mutex              mPauseConditionLock;
    std::condition_variable mPauseCondition;
    std::thread             mThread;
    std::queue<std::string> mQueue;
    std::chrono::time_point<Clock> mNextWakeupTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> mLastTime;
};

inline void MyService::LogTime(const char* funName, const char* msg) {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diffFromBeginning = now - mStartTime;
    std::chrono::duration<double> diffFromLastTime = now - mLastTime;
    std::cout << "Thread: " << std::this_thread::get_id() << ": " << funName << msg <<
                 ": time: " << diffFromBeginning.count() <<
                 ": " << diffFromLastTime.count() << std::endl;
    mLastTime = now;
}

MyService::MyService() {
    mStartTime = std::chrono::high_resolution_clock::now();
    mLastTime = mStartTime;
    LogTime(__func__, ": Start");
    mQueue.push("First Request");
    mNextWakeupTime = Clock::now() + std::chrono::seconds(2);
    mThreadRunning = true;
    mThread = std::thread(&MyService::threadLoop, this);
}

MyService::~MyService() {
    Stop();
}

void MyService::Request(const std::string& req) {
    std::unique_lock<std::mutex> _l(mLock);
    mQueue.push(req);
}

void MyService::Pause() {
    LogTime(__func__);
    std::unique_lock<std::mutex> _l(mLock);
    mPausedByUs = true;
}

void MyService::Resume() {
    LogTime(__func__);
    std::unique_lock<std::mutex> _l(mLock);
    if (mPausedByUs) {
        mPauseCondition.notify_one();
        mPausedByUs = false;
    }
}

void MyService::Stop() {
    LogTime(__func__);
    std::unique_lock<std::mutex> _l(mLock);
    mThreadRunning = false;
    mPauseCondition.notify_one();
    mWaitCondition.notify_one();
    mThread.join();
}

void MyService::threadLoop() {
    std::unique_lock<std::mutex> _l(mWaitConditionLock);
    LogTime(__func__, ": Starting");
    while (mThreadRunning) {
        if (mQueue.empty() && Clock::now() < mNextWakeupTime) {
            LogTime(__func__, ": Waiting for more Requests");
            if (mWaitCondition.wait_until(_l, mNextWakeupTime) == std::cv_status::timeout) {
                LogTime(__func__, ": Done Waiting for more Requests by timeout");
            } else {
                LogTime(__func__, ": Done Waiting for more Requests by notify");
            }
        }

        if (mPausedByUs) {
            LogTime(__func__, ": Waiting to be Resumed");
            std::unique_lock<std::mutex> lock(mPauseConditionLock);
            mPauseCondition.wait(lock);
            // If we are in the process of shutting down, exit the loop already
            if (!mThreadRunning) {
                break;
            }
        }
        if (!mQueue.empty()) {
            std::unique_lock<std::mutex> _ll(mLock);
            auto req = std::move(mQueue.front());
            mQueue.pop();
            processRequest(req);
        }

    }
}

void MyService::processRequest(const std::string& req) {
    LogTime("processRequest: ", req.c_str());
}

int main() {
    MyService service;
    service.Request("One");
    service.Request("Two");
    service.Request("Three");
    service.Request("Four");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    service.Pause();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    service.Request("Five");
    service.Request("Six");

    service.Resume();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    service.Request("Seven");
    service.Request("Eight");
    service.Request("Nine");
    service.Request("Ten");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
