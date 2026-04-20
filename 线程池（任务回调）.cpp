
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <vector>
#include <functional>
#include <chrono>

using namespace std;
 class threadpool{
  public:
threadpool(int numthread) :stop(false) {
		 //创建线程
		for (int i = 0; i < numthread; i++) {
			//lambda函数
			threads.emplace_back([this]() {
				while (1) {
          Task task;
          {
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
				 task=move(tasks.front());
					tasks.pop();
				}
        long long result=task.work();
        if(task.callback){
          task.callback(result);
        }
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
  void enqueue(function<long long()> work,
  function<void(long long)> callback ){
    {
      unique_lock<mutex> lock(mtx);
      tasks.push(Task{move(work), move(callback)});
    }
    condition.notify_one();
  }

private:

//任务结构
struct Task{
  function<long long()> work;//计算阶乘
  function<void(long long)> callback;//回调
};
vector<thread> threads;//线程数组
queue<Task> tasks;//任务队列，每一个成员都是Task
mutex mtx;//互斥锁
condition_variable condition;//通知信号

bool stop;//线程是否终止
 };

 //阶乘函数
 long long func(int n){
  if(n==1||n==0)return 1;
  return n*func(n-1);
 }

mutex mt;

 int main(){
  threadpool pool(10);
  for(int i=1;i<=10;i++){
       pool.enqueue([i]()->long long{
        {
      lock_guard<mutex> lock(mt);
      cout<<"计算"<<i<<endl;
        }
      this_thread::sleep_for(chrono::seconds(1));
      return func(i);
       },
       [i](long long result){
        {
          lock_guard<mutex> lock(mt);
        cout<<"完成"<<i<<" 阶乘 = "<<result<<endl;
       }
      }
      );
  }
  return 0;
 }