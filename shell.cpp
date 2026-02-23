#include <iostream>
#include <string.h>
#include <vector>
#include <ctype.h>

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
    for(int i=0;i<line.size();i++)
    {
    if(isspace(line[i]))
    {
        continue;
    }
    else if(line[i]=='|')
    {
        parse_line.push_back("|");
    }
    } 
}