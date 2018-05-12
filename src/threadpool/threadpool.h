#ifndef SGLIB_THREAD_POOL_H
#define SGLIB_THREAD_POOL_H
#include <functional>
#include <mutex>
#include <memory>
#include <type_traits>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>
#include <iostream>


/*
Abstrac executable object description.
*/
class Executable {
public:
	virtual void execute() = 0;
	virtual bool is_ready() = 0;
};


/*
class Task wraps arbitrary function with arguments passed.
Function can be called via execute() method.
It is not allowed to call function twice.
Once execute() is called is_ready() method will return true.
This class can work only with not void functions.
The resutl of function is available via get_result() method.
It is not copyable, moveable, copyassignable and moveassigable.
*/
template<typename R, typename ...Args>
class Task: public Executable {
private:
	std::mutex mutex;
	std::condition_variable cond;	
	std::function<R()> function;
	bool ready = false;
	std::unique_ptr<R> result;
public:
	~Task() {};
	
	Task() = delete;
	
	Task(std::function<R(Args...)> f, Args... args);

	Task(const Task& other) = delete;

	void execute() {
		if (ready) return;
		result = std::make_unique<R>(function());
		std::unique_lock<std::mutex> lock(mutex);
		ready = true;
		lock.unlock();
		cond.notify_all();
	}



	bool is_ready() {
		std::lock_guard<std::mutex> lock(mutex);
		return ready;
	}
	
	//R& try_result();
	
	R& get_result() {
		std::unique_lock<std::mutex> lock(mutex);
		cond.wait(lock, [this] {return ready;});
		return *result;
	}
};

/*
Abstract threadpool object description.
*/
class AbstractThreadPool
{
public:
	virtual void push(Executable& t) = 0;
};


/*
Threadpool with fixed number of workers;
*/
class FixedThreadPool: public AbstractThreadPool
{
private:
	std::mutex mutex;
	std::condition_variable cond;
	std::queue<Executable*> tasks;
	std::vector<std::thread> workers;
public:	

	FixedThreadPool(int thr);

	void push(Executable& e) {
		std::unique_lock<std::mutex> lock(mutex);
		tasks.push(&e);
		lock.unlock();
		cond.notify_one();
	}
	
	void do_work();
};

template<typename R, typename ...Args>
Task<R, Args...>::Task(std::function<R(Args...)> f, Args... args) {
	function = [f, args...] () -> R {
		return f(args...);
	};
}

FixedThreadPool::FixedThreadPool(int thr) {
	for (int i = 0; i < thr; i++) {
		workers.push_back(std::thread(&FixedThreadPool::do_work, this));
	}
}

void FixedThreadPool::do_work() {
	std::cout << "!" << std::endl;
	while (true) {
		std::unique_lock<std::mutex> lock(mutex);
		std::cout << "on wait" << std::endl;
		cond.wait(lock, [this] {return !tasks.empty();});
		Executable& task = *(tasks.front());
		tasks.pop();
		lock.unlock();
		task.execute();
	}
}


#endif
