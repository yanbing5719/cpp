#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
int main(){
    int socket_fd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(8888);
    //把字符串IP转成二进制
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);
    //客户端发起连接
    connect(socket_fd,(sockaddr*)&addr,sizeof(addr));
    const char * msg="hello";
    send(socket_fd,msg,strlen(msg),0);
    close(socket_fd);
    return  0;
}