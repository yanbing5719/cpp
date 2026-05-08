#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;
//插入排序
void insert_sort(int arr[],int n){
    for(int i=1;i<n;i++){
        int key=arr[i];
        int j=i-1;
        while(j>=1&&arr[j]>key){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;
    }
}
//选择排序
void select_sort(int arr[],int n){
    for(int i=0;i<n-1;i++){
        int pos=i;
        for(int j=i+1;j<n;j++){
         if(arr[j]<arr[pos]){
            pos=j;
         }
        }
        swap(arr[i],arr[pos]);
    }
}
//冒泡排序
void bubble_sort(int arr[],int n){
    for(int i=0;i<n-1;i++){
for(int j=1;j<n-1-i;j++){
    if(arr[j]>arr[j+1]){
        swap(arr[j],arr[j+1]);                  
}
}
}
}
//快速排序
void quick_sort(vector<int> &arr,int l,int r){
      if(l>=r) return ;
      int i=l,j=r;
      int point=arr[l];
      while(i<j){
        while((i<j)&&(arr[j]>=point)){
       j--;
        }
        arr[i]=arr[j];
      while((i<j)&&(arr[i]<=point)){
        i++;
      }
      arr[j]=arr[i];
      }
      arr[i]=point;
     quick_sort(arr,l,i-1);
     quick_sort(arr,i+1,r);
}
    
//归并排序
void merge(vector<int>&arr,int l,int m,int r){
     vector<int>t;
     int i=l,j=m+1;
     while(i<=m&&j<=r){
        if(arr[i]<=arr[j]){
            t.push_back(arr[i++]);
        }else{
            t.push_back(arr[j++]);
        }
     }
     while(i<=m){
        t.push_back(arr[i++]);
     }
     while(j<=r){
        t.push_back(arr[j++]);
     }
     for(int k=0;k<t.size();k++){
        arr[l+k]=t[k];
     }
}
void merge_sort(vector<int>&arr,int l,int r){
    if(l>=r)return ;
    int m=l+(r-l)/2; //等价于m=(l+r)/2;
    merge_sort(arr,l,m);
    merge_sort(arr,m+1,r);
    merge(arr,l,m,r);
}
   //打印
   void print(vector<int>&a)
{
    for(int i=0;i<a.size();i++){
        cout<<a[i]<<' ';
    }
    cout<<endl;
} 
int main(){
    vector <int> arr;
    for(int i=10;i>=1;i--){
        arr.push_back(i);                          
    }
    
     print(arr);
    
    print(arr);
    return 0;
    }
 
    