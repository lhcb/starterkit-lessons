#include <vector>
#include <iostream>

using namespace std;

int main() {
    cout << "Creating vector with list initializer" << endl;
    vector<int> a_vector = { 3, 1, 4 };
    cout << "Using a new style loop:" << endl;
    
    // Often uses auto here
    for(int x : a_vector)
        cout << x << " ";
    cout << endl;

    cout << "Looping without wasting a copy:" << endl;
    for(const int &x : a_vector)
        cout << x << " ";
    cout << endl;

    cout << "The classic method:" << endl;
    for(vector<int>::const_iterator x_p = a_vector.begin();
        x_p != a_vector.end();
        x_p++) {
        const int &x = *x_p;
        cout << x << " ";
    }
    cout << endl;

    return 0;

    // C++ allows begin(a_vector), end(a_vector) as well.
}
