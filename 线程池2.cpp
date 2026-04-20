/*#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <vector>
#include <queue>
#include <atomic>

using namespace std;

class threadpool{
    private:
    vector<thread> works;
    queue<function<void()>> tasks;
    condition_variable cv;//有没有任务（工作线程）
    mutex mtx;
    bool stop;
    atomic<int> task_cnt{0};  //统计还有多少任务没有执行完
    condition_variable finished_cv; //条件信号，判断任务是否全部执行完（主线程）
    public:

    threadpool(int n=10){
        stop=false;
        task_cnt=0;
        for(int i=0;i<n;i++){
       works.emplace_back([this](){
        while(true){
            function<void()> task;
            {
                unique_lock<mutex> lock(this->mtx);
                while(!stop&&tasks.empty()){
                    this->cv.wait(lock);
                }
                if(stop&&tasks.empty()){
                    return ;
                }
            task=move(tasks.front());
            tasks.pop();
            }
            task();
            {
                unique_lock<mutex> lock(this->mtx);
                task_cnt--;
                if(task_cnt==0){
                    finished_cv.notify_all();
                }
            }
        }
       });
    }
    }

    //把“带参数的函数” → 包装成“无参数任务” → 丢进队列
    //让enqueue可以接受任意类型的参数
    template<class F,class... Args>
    void enqueue(F f,Args...args){
    {
        unique_lock <mutex> lock(this->mtx);
        tasks.push([=](){
          f(args...);
        });
        task_cnt++;
    }
       cv.notify_one();
    }

    void waitthread(){
    {
        unique_lock<mutex> lock(this->mtx);
        while(task_cnt!=0){
            finished_cv.wait(lock);
        }
    }
    }

    void clean(){
        {
            unique_lock<mutex> lock(this->mtx);
            stop=true;
        }
        cv.notify_all();
        for(thread &t:works){
            if(t.joinable()){
                t.join();
            }
        }
    }

    ~threadpool (){
        clean();
    }
};

  mutex cout_mtx;
void download(string url,string filename){
    {
    unique_lock lock(cout_mtx);
    cout<<"开始下载"<<url<<endl;
    }
    string cmd="wget -q -O"+filename+" "+url;
    system(cmd.c_str());
    {
      unique_lock lock(cout_mtx);    
    cout<<"下载完成"<<filename<<endl;    
    }
}

int main(){
    threadpool pool(4);
    int n;
    cout<<"请输入要下载文件的数量 "<<endl;
    cin>>n;
    vector<pair<string,string>> inputs;
    for(int i=0;i<n;i++){
        string url,filename;
        cout<<"请输入url";
        cin>>url;
        cout<<"请输入文件名";
        cin>>filename;
        inputs.push_back({url,filename});
    }
    for(auto &p:inputs){
        pool.enqueue(download,p.first,p.second);
    }
    pool.waitthread();
    return 0;
}
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <vector>
#include <queue>

using namespace std;

class threadpool {
private:
    vector<thread> works;
    queue<function<void()>> tasks;

    condition_variable cv;
    condition_variable finished_cv;
    mutex mtx;

    bool stop;
    int task_cnt; 

public:
    threadpool(int n = 10) {
        stop = false;
        task_cnt = 0;

        for (int i = 0; i < n; i++) {
            works.emplace_back([this]() {
                while (true) {
                    function<void()> task;

                    {
                        unique_lock<mutex> lock(this->mtx);

                        while (!stop && tasks.empty()) {
                            cv.wait(lock);
                        }

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = move(tasks.front());
                        tasks.pop();
                    }

                    task();

                    {
                        unique_lock<mutex> lock(this->mtx);
                        task_cnt--;   
                        if (task_cnt == 0) {
                            finished_cv.notify_all();
                        }
                    }
                }
            });
        }
    }

    void enqueue(function<void()> task) {
        {
            unique_lock<mutex> lock(this->mtx);
            tasks.push(task);
            task_cnt++;   
        }
        cv.notify_one();
    }

    void waitthread() {
        unique_lock<mutex> lock(this->mtx);
        while (task_cnt != 0) {
            finished_cv.wait(lock);
        }
    }

    void clean() {
        {
            unique_lock<mutex> lock(this->mtx);
            stop = true;
        }

        cv.notify_all();

        for (thread &t : works) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    ~threadpool() {
        clean();
    }
};

mutex cout_mtx;

void download(string url, string filename) {
    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "开始下载 " << url << endl;
    }

    string cmd = "wget -q -O " + filename + " " + url;
    system(cmd.c_str());

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "下载完成 " << filename << endl;
    }
}
int main(){
    threadpool pool(4);

    int n;
    cout<<"请输入要下载文件的数量: ";
    cin>>n;

    vector<pair<string,string>> inputs;


    for(int i=0;i<n;i++){
        string url,filename;
        cout<<"请输入url: ";
        cin>>url;
        cout<<"请输入文件名: ";
        cin>>filename;
        inputs.push_back({url,filename});
    }


    for(auto &p:inputs){
        pool.enqueue([=](){
            download(p.first,p.second);
        });
    }

    pool.waitthread();

    return 0;
}