#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>

template<class T>
class BlockedQueue {
   public:
   void push(T& item_);
   
   // блокирующий метод получения элемента из очереди
   void pop(T& item_) ;

   // неблокирующий метод получения элемента из очереди
   // возвращает false, если очередь пуста
   bool fast_pop(T& item_);

   private:
       std::mutex m_locker;
       // очередь задач
       std::queue<T> m_task_queue;
       // уведомитель
       std::condition_variable m_notifier;
};

    template<class T> void BlockedQueue<T>::push(T& item_){
       std::lock_guard<std::mutex> l(m_locker);
       // обычный потокобезопасный push
       m_task_queue.push(item_);
       // делаем оповещение, чтобы поток, вызвавший
       // pop проснулся и забрал элемент из очереди
       m_notifier.notify_one();
   }

   // блокирующий метод получения элемента из очереди
   template<class T> void BlockedQueue<T>::pop(T& item_) {
       std::unique_lock<std::mutex> l(m_locker);
       if (m_task_queue.empty())
           // ждем, пока вызовут push
           m_notifier.wait(l, [this]{return !m_task_queue.empty();});
       item_ = m_task_queue.front();
       m_task_queue.pop();
   }

   // неблокирующий метод получения элемента из очереди
   // возвращает false, если очередь пуста
   template<class T> bool BlockedQueue<T>::fast_pop(T& item_) {
       std::lock_guard<std::mutex> l(m_locker);
       if(m_task_queue.empty())
           // просто выходим
           return false;
       // забираем элемент
       item_ = m_task_queue.front();
       m_task_queue.pop();
       return true;
   }