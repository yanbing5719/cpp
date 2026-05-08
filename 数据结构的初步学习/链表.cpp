#include <iostream>
using namespace std;
struct node{
    int data;
    node *next;
    node(int val=0):data(val),next(nullptr){}//构造函数初始化链表
};

//尾插法
void weicha(node *head,int x){
    node *tail=head;
    node *p=new node(x);
    while(tail->next!=nullptr){
        tail=tail->next;
    } 
    tail->next=p;
}
//头插法
void toucha(node *head,int x){
    node *p=new node(x);
     p->next=head->next;
     head->next=p;
}
//查找定植
node *find(node*head,int x){
    while(head!=nullptr){
        if(head->data==x){
            return head;
        }
      head=head->next;
    }
}
//删除指定值
node *deleteobject(node *head,int x){
    if(head->data==x){
        node*t=head;
        head=head->next;
        delete t;
        return head;
    }
       node*p=head;
       node *cur=p->next;
       while(p->next!=nullptr){
        if(cur->data==x){
            p->next=cur->next;
            delete(cur);
            return head;
        }else{
        cur=cur->next;
        p=p->next;
       }
    }
       return head;
}
//按位置删除
node *deletepos(node*head,int k){
    if(k==1){
        node*t=head;
        head=head->next;
        delete(t);
        return head;
    }
    node*p=head;
    for(int i=1;i<=k-1&&p->next!=nullptr;i++){
        p=p->next;
    }
    node*temp=p->next;
    p->next=temp->next;
    delete(temp);
    return head;
}
//链表反转(三驱指针)
node *reverse(node*head){
 node*curr=head->next;
 node*prev=nullptr;
 while(curr!=nullptr){
    node *listnext=curr->next;
    curr->next=prev;
    prev=curr;
    curr=listnext;
 }
 return prev;
}
//合并两个有序链表
node *merge(node *l1,node *l2){
    node head;
    head.next=nullptr;
    node*tail=&head;
    while(l1!=nullptr&&l2!=nullptr){
      if(l1->data<l2->data){
        tail->next=l1;
        l1=l1->next;
      }else{
        tail->next=l2;
        l2=l2->next;
      }
       tail=tail->next;
    }
    tail->next=(l1!=nullptr)?l1:l2;
    
    return head.next;
}
//打印链表
void print(node*head){
    node *cur=head->next;
    while(cur!=nullptr){
        cout<<cur->data<<' ';
        cur=cur->next;
    }
    cout<<endl;
}

int main(){
    node *head=new node(0);
    for(int i=1;i<=10;i++){
      toucha(head,i);
    }
    node *l1=new node(0);
     for(int i=20;i<=30;i++){
        weicha(l1,i);
    }
    node*head1=merge(l1,head);
    print(head1);
    cout<< find(head,2)<<endl;
    print(head);
    deleteobject(head,5);
    print(head);
    deletepos(head,2);
    print(head);
     node*prev=reverse(head);
      print(prev);
    
  
}