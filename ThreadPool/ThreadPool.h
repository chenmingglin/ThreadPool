#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>
#include <queue>
#include <functional>


class ThreadPool
{
public:
	static ThreadPool& getIntance();
	
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	template<class F, class... Args>
	void enqueue(F&& f, Args... args);

	~ThreadPool();
protected:
	ThreadPool(int numThreads);
	

private:
	static std::mutex _mtx;
	static std::unique_ptr<ThreadPool> _intance;

	std::vector<std::thread> m_threads;
	std::queue<std::function<void()>> m_tasks;
	std::mutex m_mtx;
	std::condition_variable m_cv;
	bool m_stop;
};

template<class F, class ...Args>
inline void ThreadPool::enqueue(F&& f, Args ...args)
{
	std::function<void()> task = 
		std::bind(std::forward<F>(f), std::forward<Args>(args)...);

	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_tasks.emplace(std::move(task));
	}

	m_cv.notify_one();
}

