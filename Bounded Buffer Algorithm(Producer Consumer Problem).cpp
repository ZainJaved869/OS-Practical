#include<iostream>
// for thread
#include<thread>
//protect shared data
#include<mutex>
// store
#include<queue>
// condition_variable: to block and wake threads
#include <condition_variable>
// store list of thread


using namespace std;
// buffersize
const int BUFFER_SIZE = 5;
//  Limit per producer and consumer to 10 
const int MAX_ITEMS = 10; 
//creating queueu name buffer store int data 
queue<int> buffer;

// shared varaible 
mutex mtx;
// block the producer if buffer is full.
condition_variable not_full;
// block the consumer if buffer is empty.
condition_variable not_empty;
// current number of items in buffer
int count_items = 0;

void producer(int id) {
    for (int i = 1; i <= MAX_ITEMS; i++) {
    	//lock mutex
        unique_lock<mutex> lock(mtx);
//if buffer full than producer wait until concumer consume them
        not_full.wait(lock, []() {
            return buffer.size() < BUFFER_SIZE;
        });
//push an 1 item to buffer
        buffer.push(i);
        cout << "Producer " << id << " produced item " << i << endl;
//iems increment
        count_items++;
        //lock relaese
        lock.unlock();
//Wake up one waiting consumer
        not_empty.notify_one();
        // sleep 
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void consumer(int id) {
    for (int i = 1; i <= MAX_ITEMS; ++i) {
        unique_lock<mutex> lock(mtx);
        
//if buffer empty than consumer  wait until produce produce them
        not_empty.wait(lock, []() {
            return !buffer.empty();
        });
// remove iem
        int item = buffer.front();
        // buffer empty
        buffer.pop();
        cout << "Consumer " << id << " consumed item " << item << endl;
//item decrement
        count_items--;
        //lock relaese
        lock.unlock();
//Wake up one waiting consumer
        not_full.notify_one();
                // sleep 
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

int main() {
	//Start 2 producer threads and 2 consumer threads, passing IDs.
    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);

//Wait for all threads to finish before exiting.
    p1.join();
    p2.join();
    c1.join();
    c2.join();

    cout << "All items produced and consumed." << endl;
    system("pause");
    return 0;
}
