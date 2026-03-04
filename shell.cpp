#include <iostream>
#include <string.h>
#include <vector>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <fcntl.h>
#include <signal.h>

using namespace std;
string lastdir;

//读取一整行
string read_line()
{
 string line;
 getline(cin,line);
 return line;
}

//exec
void exec_command(vector<string> &cmd)
{
     char *argv[64];
    for(int i=0;i<cmd.size();i++)
    {
        argv[i]=(char*)malloc(cmd[i].size()+1);
        strcpy(argv[i],cmd[i].c_str());
    }
    argv[cmd.size()]=nullptr;
    execvp(argv[0],argv);
    perror("execvp");
    for(int i=0;i<cmd.size();i++)
    {
        free(argv[i]);
    }
    _exit(1);
}

//解析命令
vector<string> parse(string &line)
{
    vector<string> parse_line;
    for(int i=0;i<line.size(); )
    {
    if(isspace(line[i]))
    {
        i++;
    }
    else if(line[i]=='|')
    {
        parse_line.push_back("|");
        i++;
    } else if(line[i]=='<')
    {
        parse_line.push_back("<");
         i++;
    } else if(line[i]=='>')
    {
       if(i+1<line.size()&&line[i+1]=='>')
       {
        parse_line.push_back(">>");
        i+=2;
       }else
       {
        parse_line.push_back(">");
        i++;
       }
    }  else if(line[i]=='&')
    {
        parse_line.push_back("&");
        i++;
    }else
    {
          string word;
            while (i < line.size()&&
                   !isspace(line[i]) &&
                   line[i] != '|' &&
                   line[i] != '<' &&
                   line[i] != '>' &&
                   line[i] != '&') {
                word += line[i++];
    }
    parse_line.push_back(word);
    }
  } 
  return parse_line;
}

//cd命令的实现
void cd_command(vector<string> &parse_line)
{
 char cldir[1024];
 //获取当前目录
 if(getcwd(cldir,sizeof(cldir))==nullptr)
 {
    perror("getcwd");
    return ;
 } 
 const char *target=nullptr;
 //cd无参数
 if(parse_line.size()==1)
 {
    target=getenv("HOME");
    if(target==nullptr)
    {
        cout<<"error"<<endl;
        return ;
    }
 }
//cd-
else if(parse_line[1]=="-")
{
if(lastdir.empty())
{
  cout<<"error"<<endl;
  return ;
}
target=lastdir.c_str();
    cout<<target<<endl;
}
//普通路径
else
{
 target=parse_line[1].c_str();
}
if(chdir(target)<0)
{
    perror("cd");
}else{
  lastdir=cldir;
}
}

//普通命令的实现
void smiple_command(vector <string> &parse_line,bool background)
{
   pid_t pid=fork();
   if(pid==0)
   {
  exec_command(parse_line);
   }else
   {
    if(background==false)
   waitpid(pid,nullptr,0);
   }
}

//重定向命令的实现
void redirect(vector<string> &parse_line)
{
    for(int i=0;i<parse_line.size();)
    {
        int fd;
    if(parse_line[i]=="<")
    {
    fd=open(parse_line[i+1].c_str(),O_RDONLY);
      if(fd<0)
    {
        perror("open");
        _exit(1);
    }
    dup2(fd,0);
    close(fd);
    parse_line.erase(parse_line.begin()+i,parse_line.begin()+i+2);
    }
    else if(parse_line[i]==">")
    {
        fd=open(parse_line[i+1].c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
          if(fd<0)
    {
        perror("open");
        _exit(1);
    }
    dup2(fd,1);
    close(fd);
    parse_line.erase(parse_line.begin()+i,parse_line.begin()+i+2);
    }
    else if(parse_line[i]==">>")
    {
        fd=open(parse_line[i+1].c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
     if(fd<0)
    {
        perror("open");
        _exit(1);
    }
    dup2(fd,1);
    close(fd);
    parse_line.erase(parse_line.begin()+i,parse_line.begin()+i+2);
    }else{
        i++;
    }
  }
}

//分割管道
vector<vector<string>> cut_pipe(vector<string> &parse_line)
{
    vector<vector<string>> cmd;
    vector<string>cur;
    for(int i=0;i<parse_line.size();i++)
    {
        if(parse_line[i]=="|")
        {
            cmd.push_back(cur);
            cur.clear();
        }else
        {
            cur.push_back(parse_line[i]);
        }
    }
    if(!cur.empty())
    {
        cmd.push_back(cur);
    }
    return cmd;
}

//管道命令的实现
void pipe_command(vector<string> &parse_line,bool background)
{
  vector<vector<string>> split=cut_pipe(parse_line);
  int n=split.size();
 vector<vector<int>> pipefd(n-1,vector<int>(2));
  for(int i=0;i<n-1;i++)
  {
    if(pipe(pipefd[i].data())<0)
    {
        perror("pipe");
        return ;
    }
  }
for(int i=0;i<n;i++)
{
pid_t pid=fork();
if(pid==0)
{
    if(i!=0)
    {
 dup2(pipefd[i-1][0],0);
    }
    if(i!=n-1)
    {
 dup2(pipefd[i][1],1);
    }
    for(int j=0;j<n-1;j++)
    {
        close(pipefd[j][0]);
        close(pipefd[j][1]);
    }
 redirect(split[i]);
  exec_command(split[i]);
}
}
  for(int j=0;j<n-1;j++)
    {
        close(pipefd[j][0]);
        close(pipefd[j][1]);
    }
    if(background==false)
    {
    for(int i=0;i<n;i++)
    {
        wait(nullptr);
    }
}
}

//主函数
int main()
{
    //处理部分信号
signal(SIGINT,SIG_IGN);
signal(SIGCHLD,SIG_IGN);

    while(1)
    {
    string line=read_line();
    if(line.size()==0)
    {
        continue;
    }
    vector<string> parse_line=parse(line);
    //"&"
    bool background=false;
    if(parse_line.back()=="&")
    {
        background=true;
        parse_line.pop_back();
    }
    
    //管道
    bool ispipe=false;
    for(int i=0;i<parse_line.size();i++)
    {
        if(parse_line[i]=="|")
        {
            ispipe=true;
        break;
        }
    }

    //重定向
    bool isredirect=false;
    for(int i=0;i<parse_line.size();i++)
    {
         if (parse_line[i] == ">" ||
            parse_line[i] == "<" ||
            parse_line[i] == ">>")
        {
            isredirect=true;
            break;
    }
} 
    if(parse_line[0]=="exit")
    {
     break;
    }

    //cd
    if(parse_line[0]=="cd")
    {
     cd_command(parse_line);
        continue;
    }
    else if(ispipe)
    {
        pipe_command(parse_line,background);
        continue;
    }
    else if(isredirect)
    {
        pid_t pid=fork();
        if(pid==0)
        {
    redirect(parse_line);
    exec_command(parse_line);
        }else
        {
            if(background==false)
            waitpid(pid,nullptr,0);
        }
    }
    else{
        smiple_command(parse_line,background);
    }
    }
}