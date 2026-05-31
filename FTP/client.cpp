#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class FtpClient{

private:

int consocket;
string recvbuffer;//Temporary buffer
string cmd;
string ans;

public:

FtpClient(){
    consocket=-1;
}

~FtpClient(){
    if(consocket!=-1){
        close(consocket);
    }
}

//-------------------------tool-----------------------------
//tool connect
int connect_socket(const string ip,int ports){
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        return -1;
    }
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(ports);
    inet_pton(AF_INET,ip.c_str(),&addr.sin_addr);
   if(connect(sock,(sockaddr*)&addr,sizeof(addr))<0){
    perror("connect");
    return -1;
   }
   return sock;
}

//explain command

string exp_cmd(string &ans){
if(ans=="ls"){
cmd="LIST";
}else if(ans.substr(0,3)=="get"){
cmd="RETR "+ans.substr(4);
}else if(ans.substr(0,3)=="put"){
cmd="STOR "+ans.substr(4);
}else if(ans.substr(0)=="quit"){
cmd="QUIT";
}else{
cmd=ans;
}
return cmd;
}

//explain pathname
string exp_path(const string&path){
size_t pos=path.find_last_of('/');
if(pos==string::npos){
    return path;
}
return path.substr(pos+1);
}

//send command ftp
bool send_cmd(const string &cmd){
    string data=cmd+"\r\n";
    if(sendall(consocket,data.c_str(),data.size())){
        return true;
    }
    return false;
}

//send all data
bool sendall(int sock,const char * data,int len){
    int total=0;

    while(total<len){
        int n=send(sock,data+total,len-total,0);
        if(n<=0){
           return false;
        }
        total+=n;
    }
    return true;
}



//recieve recvbuffer response
string recv_buffer(){
    
    char buf[1024];

    while(1){

       size_t pos=recvbuffer.find("\r\n");
       if(pos!=string::npos){
        string line=recvbuffer.substr(0,pos);
        recvbuffer.erase(0,pos+2);
        return line;
       }

       int n=recv(consocket,buf,sizeof(buf),0);
       if(n<=0){
        return "";
       }

       recvbuffer+=string(buf,n);
    }   
  }

  //creat sock and print cmd
  int creat_sock(const string cmd,int code){
     string ip;
     int port;
     if(!pasv(ip,port)){
        return -1;
     }
    int datasock=connect_socket(ip,port);
    if(datasock<0){
        return -1;
    }
    send_cmd(cmd);
    string resp=recv_buffer();
    cout<<resp<<endl;
    if(resp.size()<3){
    cout<<"服务器响应异常"<<endl;
    return -1;
}
    int re_code=stoi(resp.substr(0,3));
    if(re_code!=code){
        close(datasock);
        return -1;
    }
    return datasock;
  }
 
  //-----------------------------------------------------------
 
  //connect servre
bool ConnectServer(const string &ip,int ports){
    
    consocket=connect_socket(ip,ports);
    if(consocket<0){
        return false;
    }
    string resp=recv_buffer();
    cout<<resp<<endl;
    return resp.substr(0,3)=="220";
}

//client pasv（send data)
  bool pasv(string &ip,int &port){

  send_cmd("PASV");
  string resp=recv_buffer();
  if(resp.substr(0,3)!="227")return -1;
  cout<<resp<<endl;

  //recv ip (127,0,0,1,220,147)
  int p1=resp.find('(');
  int p2=resp.find(')');
  if(p1==string::npos||p2==string::npos){
    return false;
  }

  string np=resp.substr(p1+1,p2-p1-1);
  vector<int> nip;
  while(1){
    int pos=np.find(',');
    if(pos==string::npos){
        nip.push_back(stoi(np));
        break;
    }
    nip.push_back(stoi(np.substr(0,pos)));
    np.erase(0,pos+1);
  }

   port=nip[4]*256+nip[5];
 ip=to_string(nip[0])+"."+to_string(nip[1])
  +"."+to_string(nip[2])+"."+to_string(nip[3]);
  return true;
  }

  //Login
  bool login(const string&user,const string&pass){
    
    send_cmd("USER "+user);
    string resp=recv_buffer();
    //cout<<resp<<endl;
    if(resp.substr(0,3)!="331"){
        return false;
    }

    send_cmd("PASS "+pass);
    string resp1=recv_buffer();
    cout<<resp1<<endl;
    return resp1.substr(0,3)=="230";

  }

  //type
  bool type(){
    send_cmd("TYPE I");
    string resp=recv_buffer();
    cout<<resp<<endl;
    return resp.substr(0,3)=="200";
  }
  
  //rest
  bool rest(long long pos){
  send_cmd("REST "+to_string(pos));
  string recv=recv_buffer();
  cout<<recv<<endl;
  if(recv.size()<3){
    return false;
  }
  return recv.substr(0,3)=="350";
  }

  //list
 void list( ){
    int datasock=creat_sock("LIST",150);
    if(datasock<0)return ;
    char buf[4096];
    while(1){
    int n=recv(datasock,buf,sizeof(buf)-1,0);
    if(n<=0)break;
    buf[n]=0;
    cout<<buf;
    }
    close(datasock);
    cout<<recv_buffer()<<endl;
 }

 //retr
  void retr(const string &filename){
    long long pos=0;
    string local_file=filename;
    size_t pos_last = local_file.find_last_of("/\\");
    if(pos_last != string::npos){
        local_file= filename.substr(pos_last + 1);
    }
    ifstream checkfile(local_file,ios::binary);
    if(checkfile){
        checkfile.seekg(0,ios::end);
        pos=checkfile.tellg();      //获取当前指针的位置
        checkfile.close();
    }
   long long server_size=cmd_size(filename);
   if(server_size<0){
    cout<<"服务器文件不存在"<<endl;
    return ;
   }
   if(server_size==pos&&pos>0){
    cout<<"文件下载完成"<<endl;
    return ;
   }
   if(pos>server_size){
    cout<<"本地文件大于服务器文件"<<endl;
    return ;
   }
    if(pos>0){
          if(!rest(pos)){
               cout<<"REST failed"<<endl; 
               pos=0;
            }  
        }
    int datasock=creat_sock("RETR "+filename,150);
    if(datasock<0)return ;
    ofstream file(local_file,ios::binary|ios::app);
    if(!file){
        cout<<"文件创建失败"<<endl;
        close(datasock);
        return ;
    }
     char buf[4096];
     while(1){
    int n=recv(datasock,buf,sizeof(buf),0);
    if(n<=0)break;
    file.write(buf,n);
    }
    file.close();
    close(datasock);
    string resp=recv_buffer();
    cout<<resp<<endl;
    if(stoi(resp.substr(0,3))==226){
        cout<<"下载完成"<<endl;
    }else{
        cout<<"传输异常"<<endl;
    }
}

//stor

 void stor(const string &localpath,
    const string &remotepath){ 
        long long pos=0;
        long long server_size=cmd_size(remotepath);
        if(server_size>=0){
            pos=server_size;
            if(!rest(pos)){
                cout<<"REST failed"<<endl;
                pos=0;
            }
        }
        ifstream file(localpath,ios::binary);
         if(!file.is_open()){
        cout<<"can't find the file"<<endl;
        return ;
         }
        long long local_size = 0;
    file.seekg(0, ios::end);
    local_size = file.tellg();
    file.seekg(0, ios::beg);
   
        if(pos<0)pos=0;
        if(local_size==pos){
            cout<<"文件已成功上传或本地文件更小"<<endl;
            file.close();
            return;
        }
        if(pos>0){
          if(!rest(pos)){
               cout<<"REST failed"<<endl; 
               pos=0;
               file.seekg(0,ios::beg);
            }  
        }
        file.seekg(pos,ios::beg);
        int datasock=creat_sock("STOR "+remotepath,150);
        if(datasock<0)return ;

        char buf[4096];
     while(1){
        file.read(buf,sizeof(buf));
        int n=file.gcount();
        if(n<=0)break;
        if(!sendall(datasock,buf,n))break;
    }
    file.close();
    close(datasock);
    string resp=recv_buffer();
    cout<<resp<<endl;
    if(resp.size()<3){
    cout<<"服务器响应异常"<<endl;
    return;
}
    int code=stoi(resp.substr(0,3));
    if(code==226){
        cout<<"上传完成"<<endl;
    }else{
        cout<<"上传异常"<<endl;
    }
    }


//size
long long cmd_size(string filename){
        send_cmd("SIZE "+filename);
          string resp=recv_buffer();
         cout<<resp<<endl;
        if(resp.substr(0,3)!="213"){
         return -1;
    }
    string num=resp.substr(4);
    size_t pos=num.find("\r\n");
    if(pos!=string::npos){
        num=num.substr(0,pos);
    }
    return stoll(num);
    }

//shell
void shell(){
    while(1){
        string cmd;
        cout<<"ftp> ";
        getline(cin,ans);
        cmd=exp_cmd(ans);
        if(cmd.empty()){
            continue;
        }
        if(cmd=="LIST"){
            list();
            continue;
        }
        if(cmd.substr(0,4)=="RETR"){
            string filename=cmd.substr(5);
            //delete 空格
            while(!filename.empty()&&filename[0]==' '){
                filename.erase(0,1);
            }
            retr(filename);
            continue;
        }
        if(cmd.substr(0,4)=="STOR"){
            string localpath=cmd.substr(5);
            while(!localpath.empty()&&localpath[0]==' '){
                localpath.erase(0,1);
            }
            string remotepath=exp_path(localpath);
            stor(localpath,remotepath);
            continue;
        }
        send_cmd(cmd);
        string resp=recv_buffer();
        cout<<resp<<endl;
        if(resp.size()<3){
    cout<<"服务器响应异常"<<endl;
    return;
}
        int code=stoi(resp.substr(0,3));
        if(cmd=="QUIT"){
            if(code==221){
                break;
            }
        }
    }
}
};

int main(){
    FtpClient client;
    string ip;
    int port;
    cout<<"ip: ";
    cin>>ip;
    cout<<"port: ";
    cin>>port;
    if(!client.ConnectServer(ip,port))return 0;
    string user;
    string pass;
    cout<<"username:"<<endl;
    cin>>user;
    cout<<"password:"<<endl;
    cin>>pass;
    //delete th last char
    cin.ignore();
    if(client.login(user,pass))
{
    client.type();
    client.shell();
}
else
{
    cout << "登录失败\n";
}
return 0;
}