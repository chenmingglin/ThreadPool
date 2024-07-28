#include "ThreadPool.h"
#include <memory>

 

 ThreadPool& ThreadPool::getIntance()
 {
	 // TODO: 在此处插入 return 语句
	 static ThreadPool pool(4);
	 return pool;
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
