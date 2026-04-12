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

/*
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
*/
/*
#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n,k;
    cin>>n>>k;
    vector<int> a(n+1,0);
    for(int i=1;i<=n;i++)
    {
    cin>>a[i];
    }
    vector<long long>f(n+1,0);
    for(int i=1;i<=n;i++)
    {
        f[i]=a[i]+f[i-1];
    }
   long long cnt=0;
     vector<long long>p(k,0);
     p[0]=1;
   for(int i=1;i<=n;i++)
   {
   int mod=f[i]%k;
   cnt+=p[mod];
   p[mod]++;
   }
    cout<<cnt<<endl;
    return 0;
}
*/
/*
#include <bits/stdc++.h>
using namespace std;
int dx[8]={1,1,-1,-1,2,2,-2,-2};
int dy[8]={2,-2,2,-2,1,-1,1,-1};
int main(){
queue<pair<int,int>> cur;
int dis[410][410];
int n,m,x,y;
cin>>n>>m>>x>>y;
for(int i=1;i<=n;i++){
    for(int j=1;j<=m;j++){
        dis[i][j]=-1;
    }
}
dis[x][y]=0;
cur.push({x,y});
while(!cur.empty()){
    auto[cx,cy]=cur.front();
    cur.pop();
    for(int i=0;i<8;i++){
        int nx=cx+dx[i];
        int ny=cy+dy[i];
    
    if(nx>=1&&nx<=n&&ny>=1&&ny<=m&&dis[nx][ny]==-1){
        dis[nx][ny]=dis[cx][cy]+1;
        cur.push({nx,ny});
    }
}
}
for(int i=1;i<=n;i++){
    for(int j=1;j<=m;j++){
        cout<<left<<setw(3)<<dis[i][j];
    }
    cout<<endl;
}
return 0;
}*/
/*
#include <bits/stdc++.h>
using namespace std;
int main(){
    int r;
    cin>>r;
    vector<vector<int>> f(r+1);
    for(int i=0;i<=r;i++){
        f[i].resize(i+1,0);
    }
    for(int i=1;i<=r;i++){
        for(int j=1;j<=i;j++){
            cin>>f[i][j];
        }
    }
    for(int i=1;i<=r;i++){
        for(int j=1;j<=i;j++){
            if(i==1)continue;
            else if(i==j)f[i][j]=f[i-1][j-1]+f[i][j];
            else if(j==1){
                f[i][j]=f[i-1][j]+f[i][j];
            }else
            f[i][j]=max(f[i-1][j]+f[i][j],f[i-1][j-1]+f[i][j]);
        }
    }
    int max1=0;
    for(int i=1;i<=r;i++){
    max1=max(f[r][i],max1);
    }
    cout<<max1<<endl;
}
*/
/*
#include <bits/stdc++.h>
using namespace std;
int main()
{
  int n,m;
  cin>>n>>m;
  const int N=100000;
  int a[N];
  for(int i=0;i<n;i++){
    int x;
    cin>>x;
    a[i]=x;
  }
  sort(a,a+n);
  int l=1e5+10;
  for(int i=0;i+m-1<n;i++){
   l=min(l,a[i+m-1]*a[i+m-1]-a[i]*a[i]);
  }
  cout<<l;
  return 0;
}
  */
 /*
 #include <bits/stdc++.h>
using namespace std;
int main(){
int last=-1,state=-1;
int ans=0;
    string a,b;
    cin>>a>>b;
    int n=a.size();
    for(int i=0;i<n;i++){
    if(a[i]=='.'&&b[i]=='.'){
        continue;
    }
    if(last!=-1){
     ans+=i-last-1;
    }else if(a[i]=='#'&&b[i]=='#'){
    state=1;
    }
     else if(a[i]=='#'&&b[i]=='.'){
    if(state==3){
     ans++;
     state=3; 
    }else{
        state=2;
    }
}else if(a[i]=='.'&&b[i]=='#'){
         if(state==2){
           ans++;
           state=1;
       }else{
           state=3;
       }
    }
    last=i;
}
    cout<<ans<<endl;
    return 0;
}
    */
   /*
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
 const ll m=1e9+7;
 const ll N=1e5+10;
ll kmp(ll a,ll b,ll m){
    ll res=1;
    while(b>0){
       if(b&1){
        res=(res*a)%m;
       }
        a=(a*a)%m;
       b>>=1;
    }
    return res;
}

int main(){
    int s=0;
    int n;
    int ans=0;
    int a[N];
    cin>>n;
    for(int i=1;i<=n;i++){
        int x;
        cin>>x;
        a[i]=x;
    }
    for(int i=1;i<=n;i++){
        s^=a[i];
        if(n>i){
           ans+=s*2*kmp(3,n-i-1,m);
    }else{
        ans+=s;
    }
    ans%=m;
}
    cout<<ans<<endl;
    return 0;
}*/
/*
#include <bits/stdc++.h>
using namespace std;
const int N=5e6+10;
int main(){
int n,p;
    cin>>n>>p;
    int a[N];
    for(int i=1;i<=n;i++){
    cin>>a[i];
    }
    int f[N]={0};
    for(int i=1;i<=n;i++){
    f[i]=a[i]-a[i-1];
    }
    int res=102;
    while(p--){
        int x,y,z;
        cin>>x>>y>>z;
        f[x]+=z;
        f[y+1]-=z;
    }
    a[0]=0;
     for(int i=1;i<=n;i++){
        a[i]=a[i-1]+f[i];
        res=min(res,a[i]);
    }
    cout<<res<<endl;
    return 0;
}*/
#include <bits/stdc++.h>
using namespace std;
int dx[4]={1,-1,0,0};
int dy[4]={0,0,-1,1};
 void dfs(int x,int y){
     queue<pair<int,int>> q;
     
 }