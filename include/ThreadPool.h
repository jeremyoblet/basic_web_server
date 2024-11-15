#pragma once

#include <thread>
#include <vector>
#include <memory>
#include <atomic>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <stdexcept>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads, size_t max_queue_size = 10000)
        : max_queue_size(max_queue_size), is_running(true)
    {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return !is_running || !tasks.empty(); });
                        if (!is_running && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<typename F>
    void submit(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (tasks.size() >= max_queue_size) {
                throw std::runtime_error("Task queue is full");
            }
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            is_running = false;
        }
        condition.notify_all();
        for (auto& thread : threads) {
            thread.join();
        }
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    size_t max_queue_size;
    std::atomic<bool> is_running;
};