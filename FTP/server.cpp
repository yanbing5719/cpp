#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fstream>

using namespace std;

//            ----------tool function-----------
//command parameter
string tool(const string &f_cmd){
    string filename;
    size_t pos=f_cmd.find(' ');
    if(pos==string::npos){
        return "";
    }
    return f_cmd.substr(pos+1);
}

//send data
bool send_all(int fd,char *buf,int len){
int total=0;
while(total<len){
    int n= send(fd,buf+total,len-total,0);
    if(n<=0)return false;
    total+=n;
}
return true;
}

//send response
void send_response(int fd,const string&resp){
    send_all(fd,(char*)resp.c_str(),resp.size());
}
//unpack
bool cmd_recv(int fd,string &cache,string &cmd){
    while(true){
        size_t pos=cache.find("\r\n");
        if(pos!=string::npos){
            cmd=cache.substr(0,pos);
            cache.erase(0,pos+2);
            return true;
        }
        char buf[1024];
        int n=recv(fd,buf,sizeof(buf),0);
        if(n<=0)return false;
        cache+=string(buf,n);
    }
  
}

//check pasv
bool check_pasv(int fd,int &d_listenfd){
  if(d_listenfd==-1){
        string resp="425 use pasv first\r\n";
        send_response(fd,resp);
        return false;
    }
    return true;
}

//explain command
int explain(string &cmd){
    if(cmd.substr(0,4)=="USER")return 1;
    if(cmd.substr(0,4)=="PASS")return 2;
    if(cmd.substr(0,4)=="PASV")return 3;
    if(cmd.substr(0,4)=="LIST")return 4;
    if(cmd.substr(0,4)=="RETR")return 5;
    if(cmd.substr(0,4)=="STOR")return 6;
    if(cmd.substr(0,4)=="QUIT")return 7;
    if(cmd.substr(0,4)=="SIZE")return 8;
    if(cmd.substr(0,4)=="TYPE")return 9;
    return 0;
}

//user
bool cmd_user(int clientfd,string &cmd){
    string username=tool(cmd);
    string uname="yanbing";
    if(username==uname){
        string resp="331 password required\r\n";
    send(clientfd,resp.c_str(),resp.size(),0);
        return true;
    }
        string resp= "530 invalid username\r\n";
    send(clientfd,resp.c_str(),resp.size(),0);
        return false;
    
}

//pass
bool cmd_pass(int clientfd,string &cmd){
    string pword="yhsxxn";
     string password=tool(cmd);
    if(password==pword){
    string password="230 login successful\r\n";
    send(clientfd,password.c_str(),password.size(),0);
    return true;    
    }else{
        return false;
    }
}

//check login
bool check_login(int fd,bool &islogin){
    if(cmd_user&&cmd_pass){
        islogin=true;
        return true;
    }
    string resp="530 please login first\r\n";
    send_response(fd,resp);
    return false;
}

//pasv
void cmd_pasv(int clientfd,int &d_listenfd){
    d_listenfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=0;
    addr.sin_addr.s_addr=INADDR_ANY;
    //监听
    bind(d_listenfd,(sockaddr*)&addr,sizeof(addr));
    listen(d_listenfd,1);
    //获取地址，生成端口
    socklen_t len=sizeof(addr);
    getsockname(d_listenfd,(sockaddr*)&addr,&len);
    int port=ntohs(addr.sin_port);
    //返回FTP要求的端口号
    int p1=port/256;
    int p2=port%256;
    string report="227 Entering Passive Mode (127,0,0,1,"+
    to_string(p1)+","+to_string(p2)+")\r\n";
    send(clientfd,report.c_str(),report.size(),0);
}

//list
void cmd_list(int clientfd,int &d_listenfd,bool&islogin){
    if(!check_login(clientfd,islogin))return ;
    if(!check_pasv(clientfd,d_listenfd))return ;
    string resp="150 opening data connection\r\n";
    send(clientfd,resp.c_str(),resp.size(),0);
    int datafd=accept(d_listenfd,nullptr,nullptr);
    //对目录进行操作
    DIR *dir=opendir(".");
    struct dirent *entry;
    string file;
    while((entry=readdir(dir))!=nullptr){
        //skip "."and ".."
        if(strcmp(entry->d_name,".")==0||
        strcmp(entry->d_name,"..")==0){
        continue;
        }
     file+=entry->d_name;
     file+="\r\n";
    }
    closedir(dir);
   send_all(datafd,(char*)file.c_str(),file.size());
    close(datafd);
    close(d_listenfd);
    d_listenfd=-1;
    string complete="226 transfer complete\r\n";
    send(clientfd,complete.c_str(),complete.size(),0);
}

//retr
void cmd_retr(int clientfd,int &d_listenfd,bool&islogin,string &f_cmd){
    if(!check_login(clientfd,islogin))return ;
    if(!check_pasv(clientfd,d_listenfd))return ;
    string filename=tool(f_cmd);
    ifstream file(filename,ios::binary);
    if(!file){
        string resp="550 can't open the file\r\n";
        send(clientfd,resp.c_str(),resp.size(),0);
        return ;
    }
    int datafd=accept(d_listenfd,nullptr,nullptr);
    char buf[1024];
    while(1){
       file.read(buf,sizeof(buf));
       int n=file.gcount();
       if(n<=0)break;//the last n=0
      if(!send_all(datafd,buf,n)){
        break;
      }
    }
    close(datafd);
    close(d_listenfd);
    d_listenfd=-1;
    string resp="226 transfer complete\r\n";
    send(clientfd,resp.c_str(),resp.size(),0);
}

//stor
void cmd_stor(int clientfd,int &d_listenfd,bool&islogin,string &f_cmd){
if(!check_login(clientfd,islogin))return ;
if(!check_pasv(clientfd,d_listenfd))return ;
string filename=tool(f_cmd);
ofstream file(filename,ios::binary);
send(clientfd, "150 Opening data connection\r\n", 31, 0);
int datafd=accept(d_listenfd,nullptr,nullptr);
char buf[1024];
string cache;
while(1){
   int n=recv(datafd,buf,sizeof(buf),0);
   if(n<=0)break;
   file.write(buf,n);
}
close(datafd);
close(d_listenfd);
d_listenfd=-1;
string resp = "226 Transfer complete\r\n";
send(clientfd, resp.c_str(), resp.size(), 0);
}

//quit
void cmd_quit(int clientfd){
string resp = "221 goodbye\r\n";
send(clientfd, resp.c_str(), resp.size(), 0);    
}

//size
void cmd_size(int clientfd,string&cmd){
  string filenaetool(cmd);
   string filename=tool(cmd);
    ifstream file(filename,ios::binary);
    if(!file){
        string resp="550 can't open the file\r\n";
        send(clientfd,resp.c_str(),resp.size(),0);
        return ;
    }
    if(file.is_open()){
    long long size=file.tellg();
    string resp="213 "+to_string(size)+"\r\n";
    send_response(clientfd,resp);
    }
    return ;
}

//type
void cmd_type(int clientfd,string&cmd){
    string resp="200 Type set to I\r\n";
    send_response(clientfd,resp);
}

int main(){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
//设置地址
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(2100);
    addr.sin_addr.s_addr=INADDR_ANY;
   
    bind(listenfd,(sockaddr*)&addr,sizeof(addr));
    //监听
    listen(listenfd,5);
    
    cout<<"服务器启动，端口2100..."<<endl;
   
    int clientfd=accept(listenfd,nullptr,nullptr);
    cout<<"客户端连接成功"<<endl;
     
    string welcome="220 Welcome to Simple FTP Server\r\n";
     send(clientfd,welcome.c_str(),welcome.size(),0);
    
     bool islogin=false;
     int d_listenfd=-1;
     char buf[1024];
    bool isrun=true;
    string cache;
   while(isrun){
    //初始化
    memset(buf,0,sizeof(buf));
       string cmd;
    bool ok=cmd_recv(clientfd,cache,cmd);
    if(!ok){
        cout<<"断开连接"<<endl;
        break;
    }
   
    cout<<"收到命令"<<cmd<<endl;
    int choose=explain(cmd);
    switch(choose){
        case 1:cmd_user(clientfd,cmd); break;
        case 2:if(!cmd_pass(clientfd,cmd))isrun=false;break;
        case 3:cmd_pasv(clientfd,d_listenfd);break;
        case 4:cmd_list(clientfd,d_listenfd,islogin);break;
        case 5:cmd_retr(clientfd,d_listenfd,islogin,cmd);break;
        case 6:cmd_stor(clientfd,d_listenfd,islogin,cmd);break;
        case 7:cmd_quit(clientfd);isrun=false;break;
        case 8:cmd_size(clientfd,cmd);break;
        case 9:cmd_size(clientfd,cmd);break;
        default:{
         string resp = "500 Unknown command\r\n";
         send(clientfd, resp.c_str(), resp.size(), 0);
         break;    
        }
    }
   }
    close(listenfd);
    close(clientfd);
    return 0;
}