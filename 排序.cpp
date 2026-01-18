#include <iostream>
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
int main(){
    int arr[]={0,5,2,9,1,5,6};
    int n=7;
  bubble_sort(arr,n);
    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }  
    return 0;
    }