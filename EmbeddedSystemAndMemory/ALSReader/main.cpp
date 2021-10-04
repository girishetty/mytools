/**
 * Security cameras often have an Ambient Light Sensor (ALS) which measures the amount of light
 * around the camera and triggers a "night mode" when the ambient light is too low.
 *
 * You've been given an ALS that reports sensor readings using the following struct:
 *
 * struct SensorReading {
 *   int status;
 *   float lux;
 *   uint64_t timestamp; // time of reading
 * };
 *
 * To get a sensor reading, call:
 *
 * SensorReading read_next_sample() { ... }
 * 
 * This function is blocking!  It doesn't return a value until either 10 seconds have elapsed or the
 * ALS returns a new value.  The SensorReading.status int indicates whether the value changed
 * (VALID) or not (NO_CHANGE). If this field is anything other than VALID, the lux and timestamp
 * fields are undefined.
 * 
 * (We've implemented a mock version of read_next_sample below, but you do not need to read the code
 * and understand it to complete this task).
 * 
 * Working with blocking functions is hard and not thread-friendly, so we want to wrap the
 * ALS function in a non-blocking, thread-safe way.
 * 
 * Note: the first call to read_next_sample is guaranteed to not block and will return a valid
 * sample so that the system can bootstrap itself.
 * 
 * 
 * Tasks:
 * 1 - Design and implement an API that allows users to read the most recent valid lux value.
 *  The API should be non-blocking and thread-safe.
 * 2 - Design and implement an API that allows users to read lux values from any time within the
 * last 10 minutes.  Your API should be non-blocking and thread-safe.
 **/

#include <stdlib.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>

//====================================================================================
//====================================================================================
//====================================================================================
/* Status enum */
enum Status {
  ERROR = 0,
  NO_CHANGE = 1,
  VALID = 2,
};

/* Result from the vendor api */
struct SensorReading {
  int status;
  float lux;
  uint64_t timestamp;
};

/* these functions are provided for you; no need to implement */
uint64_t get_timestamp();
SensorReading read_next_sample();

/* Don't edit this code */
float random_float() {
    return (float)rand() / RAND_MAX; 
}

/*! Don't edit this code */
uint64_t get_timestamp() {
  std::chrono::time_point<std::chrono::steady_clock> ts = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(ts.time_since_epoch()).count();
}

/* Don't edit this code */
SensorReading read_next_sample() {
  static bool first_run = true;
  static uint64_t max_wait = 10 * 1000000;
  SensorReading return_value;

  if (first_run) {
    first_run = false;
    return_value.lux = random_float() * 10;
    return_value.status = Status::VALID;
    return_value.timestamp = get_timestamp();
    return return_value;
  }

  if (random_float() < 0.0001) {
    return_value.status = Status::ERROR;
    return return_value;
  }

  uint64_t next_value_change = 30 * 1000000 * random_float();

  if (max_wait <= next_value_change) {
    std::this_thread::sleep_for(std::chrono::microseconds(max_wait));
    return_value.status = Status::NO_CHANGE;
    return return_value;
  }

  std::this_thread::sleep_for(std::chrono::microseconds(next_value_change));
  return_value.lux = random_float() * 10;
  return_value.status = Status::VALID;
  return_value.timestamp = get_timestamp();
  return return_value;
}

//====================================================================================
//====================================================================================
//====================================================================================

// Map of time stamp and lux
std::map<uint64_t, float> luxTable;
std::mutex lock;

// Flag to stop the thread.
bool stopThread = false;

void SampleReadingThread() {
  size_t i = 0;
  while (!stopThread) {
    auto reading = read_next_sample();
    std::cout << "sample #: " << i << ", status: " << reading.status
              << ", lux: " << reading.lux << ", ts: " << reading.timestamp
              << std::endl;
    i++;
    if (reading.status == VALID) {
      std::lock_guard<std::mutex> lock_(lock);
      luxTable.insert(std::pair<uint64_t, float> (reading.timestamp, reading.lux));
    }
  }
}

/**
 * 1 - Design and implement an API that allows users to read the most recent valid lux value.
 *  The API should be non-blocking and thread-safe.
 */
float get_most_recent_lux() {
  std::lock_guard<std::mutex> lock_(lock);
  if (!luxTable.empty()) {
    return luxTable.rbegin()->second;
  }
  return -1;
}

/**
 * 2 - Design and implement an API that allows users to read lux values from any time within the
 * last 10 minutes.  Your API should be non-blocking and thread-safe.
 */
float get_lux_at(uint64_t timestamp) {
  std::cout << "timestamp: " << timestamp << ": Size: " << luxTable.size()
            << std::endl;
  std::lock_guard<std::mutex> lock_(lock);
  if (!luxTable.empty()) {
    auto found = luxTable.lower_bound(timestamp);
    if (found != luxTable.end()) {
      return found->second;
    }
  }

  return -1.0;
}

int main() {
  std::thread reader(SampleReadingThread);
#ifdef TEST_READ_SAMPLE
  for (int i = 0; i < 10; i++) {
    auto reading = read_next_sample();
    std::cout << "sample #: " << i << ", status: " << reading.status
              << ", lux: " << reading.lux << ", ts: " << reading.timestamp
              << std::endl;
  }
#endif
  
#ifdef TEST_PART_1
  float lux = get_most_recent_lux();
  std::cout << "lux [1] = " << lux << std::endl;
  
  std::this_thread::sleep_for(std::chrono::seconds(12));
  
  lux = get_most_recent_lux();
  std::cout << "lux [2] = " << lux << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(13));

  lux = get_most_recent_lux();
  std::cout << "lux [3] = " << lux << std::endl;
  
  std::this_thread::sleep_for(std::chrono::seconds(20));

  lux = get_most_recent_lux();
  std::cout << "lux [4] = " << lux << std::endl;  
#endif
  
#ifdef TEST_PART_2
  auto start = get_timestamp();
  std::this_thread::sleep_for(std::chrono::seconds(20));
  
  for (int i = 0; i < 30; i++) {
    auto lux = get_lux_at(start + 1000000 * i);
    std::cout << "t: " << i << ", lux: " << lux << std::endl;
  }
#endif
  
  // Now we want to stop the thread
  stopThread = true;
  reader.join();
  return 0;
}

