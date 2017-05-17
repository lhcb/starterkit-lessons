// Compile with:
// g++ -std=c++14 future.cpp

#include <iostream>
#include <thread>
#include <future>

using namespace std::chrono_literals;

void some_slow_function(std::promise<int> p) {
    for(int i=0; i<5; i++) {
        std::this_thread::sleep_for(1s);
        std::cout << "Thread working" << std::endl;
     }
     p.set_value(42);
}

int main() {
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t(some_slow_function, std::move(p));
    std::cout << "Main thread" << std::endl;
    std::this_thread::sleep_for(1.5s);
    std::cout << "Main thread still" << std::endl;
    std::cout << "The result is " << f.get() << std::endl;
    t.join();
    return 0;
}
