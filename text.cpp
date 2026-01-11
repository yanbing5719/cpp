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
}