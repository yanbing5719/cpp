/*#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;
int main(){
    pid_t pid=fork();
    if(pid==0){
        char* argv[]={
            (char*) "ls",
            (char*) "-l",
            nullptr
        };
        extern char **environ;
        execve("/bin/ls",argv,environ);
    }else{
        wait(nullptr);
        cout<<"child finished"<<endl;
    }
return 0;
}*/
/*#include <iostream>
#include <unistd.h>
using namespace std;
int main(){
     char* argv[]={
            (char*) "ls",
            (char*) "-l",
            nullptr
        };
    execvp("ls",argv);
    return 0;
}*/
/*
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>  
using namespace std;
int main(){
    pid_t pid=fork();
    if(pid==0){
        char *argv[]={
            (char*) "ls",
            (char*) "-a",
            nullptr};
    execvp("ls",argv);
    perror("execvp failed");
    }else if(pid>0){
        wait(nullptr);
        cout<<"child finished";
    }else{
        perror("fork failed");
    }
    return 0;
}*/
/*
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;
int main(){
    int fd[2];
    if(pipe(fd)==-1){
        perror("pipe");
        return 1;
    }
    pid_t pid1=fork();
    if(pid1==-1){
        perror("fork");
        return 1;
    }
    if(pid1==0){
    dup2(fd[1],1);
    close(fd[0]);
    close(fd[1]);
   execlp("ls","ls",nullptr);
   perror("exec,ls");
   _exit(1);
    }
    pid_t pid2=fork();
    if(pid2==-1){
        perror("fork");
        return 1;
    }
    if(pid2==0){
        dup2(fd[0],0);
        close(fd[0]);
        close(fd[1]);
        execlp("wc","wc","-l",nullptr);
        perror("exec wc");
        _exit(1);
        close(fd[0]);
        close(fd[1]);
        waitpid(pid1,nullptr,0);
        waitpid(pid2,nullptr,0);
        return 0;
    }
}*/

/*
//路径之谜
#include <bits/stdc++.h>
using namespace std;
int n;
vector<int>path;
//记录每一个方格是否走过
bool vis[25][25];
//是否找到正确答案
bool finish=false;
//坐标 上下左右
int dx[4]={1,-1,0,0};
int dy[4]={0,0,-1,1};
//每一个靶子的箭数
int north[25];
int west[25];
void dfs(int x,int y)
{
  if(finish)return ;
  north[y]--;
   west[x]--;
  if(north[y]<0||west[x]<0)
  {
    north[y]++;
    west[x]++;
    return ;
  }
  vis[x][y]=true;
  path.push_back(x*n+y);
  //检查每一个靶子上面箭的数量是不是归零
  bool ok=true;
  for(int i=0;i<n;i++)
  {
    if(north[i]!=0)ok=false;
  }
  for(int i=0;i<n;i++)
  {
    if(west[i]!=0) ok=false;
  }
  //判断是否走到右下角
  if(x==n-1&&y==n-1&&ok){
    finish=true;
    return;
  }
//更新新的坐标
int nx,ny;
for(int i=0;i<4;i++)
{
  nx=x+dx[i];
  ny=y+dy[i];
  //检查根新后的坐标是否符合题目
  if(nx>=0&&nx<n&&ny>=0&&ny<n&&!vis[nx][ny])
  {
    dfs(nx,ny);
    if(finish)return ;
  }
}
  //如果循环结束后还是没有找到结果,退回到上一步
  path.pop_back();
  vis[x][y]=false;
  north[y]++;
  west[x]++;

}
int main()
{
  cin>>n;
  for(int i=0;i<n;i++)
  {
    cin>>north[i];
  }
  for(int i=0;i<n;i++)
  {
    cin>>west[i];
  }
  dfs(0,0);
  for(int i=0;i<path.size();i++)
  {
    cout<<path[i]<<" ";
  }
  return 0;
}*/

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
int main()
{
    string s1,s2;
    cin>>s1>>s2;
    if(s1.size()!=s2.size())
    {
        cout<<"No";
        return 0;
    }
    sort(s1.begin(),s1.end());
    sort(s2.begin(),s2.end());
    for(int i=0;i<s1.size();i++)
    {
        if(s1[i]!=s2[i]){
            cout<<"No";
            return 0;
        }
    }
    cout<<"Yes";
    return 0;
}