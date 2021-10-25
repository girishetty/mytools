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

#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <cstdlib>
#include <condition_variable>

constexpr auto BUFFER_SIZE = 10;

class ProducerConsumer {
public:
  // Create Producer Consumer with given buffer size.
  // At any given time max of "size" number of items will be there.
  ProducerConsumer(size_t size = BUFFER_SIZE) : mBufferSize(size) {}

  // Start and run the producer for "duration" seconds.
  // Specifying 0 or no argument will make it run until its stopped.
  bool startProducer(size_t duration = 0);
  // Start the consumer.
  bool startConsumer();

  // Wait for the producer and consumer to end.
  void wait();
  // Stop the producer and the consumer.
  void stop();

  // Stop the producer.
  bool stopProducer();
  // Stop the consumer.
  bool stopConsumer();

private:
  void ProducerThread(size_t duration);
  void ConsumerThread();

private:
  bool                    mProducerRunning = false;
  bool                    mConsumerRunning = false;
  size_t                  mBufferSize = BUFFER_SIZE;
  std::mutex              mLock;
  std::thread             mProducer;
  std::thread             mConsumer;
  std::list<int>          mProducedList;
  std::condition_variable mFullCond;
  std::condition_variable mEmptyCond;
};
