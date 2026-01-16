#include <iostream>
using namespace std;
 const int N=1000;
 int n,stk[N];
// 进栈
void push(int x)
{
 stk[++n]=x;
}
// 出栈
void pop(){
    n--;
}
//查询栈顶元素
int top()
{
    return stk[n];
}
// 判断栈是否为空
bool empty()
{
    return n==0;
}
//查询有效元素的个数
int size()
{
    return n;
}
int main ()
{
for(int i=1;i<=10;i++){
    push(i);
}
while(!empty())
{
    cout<<top()<<endl;
    pop();
}
return 0;
}

//stl封装栈
#include <iostream>
#include <stack>
using namespace std;
int main()
{
    stack<int> st;
    for(int i=1;i<=10;i++)
    {
        st.push(i);
    }
    while(!st.empty())
    {
        cout<<st.top()<<endl;
        st.pop();
    }
    return 0;
}