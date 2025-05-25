#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

using namespace std;

const int N = 5; // Number of threads/processes

atomic<bool> choosing[N];
atomic<int> number[N];

void bakery_lock(int i) {
    choosing[i] = true;

    // Assign max(number) + 1
    int max_ticket = 0;
    for (int j = 0; j < N; j++) {
        int ticket = number[j];
        if (ticket > max_ticket)
            max_ticket = ticket;
    }
    number[i] = max_ticket + 1;
    choosing[i] = false;

    // Wait until no other process has higher priority
    for (int j = 0; j < N; j++) {
        while (choosing[j]); // Wait until j receives its number
        while (number[j] != 0 && (number[j] < number[i] || (number[j] == number[i] && j < i)));
    }
}

void bakery_unlock(int i) {
    number[i] = 0;
}

void critical_section(int id) {
    for (int k = 0; k < 3; ++k) {
        bakery_lock(id);

        // Begin critical section
        cout << "Thread " << id << " is in the critical section\n";
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate work
        cout << "Thread " << id << " is leaving the critical section\n";
        // End critical section

        bakery_unlock(id);

        this_thread::sleep_for(chrono::milliseconds(100)); // Remainder section
    }
}

int main() {
	// initalize process i value
    thread t[N];
    for (int i = 0; i < N; i++) {
        choosing[i] = false;
        number[i] = 0;
    }
// create thread
    for (int i = 0; i < N; i++) {
        t[i] = thread(critical_section, i);
    }
// wait for all process
    for (int i = 0; i < N; i++) {
        t[i].join();
    }
 system("pause"); 
    return 0;
}
