#pragma once
#include <queue>
#include <future>
#include <condition_variable>
#include <vector>
#include <thread>
#include <mutex>
#include "BlockedQueue.h"

typedef std::function<void()> task_type;
typedef void (*FuncType) (std::shared_ptr<int[]>, long, long);

class ThreadPool {
   public:
       void start();
       void stop();
       void push_task(FuncType f, std::shared_ptr<int[]> a, long id, long arg);
       void threadFunc(int qindex);

       ThreadPool();
   private:
       int m_thread_count;
       std::vector<std::thread> m_threads;
       std::vector<BlockedQueue<task_type>> m_thread_queues;
       int m_index;
};

