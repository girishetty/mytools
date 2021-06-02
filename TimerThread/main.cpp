#include <thread>
#include <chrono>
#include <memory>
#include <iostream>
#include <condition_variable>

const size_t kFiveSecondsMs = 5000;

class DelayedTimer;

class MyClass {
public:
    MyClass(int val = 0);
    ~MyClass();

    void Reset();
    void Update(int val);
    void Display() const;

private:
    int mVal = 0;
    std::unique_ptr<DelayedTimer> mDelayedTimer;
};

class DelayedTimer {
public:
    DelayedTimer(MyClass& parent) : mParent(parent) {};

    ~DelayedTimer() {
        stopThread();
    }

    void start(size_t timeOutMs) {
        if (!mRunning) {
            starThread(timeOutMs);
        }
    }

    void stop() {
        if (mRunning) {
            stopThread();
        }
    }

protected:
    void starThread(size_t timeOutMs) {
        if (mThread.joinable()) {
            mThread.join();
        }
        mRunning = true;
        mThread = std::thread([&, timeOutMs]() {
            std::unique_lock<std::mutex> lock(mLock);
            if (mCondition.wait_for(lock, std::chrono::milliseconds(timeOutMs), [this]() {
                return mRunning == false; })) {
                std::cout << "DelayedTimer::starThread: Wait has been cancelled" << std::endl;
            } else {
                std::cout << "DelayedTimer::starThread:: Timed Out" << std::endl;
                if (mRunning) {
                    mParent.Reset();
                }
            }
            mRunning = false;
        });
    }

    void stopThread() {
        {
            std::lock_guard<std::mutex> lock(mLock);
            mRunning = false;
        }
        mCondition.notify_one();
        if (mThread.joinable()) {
            mThread.join();
        }
    }

private:
    bool                    mRunning = false;
    MyClass&                mParent;
    std::mutex              mLock;
    std::thread             mThread;
    std::condition_variable mCondition;
};

MyClass::MyClass(int val) : mVal(val) {
    mDelayedTimer.reset(new DelayedTimer(*this));
    mDelayedTimer->start(kFiveSecondsMs);
}

MyClass::~MyClass() {
    mDelayedTimer->stop();
}

void MyClass::Reset() {
    mVal = 0;
    std::cout << "MyClass::Reset::Val: " << mVal << std::endl;
}

void MyClass::Update(int val) {
    std::cout << "MyClass::Update: Old Val: " << mVal << " New Val: " << val << std::endl;
    mVal = val;
    mDelayedTimer->stop();
}

void MyClass::Display() const {
    std::cout << "MyClass::Display: Val: " << mVal << std::endl;
    mDelayedTimer->start(kFiveSecondsMs);
}

int main() {
    MyClass obj(10);
    std::this_thread::sleep_for (std::chrono::seconds(6));
    obj.Display();
    std::this_thread::sleep_for (std::chrono::seconds(4));
    obj.Update(20);
    obj.Display();
    std::this_thread::sleep_for (std::chrono::seconds(1));
    obj.Update(20);
    obj.Display();
    std::this_thread::sleep_for (std::chrono::seconds(6));
    return 0;
}
