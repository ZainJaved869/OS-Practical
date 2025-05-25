#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

using namespace std;

const int N = 5; // Number of philosophers
mutex chopsticks[N]; // One chopstick (mutex) between each pair

// Simulate random time delay between 'min' and 'max' milliseconds
void random_sleep(int min = 1000, int max = 3000) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    this_thread::sleep_for(chrono::milliseconds(dist(gen)));
}

// Each philosopher will dine a fixed number of times
void dine(int id, int meals = 3) {
    int left = id;
    int right = (id + 1) % N;

    for (int i = 0; i < meals; ++i) {
        cout << "Philosopher " << id << " is thinking.\n";
        random_sleep();
        cout << "Philosopher " << id << " is hungry.\n";

        int first = min(left, right);
        int second = max(left, right);
        lock(chopsticks[first], chopsticks[second]);
        lock_guard<mutex> l1(chopsticks[first], adopt_lock);
        lock_guard<mutex> l2(chopsticks[second], adopt_lock);

        cout << "Philosopher " << id << " is eating. (" << (i + 1) << "/" << meals << ")\n";
        random_sleep(1000, 2000);
        cout << "Philosopher " << id << " finished eating.\n";
    }

    cout << "Philosopher " << id << " has left the table.\n";
}

int main() {
    thread philosophers[N];
    for (int i = 0; i < N; ++i)
        philosophers[i] = thread(dine, i, 3); // Each philosopher eats 3 times

    for (auto& p : philosophers)
        p.join();

    cout << "All philosophers have finished dining.\n";
}
