/*
 * DESCRIPTION:
 *
 * There is a system with a buffer of a fixed size and
 * two workers (producer and consumer) operating with this buffer.
 * The producer generates a random number and put it in the buffer, if it's NOT full.
 * The consumer reads produced numbers when the buffer is NOT empty.
 * 
 * REQUIRED:
 *  - The producer and the consumer MUST run in parallel.
 *  - Run the producer for a duration (in seconds) and calculate a sum of all produced numbers.
 *  - Consume all numbers and also calculate a sum of all consumed numbers.
 *  - Provide interface to stop producer and consumer
 *  - Provide interface to wait on the producer and consumer
 */

#include <chrono>
#include <iostream>
#include "producer_consumer.h"

bool ProducerConsumer::startProducer(size_t duration) {
  mProducerRunning = true;
  mProducer = std::thread([&, duration] { ProducerThread(duration); });
}

bool ProducerConsumer::startConsumer() {
  mConsumerRunning = true;
  mConsumer = std::thread([&] { ConsumerThread(); });
}

void ProducerConsumer::wait() {
  if (mProducer.joinable()) {
    std::cout << "Waiting for the Producer to complete..." << std::endl;
    mProducer.join();
  }
  if (mConsumer.joinable()) {
    std::cout << "Waiting for the Consumer to complete..." << std::endl;
    mConsumer.join();
  }
}

void ProducerConsumer::stop() {
  stopProducer();
  stopConsumer();
  wait();
}

bool ProducerConsumer::stopProducer() {
  std::lock_guard<std::mutex> lock(mLock);
  std::cout << "User is stopping the Producer" << std::endl;
  mProducerRunning = false;
  mFullCond.notify_one();
}

bool ProducerConsumer::stopConsumer() {
  std::lock_guard<std::mutex> lock(mLock);
  std::cout << "User is stopping the Consumer" << std::endl;
  mConsumerRunning = false;
  mEmptyCond.notify_one();
}

void ProducerConsumer::ProducerThread(size_t duration) {
  using namespace std::chrono_literals;
  size_t counter = 0;
  long long sum = 0;

  if (duration == 0) {
    // Run until stopped
    duration = std::numeric_limits<int>::max();
  }
  auto startTime = std::chrono::system_clock::now();
  auto runUntil = startTime + duration * 1000ms;
  srand(time(0));

  while (mProducerRunning) {
    std::unique_lock<std::mutex> lock(mLock);
    // Wait until one of these conditions fails:
    // 1. The producer thread has already run for "duration" seconds ==> timeout
    // 2. The consumer consumes some items
    // 3. The producer thread is asked to stop.
    auto success = mFullCond.wait_until(lock, runUntil, [&]() {
      return (mProducedList.size() < BUFFER_SIZE || !mProducerRunning);
    });

    if (mProducedList.size() < BUFFER_SIZE) {
      int num = std::rand();
      mProducedList.push_back(num);
      counter++;
      sum += num;
    } else if (mProducerRunning) {
      mProducerRunning = false;
      std::cout << "Producer has run for " << duration << " seconds" << std::endl;
    }
    // Notify the consumer
    mEmptyCond.notify_one();
  }

  {
    std::unique_lock<std::mutex> lock(mLock);
    std::cout << "Producer has produced " << counter << " no of Items with Sum of: " << sum << std::endl;
  }
}

void ProducerConsumer::ConsumerThread() {
  size_t counter = 0;
  long long sum = 0;

  while (mConsumerRunning) {
    std::unique_lock<std::mutex> lock(mLock);
    // Wait until either producer produces some items or the thread is asked to stop.
    // Wait until one of these conditions fails:
    // 1. The producer produces some items
    // 2. The consumer thread is asked to stop.
    // 3. We have consumer all the produced items and the producer thread has already ended
    mEmptyCond.wait(lock, [&]() {
      return (!mProducedList.empty() || !mConsumerRunning || !mProducerRunning);
    });

    if (!mProducedList.empty()) {
      sum += mProducedList.front();
      mProducedList.pop_front();
      mFullCond.notify_one();
      counter++;
    } else if (!mProducerRunning) {
      mConsumerRunning = false;
      std::cout << "No more items and the Producer has stopped" << std::endl;
    }
  }

  {
    std::unique_lock<std::mutex> lock(mLock);
    std::cout << "Consumer has consumed " << counter << " no of Items with Sum of: " << sum << std::endl;
  }
}
