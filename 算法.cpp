//模拟算法（P1067 [NOIP 2009 普及组] 多项式输出）
/*#include <iostream>
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
}*/
/*P5731 【深基5.习6】蛇形方阵
#include <iostream>
using namespace std;
//方向向量 右 下 左 上
int dx[]={0,1,0,-1};
int dy[]={1,0,-1,0};
const int N=10;
int arr[N][N];
int main(){
int n;
cin>>n;
  int count=1;
  int x=1,y=1;
  int pos=0;
  while(count<=(n*n)){
    arr[x][y]=count;
    int a=dx[pos]+x,b=dy[pos]+y;
    if(a<1||a>n||b<1||b>n||arr[a][b]!=0){
        pos=(pos+1)%4;
        a=dx[pos]+x,b=dy[pos]+y;
    }
    x=a,y=b;
    count++;
  }
  for(int i=1;i<=n;i++){
    for(int j=1;j<=n;j++){
        printf("%3d",arr[i][j]);
    }
    cout<<'\n';
  }
}*/
//P1601 高精度加法
/*#include <iostream>
using namespace std;
const int N=1e3;
int a[N],b[N],c[N];
int la,lb,lc;
void add(int a[],int b[],int c[]){
   for(int i=0;i<lc;i++){
    c[i]+=a[i]+b[i];
    c[i+1]+=c[i]/10;
    c[i]%=10;
   }
   if(c[lc]!=0)lc++;
}
int main(){
    string x,y;
    cin>>x>>y;
    la=x.size();
    lb=y.size();
    for(int i=0;i<la;i++){
    a[la-1-i]=x[i]-'0';
    }
    for(int i=0;i<lb;i++){
    b[lb-1-i]=y[i]-'0';
    }
    lc=max(la,lb);
    add(a,b,c);
    for(int i=lc-1;i>=0;i--){
        cout<<c[i];
    }
    return 0;
}*/
/* 高精度减法
#include <bits/stdc++.h>
using namespace std;
const int N=1e5;
int a[N],b[N],c[N];
int la,lb,lc;
bool cmp(string&x,string&y){
    if(x.size()!=y.size()){
    return (x.size()>y.size());
    }
    return (x>y);
}
void sub(int a[],int b[],int c[]){
    for(int i=0;i<lc;i++){
    c[i]+=a[i]-b[i];
    if(c[i]<0){
        c[i+1]-=1;
        c[i]+=10;
    }
    }
   while(lc>1&&c[lc-1]==0)lc--;
}
int main(){
string x,y;
cin>>x>>y;
if(!cmp(x,y)&&(x!=y)){
    swap(x,y);
    cout<<'-';
}
la=x.size(),lb=y.size();
lc=max(la,lb);
for(int i=0;i<la;i++){
    a[la-1-i]=x[i]-'0';
}
for(int i=0;i<lb;i++){
    b[lb-1-i]=y[i]-'0';
}
sub(a,b,c);
for(int i=lc-1;i>=0;i--){
    cout<<c[i];
}
return 0;
}*/
//高精度乘法
/*#include <bits/stdc++.h>
using namespace std;
const int N=1e4;
int a[N],b[N],c[N];
int la,lb,lc;
void mul(int a[],int b[],int c[]){
    lc=la+lb;
    for(int i=0;i<la;i++){
        for(int j=0;j<lb;j++){
       c[j+i]=a[i]*b[j];
        }
    }
    for(int i=0;i<lc;i++){
            c[i+1]+=c[i]/10;
            c[i]%=10;
    }
    while(lc>1&&c[lc-1]==0)lc--;
}
int main(){
    string x,y;
    cin>>x>>y;
    la=x.size();
    lb=y.size();
    lc=la+lb;
    for(int i=0;i<la;i++){
        a[la-1-i]=x[i]-'0';
    }
    for(int i=0;i<lb;i++){
        b[lb-1-i]=y[i]-'0';
    }
    mul(a,b,c);
    for(int i=lc-1;i>=0;i--){
        cout<<c[i];
    }
    return 0;
}*/
//高精度除法
#include <bits/stdc++.h>
using namespace std;
const int N=1e4;
int a[N],b,c[N];
int la,lc;
void div(int a[],int b,int c[]){
    long long t=0;
    for(int i=la-1;i>=0;i--){
       t=t*10+a[i];
        c[i]=t/b;
        t%=b;
    }
    while(lc>1&&c[lc-1]==0)lc--;
}
int main(){
    string x;
    cin>>x>>b;
    la=x.size();
    for(int i=0;i<la;i++){
        a[la-1-i]=x[i]-'0';
    }
    lc=la;
    div(a,b,c);
    for(int i=lc-1;i>=0;i--){
        cout<<c[i];
    }
    return 0;
}