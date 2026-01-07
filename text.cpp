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