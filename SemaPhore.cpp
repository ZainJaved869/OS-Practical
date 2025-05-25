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
#include<vector>
class Semaphore{
	private:
		// for check resource (1)
		int value;
		// queue to check the waiting thread 
	   queue<thread::id> waiting_threads;
		// ensure only one wiating thread
		mutex mtx;
		
		//block and resume the thread for waiting the semaphore 
		condition_variable cv;
		public:
			// constructor
			Semaphore(int inital ):value(inital){
			}
			void Down(){
			
			// lock the mutex internal state
			unique_lock<mutex>lock(mtx);
			// request access to CS
			value--;
			
			if(value<0){
			//get the thread id 
			thread::id tid=this_thread::get_id();
			// add t the waiting queue
			waiting_threads.push(tid);
			cout<<"  Thread"<<tid <<" is blocked"<<endl;
			// wait the thread
			cv.wait(lock,[&]() {
				//// Wait until it’s the thread's turn
				return waiting_threads.front()==this_thread::get_id();
		   });
			// removing fron queue
			waiting_threads.pop();	
			}
		}
			// for critical section
			void Up(){
				//lock  state 
				unique_lock<mutex>lock(mtx);
				//release access 
				value++;
				// if calue is 0 then wake up the process
				if(value<=0){
					cv.notify_all();
				}
			}
		};
			void critical_task(Semaphore &sem,int id){
				//ensure thread enter two time in CS
				for (int i=0; i<2; i++){
					sem.Down();
					cout<<"  Thread"<<id <<" is in CS"<<endl;
					// sleep
					this_thread::sleep_for(chrono::milliseconds(100));
						cout<<"  Thread"<<id <<" is leaving the  CS"<<endl;
						sem.Up();
					
				}
			}
					

int main(){
	// num of thread
	const int  NUM_THREADS=3;
	//(only 1 thread allowed at a time)
	Semaphore sem(1);
	//create thread
	vector<thread>threads;
	// run those who in the critcal sectin
	 for(int i=0; i<NUM_THREADS; ++i){
	 	threads.emplace_back(critical_task,ref(sem),i+1);
	 }
	 /// Wait for all threads to finish
	 for(auto &t:threads){
	 	t.join();
	 }
	  system("pause"); // Windows only
    return 0;
}

