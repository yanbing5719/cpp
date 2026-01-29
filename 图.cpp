/*#include <bits/stdc++.h>
using namespace std;
//邻接表图
int main(){
    int n;
    cin>>n;
    vector<vector<int>> graph(n);
    int u,v;
    u=0,v=1;
    graph[u].push_back(v);
     graph[v].push_back(u);
     u=0,v=2;
     graph[u].push_back(v);
     graph[v].push_back(u);
     u=1,v=3;
     graph[u].push_back(v);
     graph[v].push_back(u);
    u=2,v=3;
     graph[u].push_back(v);
     graph[v].push_back(u);
     for(int i=0;i<n;i++){
        cout<<i<<':';
        for(int x:graph[i]){
            cout<<x<<' ';
        }
        cout<<endl;
     }
}

*/
//DFS+邻接表
#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> graph;
vector<bool> judge;
void creat(int u,int v){
    graph[u].push_back(v);
    graph[v].push_back(u);
}
void dfs(int u){
    judge[u]=true;
    cout<<"visit:"<<u<<endl;
    for(int v:graph[u]){
         if(!judge[v]){
            dfs(v);
         }
    }
}
int main(){
    int n=4;
    graph.resize(n);
    judge.resize(n,false);
    creat(0,1);
    creat(0,2);
    creat(1,3);
    creat(2,3);
    dfs(0);
  return 0;
}