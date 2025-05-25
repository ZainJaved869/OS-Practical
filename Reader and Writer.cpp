#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

// Shared variables
mutex mtx;          // For readcount updates
mutex wrt;          // Writer lock
int readcount = 0;

// Reader thread function
void reader(int id) {
    for (int i = 0; i < 5; ++i) {
        {
            unique_lock<mutex> lock(mtx);
            readcount++;
            if (readcount == 1)
                wrt.lock();  // First reader locks writer
        }

        // Reading section
        cout << "Reader " << id << " is reading\n";
        this_thread::sleep_for(chrono::seconds(1));

        {
            unique_lock<mutex> lock(mtx);
            readcount--;
            if (readcount == 0)
                wrt.unlock();  // Last reader unlocks writer
        }

        this_thread::sleep_for(chrono::seconds(1)); // Time between reads
    }
}

// Writer thread function
void writer(int id) {
    for (int i = 0; i < 5; ++i) {
        wrt.lock();  // Lock for writing

        // Writing section
        cout << "Writer " << id << " is writing\n";
        this_thread::sleep_for(chrono::seconds(2));

        wrt.unlock();  // Unlock after writing
        this_thread::sleep_for(chrono::seconds(2)); // Time between writes
    }
}

// Main function
int main() {
    // Create threads
    thread r1(reader, 1);
    thread r2(reader, 2);
    thread w1(writer, 1);

    // Wait for all threads to finish
    r1.join();
    r2.join();
    w1.join();

    cout << "All readers and writers finished.\n";
    system("pause");
    return 0;
}
