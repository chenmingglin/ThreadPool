#include "ThreadPool.h"
#include  <iostream>
#include <chrono>
int main()
{
	for (int i = 0; i < 10; i++) {
		ThreadPool::getIntance().enqueue([i] {
			printf("task : �� %d ����ʼ\n", i);
			//std::cout << "task : ��" << i << " ����ʼ" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("task : �� %d �������\n", i);
			//std::cout << "task : ��" << i << " �������" << std::endl;
		});
		
	}
	return 0;
}