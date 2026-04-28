#include <bits/stdc++.h>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>

using namespace std;
 
class threadpool{

    private:
vector<thread> threads;
queue<function<void()>> works;
int worknum;
mutex mtx;
bool stop;
condition_variable cv;
condition_variable finish_cv;

public:
threadpool(int num):stop(false){
    for(int i=0;i<num;i++){
        threads.emplace_back([this](){
              while(1){
            function<void()> task;
            {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock,[this](){
                return stop||!works.empty();
            });
            if(stop&&works.empty()){
                return ;
            }
            task=move(works.front());
            works.pop();
            }

            task();
            {
                unique_lock<mutex> lock(mtx);
                worknum--;
                if(worknum==0){
                    finish_cv.notify_all();
                }
            }
        }
        });
    }

}


void enqueue(function<void()> f){
    {
        unique_lock<mutex> lock(mtx);
        works.push(f);
        worknum++;
    }
    cv.notify_one();
}

void waitt(){
    {
        unique_lock<mutex> lock(mtx);
        if(worknum!=0){
            finish_cv.wait(lock);
        }
    }
}
void clean(){
    {
        unique_lock<mutex> lock(mtx);
        stop=true;
    }
    cv.notify_all();
    for(auto &t:threads){
    if(t.joinable()){
        t.join();
    }
    }
}

~threadpool(){
    clean();
}
};

mutex mt;

void download(string filename,string url){
{
    unique_lock<mutex> lock(mt);
    cout<<"start: "<<filename<<endl;
}
string cmd="wget -q -O " + filename +" " + url;
system(cmd.c_str());
{
    lock_guard<mutex> lock(mt);
    cout<<"finish: "<<filename<<endl;
}
}
int main(){
    threadpool pool(10);
    cout<<"input download num: "<<endl;
    int n;
    cin>>n;
    //数组存储信息
    vector<pair<string,string>> inputs;
    for(int i=0;i<n;i++){
        string filename,url;
        cout<<"filename: "<<endl;
        cin>>filename;
        cout<<"url: "<<endl;
        cin>>url;
        inputs.push_back({filename,url});
    }
    for(auto &p:inputs){
        pool.enqueue([=](){
        download(p.first,p.second);
        });
    }
    pool.waitt();
    return 0;
}