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
        if(a==0) continue;
    //符号
    if(a<0){
        cout<<'-';
    }else{
        if(i!=n){
            cout<<'+';
        }
    }
    //数字
    a=abs(a);
    if((a!=1)||(a==1&&i==0)){
        cout<<a;
    }
    //次方
    if(i!=0){
        if(i!=1){
            cout<<'x'<<'^'<<i;
        }else{
            cout<<'x';
        }
    }else{
        continue;
    }
    }
}