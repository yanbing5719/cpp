#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <vector>
#include <functional>

using namespace std;
 class threadpool{
  public:
threadpool(int numthread) :stop(false) {
		 //创建线程
		for (int i = 0; i < numthread; i++) {
			//lambda函数
			threads.emplace_back([this]() {
				while (1) {
					unique_lock<mutex> lock(mtx);
					condition.wait(lock, [this]() {
						//线程为空或者终止，发出信号（生产者和消费者模型）
						return !tasks.empty() || stop;
						});
					//线程终止，直接return 
					if (stop&&tasks.empty()) {
						return;
					}
					 //取出任务
					function<void()> task=move(tasks.front());
					tasks.pop();
					task();
				}
				});
		}
	}

	//析构函数：在对象生命周期结束时自动调用，用于执行清理工作（如释放资源、关闭文件等）
	
	~threadpool() {
		{
			unique_lock<mutex> lock(mtx);
			stop = true;
		}
		//通知所有线程取完任务
		condition.notify_all();
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		}

	//向任务队列里面添加任务
  void enqueue(function<void()> f){
    {
      unique_lock<mutex> lock(mtx);
      tasks.push(f);
    }
    condition.notify_one();
  }

private:
vector<thread> threads;//线程数组
queue<function <void()>> tasks;//任务队列，每一个成员都是函数
mutex mtx;//互斥锁
condition_variable condition;//通知信号

bool stop;//线程是否终止
 };