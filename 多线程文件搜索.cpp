#include<bits/stdc++.h>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <filesystem>

using namespace std;

namespace f=std::filesystem;

struct SearchConfig {
    std::string root_path;    // 要搜索的根目录
    std::string file_type;    // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency;      // 最大并发数
    int max_depth;            // 最大搜索深度
    bool skip_hidden;         // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths;   // 要跳过的目录或文件的路径
};

using s=SearchConfig;

class Searchfile{
    private:
     s config;
     vector<thread> threads;
     queue<pair<f::path,int>> tasks;   //路径和深度
     mutex mtx;    //锁住任务
     mutex mt;     //锁住结果
     condition_variable cv; //信号
     set<string> result;
     int worknum=0; //执行工作次数
     bool stop=false;

     public:

     //初始化
     Searchfile (s &obj):config(obj){};

     //找到跳过指定的目录和文件
     bool isskip(const f::path &p){
     string name=p.filename().string();
     if(config.skip_hidden && !name.empty() &&  name[0]=='.'){
        return true;
     }
     string full=p.string();
     for(auto &pa : config.skip_paths){
        if(full.find(pa)!=string::npos){
            return true;
        }
     }
     return false;
     }

     //工作内容
     void worker(){
      while(1){
     pair<f::path,int> task;
     {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock,[&](){
          return stop || !tasks.empty();
        });
        if(stop&&tasks.empty()){
            return ;
        }
        task=tasks.front();
        tasks.pop();
             worknum++;
     }
       f::path path=task.first;
       int depth=task.second;
       if(depth<=config.max_depth){
        try{
        for(auto &entry:f::directory_iterator(path)){
            if(isskip(entry.path())) continue;
            if(entry.is_directory()){
                {
                unique_lock<mutex> lock(mtx);
                tasks.push({entry.path(),depth+1});
                }
                cv.notify_one();
            }
            else if(entry.is_regular_file()){
                //判断文件后缀
                if(entry.path().extension()==config.file_type){
                    {
                    unique_lock<mutex> lock(mt);
                    //将结果转化为绝对路径存储进去
                    result.insert(f::absolute(entry.path()).string());
                    }
                }
            }

        }
        }
        catch(...){}
       }
             worknum--;
        
        {
            unique_lock<mutex> lock(mtx);
       if(worknum==0&&tasks.empty()){
        stop=true;
        cv.notify_all();
       }
    }
      }
     }

     //执行任务
     void run(){
        tasks.push({config.root_path,0});
        for(int i=0;i<config.max_concurrency;i++){
          threads.emplace_back(&Searchfile::worker,this);
        }

        for(auto &t:threads){
            t.join(); 
        }
     }

     //打印结果
     void print(){
        for(auto res:result){
            cout<<res<<endl;
        }
     }
};

s exec(int argc,char *argv[]){
   s config;
   config.root_path=".";
   config.file_type=".cpp";
   config.max_concurrency=4;
   config.max_depth=5;
   config.skip_hidden=true;
   config.skip_paths={".git", "build"};

      if (argc > 1) {
        config.root_path = argv[1];
    }

    if (argc > 2) {
        config.file_type = argv[2];
    }

    if (argc > 3) {
        config.max_concurrency = stoi(argv[3]);
    }

    if (argc > 4) {
        config.max_depth = stoi(argv[4]);
    }

    return config;
}

int main(int argc,char *argv[]){
    s config=exec(argc,argv);
    Searchfile fs(config);
    fs.run();
    fs.print();
    return 0;
}