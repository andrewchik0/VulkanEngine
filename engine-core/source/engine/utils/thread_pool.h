#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <future>
#include <type_traits>

namespace VKEngine::Utils {
  
  class ThreadPool
  {
  public:
    ThreadPool()
    : ThreadPool(std::thread::hardware_concurrency())
    {}
    
    ThreadPool(uint32_t threadCount)
      : _threadCount(threadCount)
    {
      _threads.resize(_threadCount);
      start();
    }
    
    ~ThreadPool()
    {
      terminate();
    }
    
    void terminate()
    {
      {
        std::unique_lock<std::mutex> lock(_queueMutex);
        _bTerminate = true;
      }
      _mutexCondition.notify_all();
      for (std::thread& thread : _threads)
      {
        if (thread.joinable())
          thread.join();
      }
      _threads.clear();
    }
    
    void resize(uint32_t threadCount)
    {
      terminate();
      _threadCount = threadCount;
      start();
    }
    
    template <typename F, typename R = std::invoke_result_t<F>>
    std::future<R> enqueue(const F& job)
    {
      std::shared_ptr<std::promise<R>> promise = std::make_shared<std::promise<R>>();
      {
        std::unique_lock<std::mutex> lock(_queueMutex);
        _jobs.push([promise, &job]{
          if constexpr(std::is_void_v<R>)
          {
            job();
            promise->set_value();
          }
          else
            promise->set_value(job());
        });
      }
      _mutexCondition.notify_one();
      return promise->get_future();
    }
    
    bool is_busy()
    {
      bool bPoolBusy;
      {
        std::unique_lock<std::mutex> lock(_queueMutex);
        bPoolBusy = !_jobs.empty();
      }
      return bPoolBusy;
    }
  private:
    
    bool _bTerminate = false;
    std::vector<std::thread> _threads;
    std::mutex _queueMutex;
    std::condition_variable _mutexCondition;
    uint32_t _threadCount;
    std::queue<std::function<void()>> _jobs;
    
    void start()
    {
      for (size_t i = 0; i < _threadCount; i++)
        _threads.emplace_back(std::thread(&ThreadPool::thread_loop, this));
    }
    
    void thread_loop()
    {
      while (true)
      {
        std::function<void()> job;
        
        // Create scope with locking to access queue
        {
          // Lock the queue
          std::unique_lock<std::mutex> lock(_queueMutex);
          
          // Wait until some work will appear
          _mutexCondition.wait(lock, [this] {
            return !_jobs.empty() || _bTerminate;
          });
          
          if (_bTerminate)
            return;
          
          job = _jobs.front();
          _jobs.pop();
        }
        
        job();
      }
    }
  };
}
