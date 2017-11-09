#include <iostream>
#include <string>
#include <complex>
#include <chrono>

using namespace std;

int main() {
    using namespace std::string_literals;
    using namespace std::literals::chrono_literals;

    auto duration = 1h + 3min;
    cout << "A duration: " << chrono::duration_cast<chrono::seconds>(duration).count() << " s" << endl;


    cout << "This is a "s + "real C++ string!"s << endl;

    cout << "This is a big number: " << 1'000'000 << endl;


    return 0;
}
