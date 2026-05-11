#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
 
void parse(string &buf){
    while(1){
        if(buf.size()<1)break;
        int len=buf[0]-'0';
        if(buf.size()<1+len)break;
        string msg=buf.substr(1,len);
        buf.erase(0,len+1);
    }
}

int main(){
    int serev_fd=socket(AF_INET,SOCK_STREAM,0);
    //地址结构体，描述地址
    sockaddr_in addr;
    addr.sin_family=AF_INET;//IPV4
    addr.sin_port=htons(8888);//端口号
    addr.sin_addr.s_addr=INADDR_ANY;//监听所有IP

    //将socket绑到一个地址上
    bind(serev_fd,(sockaddr*)&addr,sizeof(addr));

    //监听
    listen(serev_fd,5);
    cout<<"服务器启动，等待连接..."<<endl;

    //接受连接

    int client_fd=accept(serev_fd,nullptr,nullptr);
    cout<<"客户端已经连接"<<endl;
    //接收数据
    char buf[1024]={0};
    int n=recv(client_fd,buf,sizeof(buf),0);
    cout<<"accept data: "<<buf<<endl;
    cout<<"bate number: "<<n<<endl;
    //close
    close(client_fd);
    close(serev_fd);
    return 0;
     
}