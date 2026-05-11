#include <iostream>
#include <queue>
#include <vector>
using namespace std;
class Solution {
private:
priority_queue<int,vector<int>,greater<int>>pq;
int k;
public:
    int findKthLargest(vector<int>& nums, int k) {
        for(int x:nums){
            pq.push(x);
        }
        while(pq.size()>k){
            pq.pop();
        }
        return pq.top();
    }
};
int main(){
    priority_queue <int> arr;
    for(int i=0;i<20;i+=2){
        arr.push(i);
    }
      cout<<arr.top()<<endl;
      priority_queue <int,vector<int>,greater<int>> pq;
      for(int i=0;i<50;i=2*i+1){
        pq.push(i);
      }
      cout<<pq.top();
}