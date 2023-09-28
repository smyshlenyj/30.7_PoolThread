#include "ThreadPool.h"

void ThreadPool::start()
{
    for (int i = 0; i < m_thread_count; i++)
        m_threads.emplace_back(&ThreadPool::threadFunc, this, i);
}

void ThreadPool::stop()
{
    for (int i = 0; i < m_thread_count; i++)
    {
        task_type empty_task;
        m_thread_queues[i].push(empty_task);
    }
    for (auto &t : m_threads)
        t.join();
}

void ThreadPool::push_task(FuncType f, std::shared_ptr<int[]> array, long arg1, long arg2)
{
    int queue_to_push = m_index++ % m_thread_count;
    task_type task([=]
                   { f(array, arg1, arg2); });
    m_thread_queues[queue_to_push].push(task);
}

ThreadPool::ThreadPool() : m_thread_count(std::thread::hardware_concurrency() != 0? std::thread::hardware_concurrency():4),
                           m_thread_queues(m_thread_count) {}

void ThreadPool::threadFunc(int qindex)
{
    while (true)
    {
        task_type task_to_do;
        bool res;
        int i = 0;
        for (; i < m_thread_count; i++)
            if (res = m_thread_queues[(qindex + i) % m_thread_count].fast_pop(task_to_do))
                break;

        if (!res)
            m_thread_queues[qindex].pop(task_to_do);

        else if (!task_to_do)
            m_thread_queues[(qindex + i) % m_thread_count].push(task_to_do);

        if (!task_to_do)
            return;

        task_to_do();
    }
}