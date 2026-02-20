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
/*
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
}*/
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;
int main(){
    int fd[2];
    if(pipe(fd)==-1){
        perror("pipe");
        return 1;
    }
    pid_t pid1=fork();
    if(pid1==-1){
        perror("fork");
        return 1;
    }
    if(pid1==0){
    dup2(fd[1],1);
    close(fd[0]);
    close(fd[1]);
   execlp("ls","ls",nullptr);
   perror("exec,ls");
   _exit(1);
    }
    pid_t pid2=fork();
    if(pid2==-1){
        perror("fork");
        return 1;
    }
    if(pid2==0){
        dup2(fd[0],0);
        close(fd[0]);
        close(fd[1]);
        execlp("wc","wc","-l",nullptr);
        perror("exec wc");
        _exit(1);
        close(fd[0]);
        close(fd[1]);
        waitpid(pid1,nullptr,0);
        waitpid(pid2,nullptr,0);
        return 0;
    }
}