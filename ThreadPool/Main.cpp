#include "ThreadPool.h"
#include  <iostream>
#include <chrono>
int main()
{
	for (int i = 0; i < 10; i++) {
		ThreadPool::getIntance().enqueue([i] {
			printf("task : 第 %d 任务开始\n", i);
			//std::cout << "task : 第" << i << " 任务开始" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("task : 第 %d 任务结束\n", i);
			//std::cout << "task : 第" << i << " 任务结束" << std::endl;
		});
		
	}
	return 0;
}