#include <iostream>
#include <string.h>
#include <vector>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <fcntl.h>

using namespace std;


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

//普通命令的实现
void smiple_command(vector <string> &parse_line)
{
   pid_t pid=fork();
   if(pid==0)
   {
  exec_command(parse_line);
   }else
   {
   waitpid(pid,nullptr,0);
   }
}

//管道命令的实现
void pipe_command(vector<string> &parse_line)
{
  int n=parse_line.size();
  int pos=-1;
  for(int i=0;i<n;i++)
  {
    if(parse_line[i]=="|")
    {
    pos=i;
    break;
    }
  }
  vector<string> left(parse_line.begin(),parse_line.begin()+pos);
  vector<string> right(parse_line.begin()+pos+1,parse_line.end());
  int fd[2];
  if(pipe(fd)==-1)
{
  perror("pipe");
  return ;
}
pid_t pid1=fork();
if(pid1==0)
{
 dup2(fd[1],1);
 close(fd[0]);
 close(fd[1]);
  exec_command(left);
}
pid_t pid2=fork();
if(pid2==0)
{
 dup2(fd[0],0);
 close(fd[0]);
 close(fd[1]);
  exec_command(right);
}
close(fd[0]);
close(fd[1]);
waitpid(pid1,nullptr,0);
waitpid(pid2,nullptr,0);
}

//重定向命令的实现
void redirect(vector<string> &parse_line)
{
    int n=parse_line.size();
    int pos=-1;
    string obj;
    for(int i=0;i<n;i++)
    {
         if (parse_line[i] == ">" ||
            parse_line[i] == "<" ||
            parse_line[i] == ">>")
        {
            pos=i;
            obj=parse_line[i];
            break;
        }
    }
    if(pos==-1)
    {
        return ;
    }
    vector<string>cmd(parse_line.begin(),parse_line.begin()+pos);
    string filename=parse_line[pos+1];
    pid_t pid=fork();
    if(pid==0)
    {
        int fd;
    if(obj=="<")
    {
    fd=open(filename.c_str(),O_RDONLY);
      if(fd<0)
    {
        perror("open");
        _exit(1);
    }
    dup2(fd,0);
    close(fd);
    }
    else if(obj==">")
    {
        fd=open(filename.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
          if(fd<0)
    {
        perror("open");
        _exit(1);
    }
    dup2(fd,1);
    close(fd);
    }
    else if(obj==">>")
    {
        fd=open(filename.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
     if(fd<0)
    {
        perror("open");
        _exit(1);
    }
    dup2(fd,1);
    close(fd);  
    }
   exec_command(cmd);
  }
  waitpid(pid,nullptr,0);
}
//主函数

int main()
{
    while(1)
    {
    string line=read_line();
    if(line.size()==0)
    {
        continue;
    }
    vector<string> parse_line=parse(line);
    int n=parse_line.size();
    //管道
    bool ispipe=false;
    for(int i=0;i<n;i++)
    {
        if(parse_line[i]=="|")
        {
            ispipe=true;
        break;
        }
    }

    //重定向
    bool isredirect=false;
    for(int i=0;i<n;i++)
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
        if(n<2){
            cout<<"error"<<endl;
            return 0;
        }
        chdir(parse_line[1].c_str());
        continue;
    }
    else if(ispipe)
    {
        pipe_command(parse_line);
        continue;
    }
    else if(isredirect)
    {

    }
    else{
        smiple_command(parse_line);
    }
   
     /*if(!ispipe&&!isredirect)
     {
        smiple_command(parse_line);
        continue;
     }*/
    }
}