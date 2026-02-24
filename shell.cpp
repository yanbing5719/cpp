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

//