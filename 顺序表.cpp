#include <iostream>
#include <vector>
using namespace std;
const int N=100000; 
//静态封装顺序表
class SqList
{
   int a[N];
   int n;//元素个数
   public: 
   SqList(){
    n=0;
   }
   //顺序表的尾插法
   void push_back(int x){
    a[++n]=x;
   }
   //顺序表的头插法
   void push_front(int x){
      for(int i=n;i>=0;i--){
        a[i+1]=a[i];
      }
      a[1]=x;
      n++;
   }
   //在任意位置插入元素
   void insert(int p,int x){
    for(int i=n;i>=p;i--){
        a[i+1]=a[i];
    }
    a[p]=x;
    n++;
   }
   //删除表尾的元素
   void pop_back(){
    n--;
   }
   //删除表头的元素
   void pop_front(){
    for(int i=2;i<=n;i++){
        a[i-1]=a[i];
    }
    n--;
   }
   //删除任意位置的元素
   void erase(int p){
    for(int i=p+1;i<=n;i++){
        a[i-1]=a[i];
    }
    n--;
   }
   //查找元素-按值查找
   int find(int x){
    for(int i=0;i<n;i++){
        if(a[i]==x){
            return i;
        }
    }
    return -1;
   }
   //查找元素-按下标查找
   int get(int p){
    return a[p];
   }
   //打印顺序表
   void print( ){
    for(int i=0;i<=n;i++){
        cout<<a[i]<<' ';
    }
    cout<<endl<<endl;
   }
};
void print(vector<int>&a)
{
    for(int i=0;i<a.size();i++){
        cout<<a[i]<<' ';
    }
    cout<<endl;
}

#include <iostream>
#include <vector>
using namespace std;
const int N=100000;
 int main(){
    vector<int> a1;
    vector<int> a2(N);//创建一个大小为N的动态数组
    vector<int> a3(N,2);//初始化为2
    vector<int> a4{1,2,3,4,5,6};//初始化
 }





