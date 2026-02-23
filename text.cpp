/*#include <iostream>
using namespace std;
namespace A{
    int x=10;
    void func1();
}
void A::func1(){
    cout<<"hello"<<' ';
}
namespace B{
    int x=20;
    void func2();
}
void B::func2(){
    cout<<"winter vocation"<<endl;
}
int main(){
    cout<<A::x<<' '<<B::x<<endl;
    A::func1();
    B::func2();
    return 0;
}*/
/*
#include <iostream>
#include <string>
 using namespace std;
struct student{
    string name;
    int age;
    int score[3];
    void setname(string name1){
        name=name1;
    }
    void setage(int age1){
        age=age1;
    }
    void output(){
            cout<<"name: "<<name<<endl;
            cout<<"age: "<<age<<endl;
        }
    void setscore(int score1[3]){
        for(int i=0;i<3;i++){
            score[i]=score1[i];
        }
    }
    void outputscore(){
        cout<<"chushiscore: ";
        for(int i=0;i<3;i++){
            cout<<score[i]<<' ';
        }
        cout<<endl;
    }
        void paixu(){
            for(int i=0;i<2;i++){
                for(int j=0;j<2-i;j++){
                    if(score[j]>score[j+1]){
                        int temp=score[j];
                        score[j]=score[j+1];
                        score[j+1]=temp;
                    }
                }
            }
            for(int i=0;i<3;i++){
                cout<<score[i]<<' ';
            }
        }
};
int main(){
    student st;
    st.setname("zhangsan");
    st.setage(18);
    st.output();
    int arr[3]={90,80,70};
    st.setscore(arr);
    st.outputscore();
    st.paixu();
    return 0;
}*/
/*#include <iostream>
using namespace std;
int main(){
    string s1("hello");
    string s2=" world";
    string s3("hello world");
     if(s1==s2){
        cout<<"no"<<endl;
     }
     if(s3==s1+s2){
        cout<<"yes"<<endl;
     }
     return 0;
}*/
/*
#include <iostream>
#include <utility>
using namespace std;
int main(){
   int a,b;
   cin>>a>>b;
   int x=add(a,b);
   double c,d;
   cin>>c>>d;
   double y=add(c,d);
}
int Intadd(int a,int b){
    return a+b;
}
double Doubleadd(double a,double b){
    return a+b;
}
int add(int a,int b){
    return a+b;
}
double add(double a,double b){
    return a+b;
}*/
/*#include <iostream>
using namespace std;
struct student{
    string name;
    int age;
    int score[3];
};
ostream& operator<<(ostream &os,const student &st){
    os<<st.name<<endl;
    os<<st.age<<endl;
    return os;
}
istream& operator>>(istream &is,student &st){
    is>>st.name;
    is>>st.age;
    return is;
}       
int main(){
    student st;
    cin>>st;
   cout<<st<<endl;
   return 0;
}*/
/*
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
//设计比较函数
bool cmp(int x,int y){
    return x>y;
}
//设计仿函数
struct CMP{
    bool operator()(int x,int y){
        return x>y;
    }
}cmp1;
struct Student{
    string name;
    int age;
};
//结构体排序函数
bool cmp_name(Student s1,Student s2){
    return s1.name<s2.name;
};
int main(){
    int arr[5]={3,1,4,5,2};
    sort(arr,arr+5);
    for(int i=0;i<5;i++){
        cout<<arr[i]<<' ';
    }
    sort(arr,arr+5,cmp);
    cout<<endl;
    for(int i=0;i<5;i++){
        cout<<arr[i]<<' ';
    }
    cout<<endl;
    string s("treltrhujhbxbghfdshauihiubvhd");
    sort(s.begin(),s.end(),cmp1);
    cout<<s<<endl;
    Student arrs[3]={{"zhangsan",18},{"lisi",17},{"wangwu",19}};
    sort(arrs,arrs+3,cmp_name);
    for(int i=0;i<3;i++){
        cout<<arrs[i].name<<' '<<arrs[i].age<<endl;
    }
    return 0;
}*/
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

using namespace std;

/* ---------------- 数据结构 ---------------- */

struct Command {
    vector<string> argv;
    string input;      // <
    string output;     // > or >>
    bool append = false;
};

struct Pipeline {
    vector<Command> cmds;
    bool background = false;
};

/* ---------------- 全局变量 ---------------- */

string last_dir;

/* ---------------- 工具函数 ---------------- */

vector<string> tokenize(const string& line) {
    vector<string> tokens;
    string cur;
    for (size_t i = 0; i < line.size(); i++) {
        if (isspace(line[i])) {
            if (!cur.empty()) {
                tokens.push_back(cur);
                cur.clear();
            }
        } else if (line[i] == '>' || line[i] == '<' ||
                   line[i] == '|' || line[i] == '&') {
            if (!cur.empty()) {
                tokens.push_back(cur);
                cur.clear();
            }
            if (line[i] == '>' && i + 1 < line.size() && line[i + 1] == '>') {
                tokens.push_back(">>");
                i++;
            } else {
                tokens.push_back(string(1, line[i]));
            }
        } else {
            cur.push_back(line[i]);
        }
    }
    if (!cur.empty()) tokens.push_back(cur);
    return tokens;
}

/* ---------------- 解析 ---------------- */

Pipeline parse_pipeline(const vector<string>& tokens) {
    Pipeline p;
    Command cur;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "|") {
            p.cmds.push_back(cur);
            cur = Command();
        } else if (tokens[i] == "<") {
            cur.input = tokens[++i];
        } else if (tokens[i] == ">") {
            cur.output = tokens[++i];
            cur.append = false;
        } else if (tokens[i] == ">>") {
            cur.output = tokens[++i];
            cur.append = true;
        } else if (tokens[i] == "&") {
            p.background = true;
        } else {
            cur.argv.push_back(tokens[i]);
        }
    }
    p.cmds.push_back(cur);
    return p;
}

/* ---------------- 内建命令 cd ---------------- */

bool builtin_cd(const vector<string>& argv) {
    if (argv.empty() || argv[0] != "cd") return false;

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    string target;
    if (argv.size() == 1) {
        target = getenv("HOME");
    } else if (argv[1] == "-") {
        target = last_dir;
        cout << target << endl;
    } else {
        target = argv[1];
    }

    if (chdir(target.c_str()) == 0) {
        last_dir = cwd;
    } else {
        perror("cd");
    }
    return true;
}

/* ---------------- 执行 pipeline ---------------- */

void execute_pipeline(Pipeline& p) {
    int n = p.cmds.size();
    int prev_fd = -1;
    vector<pid_t> pids;

    for (int i = 0; i < n; i++) {
        int pipefd[2];
        if (i < n - 1) {
            if (pipe(pipefd) < 0) {
                perror("pipe");
                return;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return;
        }

        if (pid == 0) {
            // 子进程
            signal(SIGINT, SIG_IGN);

            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < n - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            Command& c = p.cmds[i];

            if (!c.input.empty()) {
                int fd = open(c.input.c_str(), O_RDONLY);
                if (fd < 0) {
                    perror("open input");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if (!c.output.empty()) {
                int fd;
                if (c.append)
                    fd = open(c.output.c_str(),
                              O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(c.output.c_str(),
                              O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open output");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            vector<char*> argv;
            for (auto& s : c.argv)
                argv.push_back(const_cast<char*>(s.c_str()));
            argv.push_back(nullptr);

            execvp(argv[0], argv.data());
            perror("execvp");
            exit(1);
        }

        // 父进程
        if (prev_fd != -1) close(prev_fd);
        if (i < n - 1) {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
        pids.push_back(pid);
    }

    if (!p.background) {
        for (pid_t pid : pids)
            waitpid(pid, nullptr, 0);
    }
}

/* ---------------- 主循环 ---------------- */

int main() {
    signal(SIGINT, SIG_IGN);

    while (true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        cout << "\033[1;32mgpt@gpt-super-shell\033[0m:"
             << "\033[1;34m" << cwd << "\033[0m $ ";

        string line;
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        if (line == "exit") break;

        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        if (builtin_cd(tokens)) continue;

        Pipeline p = parse_pipeline(tokens);
        execute_pipeline(p);
    }

    return 0;
}