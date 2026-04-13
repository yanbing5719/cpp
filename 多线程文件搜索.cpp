/*struct SearchConfig {
    std::string root_path;    // 要搜索的根目录
    std::string file_type;    // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency;      // 最大并发数
    int max_depth;            // 最大搜索深度
    bool skip_hidden;         // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths;   // 要跳过的目录或文件的路径
};*/
#include <bits/stdc++.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <set>

using namespace std;
namespace fs = std::filesystem; //别名

struct SearchConfig {
    string root_path;    
    string file_type;    
    int max_concurrency; 
    int max_depth;       
    bool skip_hidden;    
    vector<string> skip_paths;   
};

class FileSearcher {
private:
    SearchConfig config;

    queue<pair<fs::path, int>> tasks; // (路径, 深度)
    mutex mtx;//锁任务队列
    condition_variable cv;

    vector<thread> workers;

    bool stop = false;

    set<string> result;
    mutex res_mtx;//锁结果

public:
    FileSearcher(const SearchConfig& cfg) : config(cfg) {}

    bool shouldSkip(const fs::path& p) {
        string name = p.filename().string();//获取，文件名，并转化成string类型

        if (config.skip_hidden && !name.empty() && name[0] == '.')
            return true;

        string full = p.string();//获得完整路径名
        for (auto& s : config.skip_paths) {
            if (full.find(s) != string::npos)
                return true;
        }
        return false;
    }

    void worker() {
        while (true) {
            pair<fs::path, int> task;

            {
                unique_lock<mutex> lock(mtx);

                cv.wait(lock, [&]() {
                    return stop || !tasks.empty();
                });

                if (stop && tasks.empty())
                    return;

                task = tasks.front();
                tasks.pop();
            }

            fs::path path = task.first;
            int depth = task.second;

            if (depth > config.max_depth)
                continue;

            try {
                for (auto& entry : fs::directory_iterator(path)) {
                    fs::path p = entry.path();

                    if (shouldSkip(p)) continue;

                    if (entry.is_directory()) {
                        {
                            lock_guard<mutex> lock(mtx);
                            tasks.push({p, depth + 1});
                        }
                        cv.notify_one();
                    }
                    else if (entry.is_regular_file()) {
                        if (p.extension() == config.file_type) {
                            lock_guard<mutex> lock(res_mtx);
                            result.insert(p.string());
                        }
                    }
                }
            } catch (...) {
                // 忽略权限问题，捕获所有异常类型
            }
        }
    }

    void run() {
        // 初始任务
        tasks.push({config.root_path, 0});

        // 创建线程
        for (int i = 0; i < config.max_concurrency; i++) {
            workers.emplace_back(&FileSearcher::worker, this);
        }

        // 等待任务处理完（简单做法）
        while (true) {
            {
                unique_lock<mutex> lock(mtx);
                if (tasks.empty())
                    break;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        // 通知线程退出
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();

        for (auto& t : workers)
            t.join();
    }

    void printResult() {
        for (auto& s : result) {
            cout << s << endl;
        }
    }
};

int main() {
    SearchConfig config;
    config.root_path = ".";        // 当前目录
    config.file_type = ".cpp";     
    config.max_concurrency = 4;    
    config.max_depth = 5;          
    config.skip_hidden = true;
    config.skip_paths = {".git", "build"};

    FileSearcher fs(config);
    fs.run();
    fs.printResult();

    return 0;
}