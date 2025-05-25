#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std;

const int NUM_PROCESSES = 2;
atomic<bool> interested[NUM_PROCESSES];
atomic<int> turn;

void peterson_algorithm(int process) {
    int other = 1 - process;

    // Entry Section
    interested[process] = true;
    turn = process;
    while (interested[other] && turn == process) {
        // busy wait
    }

    // Critical Section
    cout << "Process " << process << " is in the critical section." << endl;
    this_thread::sleep_for(chrono::seconds(1)); // Simulate work

    // Exit Section
    interested[process] = false;
}

int main() {
    interested[0] = false;
    interested[1] = false;
    turn = 0;

    thread t1(peterson_algorithm, 0);
    thread t2(peterson_algorithm, 1);

    t1.join();
    t2.join();
   system("pause"); 
    return 0;
}
