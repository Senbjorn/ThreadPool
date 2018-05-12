#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <mutex>


template<typename T>
int function(T a) {
	return a;
}

template<typename T, typename ...Args>
int function(T a, Args... args) {
	return a + function(args...);
}

template<typename T, typename ...Args>
double call_function(T f, Args... args) {
	return f(args...);
}

double sinxy(double x, double y) {
	return std::sin(x+y);
}

double pi() {
	return M_PI;
}


template<typename R, typename ...Args>
class Task {
private:
	std::function<R(void)> function;
public:
	
	Task() = delete;
	Task(std::function<R(Args...)> f, Args ...args) {
		function = [f, args...] ()->R {
			return f(args...);
		};
	}

	R execute() {
		return function();
	}
};

int main(int argn, char** argv) {
	Task<double, double, double> t1(sinxy, 0.5, -0.5);
	std::cout << function(1, 2, 3, 4, 5, 6) << std::endl;
	std::cout << call_function(pi) << std::endl;
	std::cout << t1.execute() << std::endl;	
}
