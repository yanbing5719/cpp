/*#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;
int main(){
    pid_t pid=fork();
    if(pid==0){
        char* argv[]={
            (char*) "ls",
            (char*) "-l",
            nullptr
        };
        extern char **environ;
        execve("/bin/ls",argv,environ);
    }else{
        wait(nullptr);
        cout<<"child finished"<<endl;
    }
return 0;
}*/
/*#include <iostream>
#include <unistd.h>
using namespace std;
int main(){
     char* argv[]={
            (char*) "ls",
            (char*) "-l",
            nullptr
        };
    execvp("ls",argv);
    return 0;
}*/
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>  
using namespace std;
int main(){
    pid_t pid=fork();
    if(pid==0){
        char *argv[]={
            (char*) "ls",
            (char*) "-a",
            nullptr};
    execvp("ls",argv);
    perror("execvp failed");
    }else if(pid>0){
        wait(nullptr);
        cout<<"child finished";
    }else{
        perror("fork failed");
    }
    return 0;
}