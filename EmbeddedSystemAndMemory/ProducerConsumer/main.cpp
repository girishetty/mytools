#include <thread>
#include <mutex>
#include <list>
#include <cstdlib>
#include <iostream>
#include <condition_variable>

/*
 * DESCRIPTION:
 *
 * There is a system with a buffer of a fixed size and
 * two workers (producer and consumer) operating with this buffer.
 * The producer generates a random number and put it in the buffer, if it's NOT full.
 * The consumer reads produced numbers when the buffer is NOT empty.
 * 
 * REQUIRED:
 *
 *  - The producer and the consumer MUST run in parallel.
 *  - Run the producer for 1 second and calculate a sum of all produced numbers.
 *  - Consume all numbers and also calculate a sum of all consumed numbers.
 */

constexpr auto BUFFER_SIZE = 10;

std::mutex gLock;
std::list<int> gProducedList;
std::condition_variable gFullCond;
std::condition_variable gEmptyCond;

void ProducerThread() {
  size_t counter = 0;
  long long sum = 0;

  while (true) {
    std::unique_lock<std::mutex> lock(gLock);
    if (gProducedList.size() < BUFFER_SIZE) {
      int num = std::rand();
      gProducedList.push_back(num);
      sum += num;
      gEmptyCond.notify_one();
      counter++;
    } else {
      gFullCond.wait(lock, []() { return gProducedList.size() >= BUFFER_SIZE; });
    }
    if (counter == 1000) {
      std::cout << "Produce Exiting with sum: " << sum << std::endl;
      break;
    }
  }
}

void ConsumerThread() {
  size_t counter = 0;
  long long sum = 0;
  while (true) {
    std::unique_lock<std::mutex> lock(gLock);
    if (gProducedList.empty()) {
      gEmptyCond.wait(lock, []() { return !gProducedList.empty(); });
    }
    sum += gProducedList.front();
    gProducedList.pop_front();
    gFullCond.notify_one();
    counter++;

    if (counter == 1000) {
      std::cout << "Consumer Exiting with sum:" << sum << std::endl;
      break;
    }
  
  }
}

int main() {
  auto producer = std::thread(ProducerThread);
  auto consumer = std::thread(ConsumerThread);
  producer.join();
  consumer.join();
  
  return 0;
}

