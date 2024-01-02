/* Implement a thread pool in modern C++.
 * Explanation: https://www.cnblogs.com/sinkinben/p/16064857.html
 * Copyright: https://github.com/progschj/ThreadPool
 */

#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace impl
{
class thread_pool
{
  public:
    thread_pool(const thread_pool &) = delete;
    thread_pool(thread_pool &&) = delete;
    thread_pool &operator=(const thread_pool &) = delete;
    thread_pool &operator=(thread_pool &&) = delete;

    thread_pool(size_t nr_threads) : stop(false)
    {
        auto worker_entry = [this]() {
            while (1)
            {
                std::function<void()> task;
                // pop a task from queue 'tasks', and execute it
                {
                    std::unique_lock lock(mtx);
                    cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                    if (stop && tasks.empty())
                        return;
                    // even if stop = 1, but 'tasks' is not empty, then
                    // excucte the task until tasks queue become empty
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        };

        for (size_t i = 0; i < nr_threads; ++i)
        {
            workers.emplace_back(std::thread(worker_entry));
        }
    }
    virtual ~thread_pool()
    {
        // stop thread pool, and notify all threads to finish the remained tasks.
        {
            std::unique_lock lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (auto &worker : workers)
            worker.join();
    }

    template <class F, class... Args> std::future<std::result_of_t<F(Args...)>> enqueue(F &&f, Args &&...args)
    {
        // The return type of task `F`
        using return_type = std::result_of_t<F(Args...)>;

        // make wrapper for F(args...) to have no arguments
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock lock(mtx);

            if (stop)
                throw std::runtime_error("The thread pool has been stop.");

            // wrapper for no returned value
            tasks.emplace([task]() -> void { (*task)(); });
        }

        cv.notify_one();
        return res;
    }

  private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    /* For sync usage, protect the `tasks` queue and `stop` flag. */
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
};
} // namespace impl