#include <iostream>
#include "producer_consumer.h"

int main() {
  ProducerConsumer pc;
  // Run producer for 5 seconds
  pc.startProducer(5);
  // Run the consumer
  pc.startConsumer();

  // wait until they complete
  pc.wait();

  // stop
  pc.stop();
  
  return 0;
}
