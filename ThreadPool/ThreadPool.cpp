#include "ThreadPool.h"
#include <memory>

std::mutex ThreadPool::_mtx;
std::unique_ptr<ThreadPool> ThreadPool::_intance;


 ThreadPool& ThreadPool::getIntance()
 {
	 std::lock_guard<std::mutex> lock(_mtx);
	 if (!_intance) {
		 _intance.reset(new ThreadPool(4));
	 }
	 return *_intance;
 }


ThreadPool::ThreadPool(int numThreads) : m_stop(false)
{
	for (int i = 0; i < numThreads; ++i)
	{
		m_threads.emplace_back([this] {
			for (;;)
			{
				std::unique_lock<std::mutex> lock(m_mtx);
				m_cv.wait(lock, [this] {
					return !m_tasks.empty() || m_stop;
				});

				if (m_stop && m_tasks.empty())
				{
					return;
				}

				std::function<void()> task(std::move(m_tasks.front()));
				m_tasks.pop();
				lock.unlock();
				task();
			}
		});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_stop = true;
	}
	m_cv.notify_all();
	for (auto& t : m_threads) {
		t.join();
	}
}
