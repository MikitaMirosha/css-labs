#include <chrono>
#include <iostream>
using namespace std;

int main()
{
    cout << "Child process time:" << endl;
    cout << chrono::system_clock::to_time_t(chrono::system_clock::now()) << endl;
}
