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
    (head->data==x){
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
    node *head;
    for(int i=1;i<=10;i++){
      toucha(head,i);
    }
    cout<< find(head,2)<<endl;
    print(head);
    deleteobject(head,5);
    print(head);
}