#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <filesystem>
#include <condition_variable>

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

class FileSearch{
    private:
    s obj;
    //此处的path是了f里面的类，
    queue<pair<f::path,int>> tasks; //路径，深度
    mutex mtx;//锁住任务队列
    condition_variable cv;
    bool stop=false;
    vector<thread> threads;
    set<string> result;//存放结果，自动排序
    mutex mt;//锁结果
    int working = 0;
    public:
    //初始化
    FileSearch(s& aim):obj(aim){}
        
        //判断是否要跳过一些特殊情况
        bool isskip(const f::path &p){
            //获取文件名
            string name=p.filename().string();
            if(obj.skip_hidden && !name.empty() && name[0]=='.'){
                return true;
            }
            string full=p.string();
            for(auto &t:obj.skip_paths){
                if(full.find(t)!=string::npos){
                 return true;
                }//判断是否找到改路径
            }
            return false;
        }
    
        //任务函数
    void worker(){
        while(1){
            pair<f::path,int>task;
            {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock,[&](){
                 return stop||!tasks.empty();
                });
                if(stop&&tasks.empty()){
                 return ;
                }
                task=tasks.front();
                tasks.pop();
                working++;
            }
            f::path path=task.first;
            int depth=task.second;

            if(depth<=obj.max_depth) {
         try{
                for(auto &file:f::directory_iterator(path)){
                   f::path p=file.path();
                   if(isskip(p)) continue;
                   //判断是不是目录
                   if(file.is_directory()){
                    {
                    lock_guard<mutex> lock(mtx);
                    tasks.push({p,depth+1});
                    }
                    cv.notify_one();
                   }
                   else if(file.is_regular_file()){
                    if(p.extension()== obj.file_type){
                        lock_guard<mutex> lock(mt);
                      result.insert(f::absolute(p).string());
                    }
                   }
                }
            }
            catch(...){}
        }
    
         {
            lock_guard<mutex> lock(mtx);
            working--;   

            if(tasks.empty() && working == 0){
                stop = true;
                cv.notify_all();
            }
        }
    }
}

    void run(){
        tasks.push({obj.root_path,0});

        for(int i=0;i<obj.max_concurrency;i++){
            threads.emplace_back(&FileSearch::worker,this);
        }

        for(auto &t: threads){
            t.join();
        }
    }

    void print(){
    for(auto &s:result){
        cout<<s<<endl;
       }
    }
};

s parseArgs(int argc, char* argv[]) {
    s config;

    // 直接写默认值
    config.root_path = ".";
    config.file_type = ".cpp";
    config.max_concurrency = 4;
    config.max_depth = 5;
    config.skip_hidden = true;
    config.skip_paths = {".git", "build"};

    // 按顺序一个个判断
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

int main(int argc, char* argv[]) {
    SearchConfig config = parseArgs(argc, argv);

    FileSearch fs(config);
    fs.run();
    fs.print();

    return 0;
}
   