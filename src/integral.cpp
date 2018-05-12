#include <iostream>
#include <thread>
#include <sys/sysinfo.h>
#include <vector>
#include <cmath>
#include "threadpool.h"
#include <functional>

double pi = 3.1415926535897;

double func(double x);

double sinx(double x);

double integrate(double (*func)(double), double left, double right, int n);

int main(int argc, char *argv[]) { std::cout << get_nprocs() << std::endl;
	double left = 0;
	double right = 0.5 * pi;
	int n = 100000000;
	int nthreads = std::stoi(argv[1]);
	int tn = n / nthreads;
	double step = (right - left) / nthreads;
	double integral = 0;
	std::vector<Task<double, double(*)(double), double, double, int>*> pres;
	std::cout << "Create pool" << std::endl;
	/*
	std::thread t = std::thread(integrate,
		 				func,
						left, right, n, std::ref(integral));
	t.join();
	
	*/
	AbstractThreadPool* pool = (new FixedThreadPool(4));
	//std::function<double(*)(double(*)(double), double, double, int)> my_func(&integrate);
	for (int i = 0; i < nthreads; i++) {
		double ileft = left + i * step;
		double iright = left + (i + 1) * step;
		pres.push_back(new Task<double, double(*)(double), double, double, int>(integrate, &func, ileft, iright, tn));
		pool->push(*pres.back());
	}
	for (int i = 0; i < nthreads; i++) {
		integral += pres[i]->get_result();
	}
	std::cout << integral << std::endl;
	delete pool;
}

double func(double x) {
	return (x*x);
}

double sinx(double x) {
	return std::sin(x);
}

double integrate(double (*func)(double), double left, double right, int n) {
	double local_res = 0;
	double step = (right - left) / n;
	for(double x = left; x < right; x += step) {
		local_res += func(x) * step;
	}
	return local_res;
}
