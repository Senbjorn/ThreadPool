#ifndef SG_LIB_THREADPOOL
#define SG_LIB_THREADPOOL
#include "threadpool.h"
#include <iostream>
#include <vector>

int fib(int n) {
	if (n <= 2) {
		return 1;
	}
	return fib(n - 1) + fib(n - 2);
}

int sum = 0;

void count(int n) {
	for (int i = 0; i < n; i++) {
		sum += i + 1;
	}
}

double pi_calc(int n) {
	double sum = 0;
	double i = 1;
	for (int k = 1; k < n; k++) {
		sum += 1. / (2*i - 1) * (2 * (k % 2) - 1);
		i ++;
	}
	return sum * 4;
}

int main() {
	AbstractThreadPool* pool = (new FixedThreadPool(4));
	Task<int, int> task(fib, 10);
	Executable& e = task;
	e.execute();
	std::cout << task.get_result() << std::endl;
	std::vector<Task<double, int>*> tasks;
	for (int i = 0; i < 20; i++) {
		tasks.push_back(new Task<double, int>(pi_calc, 100000000));
		pool->push(*tasks.back());
	}
	for (Task<double, int>* task_ptr: tasks) {
		Task<double, int>& task = *task_ptr;
		std::cout << "pi: " << task.get_result() << std::endl;
	}	
}
#endif
