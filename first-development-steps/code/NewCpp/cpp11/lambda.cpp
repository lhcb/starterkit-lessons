#include <vector>
#include <iostream>
#include <memory>
#include <functional>

using namespace std;

int square(int x) {
    return x*x;
}

template<typename T>
void apply_to_vector(vector<T> &vec, std::function<T(T)> f) {
    for(auto &v : vec)
        v = f(v);
}

int main() {
    vector<int> a_vector = { 3, 1, 4 };
    
    auto lambda_print = [](const std::vector<int> &vec){
        for(auto const &item : vec)
            cout << item << " ";
        cout << endl;
    };

    cout << "Printing a vector with a lambda function:" << endl;
    lambda_print(a_vector);

    cout << "Creating a lambda that captures something" << endl;
    auto print_existing_vector = [&a_vector](){
        for(auto const &item : a_vector)
            cout << item << " ";
        cout << endl;
    };

    cout << "Nothing is passed in:" << endl;
    print_existing_vector();

    cout << "Using a function as a parameter:" << endl;
    std::function<int(int)> f_square = square;
    apply_to_vector(a_vector, f_square);
    print_existing_vector();

}
