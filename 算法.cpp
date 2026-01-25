//模拟算法（P1067 [NOIP 2009 普及组] 多项式输出）
#include <iostream>
#include <cstdlib>
using namespace std;
int main(){
    int n;
    cin>>n;
    for(int i=n;i>=0;i--){
        int a;
        cin>>a;
        //符号
        if(a==0) continue;
        if(a<0)cout<<'-';
        if(a>0){
            if(i==n){
                continue;
            }else{
                cout<<'+';
            }
        }
        //数字
        a=abs(a);
        if(a==0){
            continue;
        }else if(a==1&&i!=0){
            continue;
        }
        else{
            cout<<a;
        }
        //输出x
        if(a==0){
            continue;
        }
         if(i==0){
            continue;
        }
        if(i==1){
            cout<<'x';
        }
        else{
            cout<<'x'<<'^';
        }
        //次数
        if(a==0){
             continue;
        }
         if(i==0||i==1){
          continue;
        }else{
            cout<<i;
        }
    }
}