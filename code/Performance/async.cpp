// Compile with:
// g++ -std=c++14 future.cpp

#include <iostream>
#include <thread>
#include <future>

using namespace std::chrono_literals;

int some_slow_function(int time) {
    for(int i=0; i<time; i++) {
        std::this_thread::sleep_for(1s);
        std::cout << "Thread working" << std::endl;
     }
     return 42;
}

int main() {
    std::future<int> f = std::async(std::launch::async, some_slow_function, 5);

    std::cout << "Main thread" << std::endl;
    std::this_thread::sleep_for(1.5s);
    std::cout << "Main thread still" << std::endl;
    std::cout << "The result is " << f.get() << std::endl;
    return 0;
}
