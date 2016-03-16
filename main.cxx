/*
 * main.cxx
 *
 *  Created on: Mar 16, 2016
 *      Author: hoangle
 */
#include <signal.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>

//Producer and consumer
boost::thread thread1;

//Producer and consumer
boost::thread thread2;

const int maxQueueSize = 10;

boost::lockfree::spsc_queue<int> queue1(maxQueueSize);
boost::lockfree::spsc_queue<int> queue2(maxQueueSize);

void funcThread1(void) {
  int loop = 1;
  while (1) {
    int outV;
    while (queue2.pop(outV)) {
      std::cout << "funcThread1 queue2.pop:" << outV << std::endl;
    }
    for (int i = 10 * loop; i < 100 * loop; i++) {
      while (!queue1.push(i));
      std::cout << "funcThread1 queue1.push:" << i << std::endl;
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(50));
  }

}

void funcThread2(void) {
  int loop = 1;
  while (1) 
  {
    int outV;
    while (queue1.pop(outV)) {
      std::cout << "funcThread2 queue1.pop:" << outV << std::endl;
    }
    for (int i = 10 * loop; i < 100 * loop; i++) 
    {
      while (!queue2.push(i));
      std::cout << "funcThread2 queue2.push:" << i << std::endl;
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(50));
  }
}

int main(int argc, char *argv[]) {
  thread1 = boost::thread(funcThread1);
  thread2 = boost::thread(funcThread2);

  thread2.join();
  thread1.join();
  return 0;
}
