#include <iostream>
#include <string.h>
#include <vector>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>

using namespace std;

//读取一整行

string read_line()
{
 string line;
 getline(cin,line);
 return line;
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
   char *argv[64];
   for(int i=0;i<parse_line.size();i++)
{
  argv[i]=(char*)malloc(parse_line[i].size()+1);
  strcpy(argv[i],parse_line[i].c_str());
}
argv[parse_line.size()]=nullptr;
   pid_t pid=fork();
   if(pid==0)
   {
   execvp(argv[0],argv);
   perror("execvp");
   _exit(1);
   }else
   {
   waitpid(pid,nullptr,0);
   }
   for(int i=0;i<parse_line.size();i++){
    free(argv[i]);
   }
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
     if(!ispipe&&!isredirect)
     {
        smiple_command(parse_line);
        continue;
     }
     char *argv[64];
     for(int i=0;i<n;i++)
{
  argv[i]=(char*)malloc(parse_line[i].size()+1);
  strcpy(argv[i],parse_line[i].c_str());
}
   argv[parse_line.size()]=nullptr;
   pid_t pid=fork();
   if(pid==0)
   {
   
   execvp(argv[0],argv);
   perror("execvp");
   _exit(1);
   }else
   {
   waitpid(pid,nullptr,0);
   }
   for(int i=0;i<parse_line.size();i++){
    free(argv[i]);
   }
}
}