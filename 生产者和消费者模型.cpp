#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
using namespace std;
queue<int> works;
mutex mt;
condition_variable s; 
void producer(){
    for(int i=0;i<10;i++){
    {
        unique_lock<mutex> l(mt);
        works.push(i);
        cout<<"producer: "<<i<<endl;
        s.notify_one();
    }
      this_thread::sleep_for(chrono::microseconds(100));
    }
}

void consumer(){
    while(1){
    {
        unique_lock<mutex> l(mt);
        s.wait(l,[](){
            return !works.empty();
        });
        int value=works.front();
        works.pop();
        cout<<"consumer: "<<value<<endl;
    }
  }
}

int main(){
    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}