#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fstream>
#include <signal.h>
#include <mutex>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

#include "threadpool.cpp"

using namespace std;

mutex mtx;
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
    if(cmd.substr(0,4)=="REST")return 10;
    return 0;
}

//user
void cmd_user(int clientfd,string &cmd){
    string username=tool(cmd);
    string uname="yanbing";
    if(username==uname){
        string resp="331 password required\r\n";
    send(clientfd,resp.c_str(),resp.size(),0);
        return ;
    }
        string resp= "530 invalid username\r\n";
    send(clientfd,resp.c_str(),resp.size(),0);
}

//pass
void cmd_pass(int clientfd,string &cmd,bool&isright){
    string pword="123";
     string password=tool(cmd);
    if(password==pword){
    string password="230 login successful\r\n";
    send(clientfd,password.c_str(),password.size(),0);
    isright=true;  
    }
    else{
        send_response(clientfd,
        "530 login incorrect\r\n");

        isright=false;
    }
}

//check login
bool check_login(int fd,bool &islogin,bool &isright){
    if(isright){
        islogin=true;
        return true;
    }
    string resp="530 please login first\r\n";
    send_response(fd,resp);
    return false;
}

//pasv
void cmd_pasv(int clientfd,int &d_listenfd){
    if(d_listenfd!=-1){
    close(d_listenfd);
}
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
    //获取服务器地址
     sockaddr_in localaddr;
     socklen_t locallen=sizeof(localaddr);
     getsockname(clientfd,(sockaddr*)&localaddr,&locallen);
     // 转换成字符串
string ip=inet_ntoa(localaddr.sin_addr);

// 192.168.1.100 -> 192,168,1,100
for(char &c:ip){
    if(c=='.'){
        c=',';
    }
}

string report=
"227 Entering Passive Mode ("+
ip+","+
to_string(p1)+","+
to_string(p2)+")\r\n";
    send(clientfd,report.c_str(),report.size(),0);
}

//list
void cmd_list(int clientfd,int &d_listenfd,
    bool &islogin,bool &isright){
    if(!check_login(clientfd,islogin,isright))return ;
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
void cmd_retr(int clientfd,int &d_listenfd,bool &islogin,
    bool &isright,string &f_cmd,long long&file_pos){
    if(!check_login(clientfd,islogin,isright))return ;
    if(!check_pasv(clientfd,d_listenfd))return ;
     
    int datafd=accept(d_listenfd,nullptr,nullptr);
    if(datafd<0){
     send_response(clientfd,
        "425 data connection failed\r\n");
     close(datafd);
     close(d_listenfd);
     d_listenfd=-1;
        return;
}

    string filename=tool(f_cmd);
    filename.erase(0, filename.find_first_not_of(" \t"));
    filename.erase(filename.find_last_not_of(" \t\r\n") + 1);
    
    if(filename.empty()){
        send_response(clientfd, "550 No filename given\r\n");
        close(datafd); close(d_listenfd); d_listenfd=-1;
        return;
    }
    int fd=open(filename.c_str(),O_RDONLY);
    //ifstream file(filename,ios::binary);
    if(fd<0){
        string resp="550 can't open the file\r\n";
        send(clientfd,resp.c_str(),resp.size(),0);
        close(datafd);
        close(d_listenfd);
        d_listenfd=-1;
        file_pos=0;
        return ;
    }
   /* file.seekg(0,ios::end);
    long long filesize=file.tellg();*/
     struct stat st;
     fstat(fd,&st);
     off_t filesize=st.st_size;
    if(file_pos>filesize){
        send_response(clientfd,
        "554 invalid restart position\r\n");
         close(datafd);
         close(d_listenfd);
         d_listenfd=-1;
         file_pos=0;
          return;
    }
    //file.seekg(file_pos,ios::beg);
    send_response(clientfd,"150 opening data connection\r\n");
     off_t offset=file_pos;
   /* char buf[1024];
    while(1){
       file.read(buf,sizeof(buf));
       int n=file.gcount();
       if(n<=0)break;//the last n=0
      if(!send_all(datafd,buf,n)){
        break;
      }
    }*/
   while(offset<filesize){
    ssize_t n=sendfile(datafd,fd,&offset,filesize-offset);
    if(n<=0){
        break;
    }
   }
    close(fd);
    close(datafd);
    close(d_listenfd);
    d_listenfd=-1;
    file_pos=0;
    string resp="226 transfer complete\r\n";
    send(clientfd,resp.c_str(),resp.size(),0);
}

//stor
void cmd_stor(int clientfd, int &d_listenfd, bool &islogin,
    bool &isright, string &f_cmd, long long &file_pos){

    if(!check_login(clientfd, islogin, isright)){
        file_pos = 0;
        return;
    }

    if(!check_pasv(clientfd, d_listenfd)){
        file_pos = 0;
        return;
    }

    // 先回应150
    send_response(clientfd,
        "150 Opening data connection\r\n");

    // 再accept
    int datafd = accept(d_listenfd, nullptr, nullptr);

    if(datafd < 0){
        send_response(clientfd,
            "425 data connection failed\r\n");

        close(d_listenfd);
        d_listenfd = -1;

        file_pos = 0;
        return;
    }

    string filename = tool(f_cmd);

    filename.erase(0,
        filename.find_first_not_of(" \t"));

    filename.erase(
        filename.find_last_not_of(" \t\r\n") + 1);

    if(filename.empty()){

        send_response(clientfd,
            "550 No filename given\r\n");

        close(datafd);
        close(d_listenfd);

        d_listenfd = -1;
        file_pos = 0;

        return;
    }

    fstream file;


    if(file_pos > 0){


        file.open(filename,
            ios::binary |
            ios::in |
            ios::out);

    
        if(!file.is_open()){

            file.clear();

            ofstream create_file(filename,
                ios::binary |
                ios::out);

            create_file.close();

       
            file.open(filename,
                ios::binary |
                ios::in |
                ios::out);
        }


        if(file.is_open()){
            file.seekp(file_pos, ios::beg);
        }

    }
   
    else{

        
        file.open(filename,
            ios::binary |
            ios::out |
            ios::trunc);
    }

    if(!file.is_open()){

        send_response(clientfd,
            "550 can't open or create file\r\n");

        close(datafd);
        close(d_listenfd);

        d_listenfd = -1;
        file_pos = 0;

        return;
    }

    // 接收文件
    char buf[4096];

    while(1){

        int n = recv(datafd,
            buf,
            sizeof(buf),
            0);

        if(n <= 0){
            break;
        }

        file.write(buf, n);

        if(!file){
            break;
        }
    }

    file.flush();
    file.close();

    close(datafd);
    close(d_listenfd);

    d_listenfd = -1;

    file_pos = 0;

    send_response(clientfd,
        "226 Transfer complete\r\n");
}
//quit
void cmd_quit(int clientfd){
string resp = "221 goodbye\r\n";
send(clientfd, resp.c_str(), resp.size(), 0);    
}

//size
void cmd_size(int clientfd,string&cmd){
  //string filenaetool(cmd);
   string filename=tool(cmd);
   filename.erase(0, filename.find_first_not_of(" \t"));
    filename.erase(filename.find_last_not_of(" \t\r\n") + 1);
    if(filename.empty()){
        send_response(clientfd, "550 No filename\r\n");
        return;
    }
   ifstream file(filename,ios::binary);
    if(!file){
        string resp="550 can't open the file 123\r\n";
        send(clientfd,resp.c_str(),resp.size(),0);
        return ;
    }
    if(file.is_open()){
        //指针移动到文件末尾
        file.seekg(0,ios::end);
    long long size=file.tellg();
    string resp="213 "+to_string(size)+"\r\n";
    send_response(clientfd,resp);
    }
    return ;
}

//type
//type
void cmd_type(int clientfd, string&cmd){
    string param = tool(cmd);
    if(param == "I" || param == "i"){
        string resp = "200 Switching to Binary mode.\r\n";
        send_response(clientfd, resp);
    }
    else if(param == "A" || param == "a"){
        string resp = "200 Switching to ASCII mode.\r\n";
        send_response(clientfd, resp);
    }
    else{
        string resp = "200 Type set to I (Binary).\r\n"; 
        send_response(clientfd, resp);
    }
}

//rest
void cmd_rest(int clientfd,string &cmd,long long &file_pos){
  string pos_line=tool(cmd);
  file_pos=stoll(pos_line);
  string resp="350 Restart position accepted\r\n";
    send_response(clientfd,resp);
}


void process_client(int clientfd){
     string welcome="220 Welcome to Simple FTP Server\r\n";
     send(clientfd,welcome.c_str(),welcome.size(),0);
    bool isright=false;
    bool islogin=false;
    int d_listenfd=-1;
    char buf[1024];
    long long file_pos=0;
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
   {
    lock_guard<mutex> lock(mtx);
    cout<<"收到命令"<<cmd<<endl;
   }
    int choose=explain(cmd);
    switch(choose){
        case 1:cmd_user(clientfd,cmd); break;
        case 2:cmd_pass(clientfd,cmd,isright); break;
        case 3:cmd_pasv(clientfd,d_listenfd);break;
        case 4:cmd_list(clientfd,d_listenfd,islogin,isright);break;
        case 5:cmd_retr(clientfd,d_listenfd,islogin,isright,cmd,file_pos);break;
        case 6:cmd_stor(clientfd,d_listenfd,islogin,isright,cmd,file_pos);break;
        case 7:cmd_quit(clientfd);isrun=false;break;
        case 8:cmd_size(clientfd,cmd);break;
        case 9:cmd_type(clientfd,cmd);break;
        case 10:cmd_rest(clientfd,cmd,file_pos);break;
        default:{
         string resp = "500 Unknown command\r\n";
         send(clientfd, resp.c_str(), resp.size(), 0);
         break;    
        }
    }
   }
   close(clientfd);
}


int main(){
    //当程序向一个已经关闭的连接（管道或Socket）写入数据时，
    //不要终止程序，而是忽略这个错误，让系统直接返回一个错误码给函数。
    signal(SIGPIPE,SIG_IGN);
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
//设置地址
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(2100);
    addr.sin_addr.s_addr=INADDR_ANY;
   
    //允许在 TIME_WAIT 状态立即重启服务器
    //允许多个 socket 绑定到同一端口（需配合其他条件）
    int opt = 1;
setsockopt(listenfd,
           SOL_SOCKET,
           SO_REUSEADDR,
           &opt,
           sizeof(opt));

    bind(listenfd,(sockaddr*)&addr,sizeof(addr));
    //监听
    listen(listenfd,5);
    
    cout<<"服务器启动，端口2100..."<<endl;
    threadpool pool(4);
    while(1){
    int clientfd=accept(listenfd,nullptr,nullptr);
    if(clientfd<0){
        continue;
    }
    cout<<"客户端连接成功"<<endl;
     pool.enqueue([clientfd](){
        process_client(clientfd);
     });
    }

close(listenfd);
    return 0;
}