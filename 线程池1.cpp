#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <functional>
#include <condition_variable>

using namespace std;
class threadpool{
    private:
    vector<thread> works;  //工作线程
    queue<function<void()>> tasks; //任务队列
    mutex mtx;  //锁
    condition_variable s;  //没任务时线程睡觉
    bool stop;  //控制线程池是否结束
     
    public:
    threadpool(int n=10):stop(false){
    for(int i=0;i<n;i++){
    works.emplace_back([this](){
      while(true){
        function<void()> task;
        { //控制锁的作用域
          unique_lock<mutex> lock(this->mtx);
          this->s.wait(lock,[this](){
          return this->stop|| !this->tasks.empty();
          });
          if(this->stop&&this->tasks.empty()){
            return ;
          }
          task=move(this->tasks.front());
          tasks.pop();
        }
        task();
      }
    });
    }
  }

  //将任务放进队列然后唤醒线程
  void enqueue(function<void()> f){
    {
  unique_lock<mutex> lock(mtx);
  tasks.push(move(f));
    }
  s.notify_one();
  }

  //关闭线程池
  void close(){
    {
      unique_lock<mutex> lock(mtx);
      stop=true;
    }
    s.notify_all();
    for(thread &t:works){
      if(t.joinable()){
        t.join();
      }
    }
  }
  //关闭以后，清理
      ~threadpool(){
        close();
      }
  };

  //阶乘函数
  long long func(int n){
    if(n==1||n==0)return 1;
    return n*func(n-1);
  }

  int main(){
    threadpool pool(5);
    mutex cout_mtx;  

    for(int i=0;i<10;i++){
        pool.enqueue([i, &cout_mtx](){
            long long result = func(i);

            lock_guard<mutex> lock(cout_mtx);
            cout << i << "! = " << result << endl;
        });
    }

    pool.close();
    return 0;
}