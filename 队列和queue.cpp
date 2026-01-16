#include <iostream> 
using namespace std;
const int N=1000;
int q[N];
int h;//头结点的位置
int t;//尾结点的下一个位置
//入队
void push(int x){
    q[t++]=x;
}
//出队
void pop(){
    h++;
}
//查询队头元素
int front(){        
    return q[h];
}
//查询队尾元素
int back(){
    return q[t-1];
}
//判断队列是否为空
bool empty(){
    return h==t;
}
//查询有效元素的个数
int size(){
    return t-h;
}
int main(){
    for(int i=1;i<=10;i++){
        push(i);
    }
    while(!empty()){
        cout<<front()<<back()<<endl;
        pop();
    }
    return 0;
}

//stl封装队列
#include <iostream>
#include <queue>
using namespace std;
typedef pair<int,int> PII;
int main()
{
    queue<PII> q;
    for(int i=1;i<=10;i++){
        q.push({i,i*10});
    }
    while(!q.empty()){
        cout<<q.front().first<<' '<<q.front().second<<endl;
        q.pop();
    }
    return 0;            
  }