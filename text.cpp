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
/*
#include <bits/stdc++.h>
using namespace std;
long long cnt=0;
int main()
{
    int k;
    long long n; 
    cin>>n>>k;
    vector<pair<int,int>> num(n);
    for(int i=0;i<n;i++){
    cin>>num[i].first>>num[i].second;
    }
    int l=1,r=1e5,mid;
    while(l<=r)
    {
        cnt=0;
    mid=l+(r-l)/2;
    for(int i=0;i<n;i++)
    {
        int n1=num[i].first/mid;
        int n2=num[i].second/mid;
        cnt+=(n1*n2);
        if(cnt>=k)break;
    }
    
    if(cnt>=k)l=mid+1;
    else r=mid-1;
    }
    cout<<r;
    return 0;
}*/
/*
#include <iostream>
#include <unordered_map>
using namespace std;
int main()
{
    unordered_map<int,int> mp;
    mp[3]=5;
    mp.count(3);
    mp[3];
    mp.erase(3);
    return 0;
}
*/
/*
#include <bits/stdc++.h>
using namespace std;
int main(){
    int m;
    long long n;
    vector<int>num(n+1,0);
    for(int i=1;i<=n;i++){
        cin>>num[i];
    }
    vector<int>a(n+1,0);
    for(int i=1;i<=n;i++){
        a[i]+=num[i]+a[i-1];
    }
    cin>>n>>m;
    int l,r;
    while(m--){
    cin>>l>>r;
    int result=a[r]-a[l-1];
    cout<<result<<endl;
    }
}*/
/*
#include <iostream>
using namespace std;
class Person{
    
    public:
    int age;
    Person(){
        cout<<"no parameters function"<<endl;
    }
    Person(int a){
    age=a;
    cout<<"have parameters function"<<endl;
    }
    Person(Person &p1){
    age=p1.age;
    cout<<"copy extablish"<<endl;
    }

    ~Person(){
        cout<<"xi gou function"<<endl;
    }
};
int main(){
    Person p1;
    Person p2(10);
    Person p3(p2);
    cout<<p2.age<<" "<<p3.age<<endl;
    return 0;
}*/
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    // 1. 创建 socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定地址
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);          // 端口
    addr.sin_addr.s_addr = INADDR_ANY;    // 监听所有IP

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));

    // 3. 监听
    listen(server_fd, 5);

    cout << "服务器启动，等待连接..." << endl;

    // 4. 接受连接
    int client_fd = accept(server_fd, nullptr, nullptr);

    cout << "客户端已连接" << endl;

    // 5. 接收数据
    char buf[1024] = {0};
    int n = recv(client_fd, buf, sizeof(buf), 0);

    cout << "收到数据: " << buf << endl;
    cout << "字节数: " << n << endl;

    // 6. 关闭
    close(client_fd);
    close(server_fd);

    return 0;
}