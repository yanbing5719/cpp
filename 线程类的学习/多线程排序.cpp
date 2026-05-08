#include <bits/stdc++.h>
#include <thread>
#include <mutex>
using namespace std;

//生成随机数
vector<int> rannum(int n){
    vector<int>a(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1,100000);
    for(int i=0;i<n;i++){
        a[i]=dis(gen);
    }
    return a;
}

queue <vector<int>> q;//线程队列
mutex mtx;//上锁

//排序
void sort1(vector<int> thread1){
 sort(thread1.begin(),thread1.end());
 lock_guard lock(mtx);
 q.push(thread1);
}
 
//合并两个数组
vector<int> merge(vector<int> &a,vector<int> &b){
    vector<int>res;
    int l1=a.size();
    int l2=b.size();
    int i=0,j=0;
    while(i<l1&&j<l2){
        if(a[i]>b[j]){
            res.push_back(b[j++]);
        }else{
            res.push_back(a[i++]);
        }
    }
    while(i<l1)res.push_back(a[i++]);
    while(j<l2)res.push_back(b[j++]);
    return res;
}



int main(){
    int n=100000;
    vector<int> a=rannum(n);
    int thread_num=thread::hardware_concurrency();
    if(thread_num==0)thread_num=4;
    //一个线程要处理的数据
    int data=n/thread_num;

    //创建线程数组，在每个线程里面进行排序
    vector<thread> threads;
    for(int i=0;i<thread_num;i++){
        int l=i*data,r=l+data;
        vector<int> thread1(a.begin()+l,a.begin()+r);
        threads.emplace_back(sort1,thread1);
    }

    //wait all thread finished
    for(auto&t:threads) t.join();
    
    //take out datas from q;
    vector<vector<int>> q_data;
    while(!q.empty()){
        q_data.push_back(q.front());
        q.pop();
    }
    
    //将所有线程里面的数据排序后储存到一个二维数组里面
    while(q_data.size()>1){
          vector<vector<int>> res;
    for(int i=0;i<q_data.size();i+=2){
        if((i+1)>q_data.size()){
            res.push_back(q_data[i]);
        }else{
                res.push_back(merge(q_data[i],q_data[i+1]));
        }
  }
  q_data=res;
    }
    
    //output
 for(int i=0;i<10;i++){
    cout<<q_data[1][i]<<' ';
 }
 cout<<endl;
    return 0;
}