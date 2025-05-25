#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <random>

using namespace std;
const int CHAIRS = 3, CUSTOMERS = 10;

mutex seats_mutex;
condition_variable customers_cv, barber_cv;

int freeSeats = CHAIRS;
int waiting_customers = 0;
bool barber_ready = false;

void sleep_random(int min = 500, int max = 1500) {
    static random_device rd; 
	static mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    this_thread::sleep_for(chrono::milliseconds(dist(gen)));
}

void barber_thread() {
    while (true) {
        unique_lock<mutex> lock(seats_mutex);
        customers_cv.wait(lock, [] { return waiting_customers > 0; });  // Wait for customer

        waiting_customers--;
        freeSeats++;  // One seat becomes free
        barber_ready = true;
        barber_cv.notify_one();  // Signal customer to get haircut
        lock.unlock();

        cout << "Barber is cutting hair...\n";
        sleep_random(1000, 2000);
        cout << "Barber finished haircut.\n";
    }
}

void customer_thread(int id) {
    this_thread::sleep_for(chrono::milliseconds(100 * id));

    unique_lock<mutex> lock(seats_mutex);
    if (freeSeats > 0) {
        freeSeats--;
        waiting_customers++;
        cout << "Customer " << id << " is waiting.\n";
        customers_cv.notify_one();  // Notify barber
        barber_cv.wait(lock, [] { return barber_ready; });  // Wait for barber to be ready
        barber_ready = false;
        lock.unlock();

        cout << "Customer " << id << " is getting a haircut.\n";
    } else {
        cout << "Customer " << id << " left (no seat).\n";
    }
}

int main() {
    thread b(barber_thread);
    vector<thread> c;
    for (int i = 1; i <= CUSTOMERS; ++i)
        c.emplace_back(customer_thread, i);
    for (auto& t : c) t.join();
    b.detach();
    system("pause");
    return 0;
}
