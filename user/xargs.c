#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include"kernel/fs.h"
#include"kernel/param.h"


#define MSGSIZE 16


int main(int argc,char* argv[])
{
    sleep(10);
    
    char buf[MSGSIZE];
    read(0,buf,MSGSIZE);

    char* xargv[MSGSIZE];
    int xargc=0;
    for(int i=1;i<argc;++i)
    {
        xargv[xargc++]=argv[i];
    }
    char* p=buf;
    for(int i=0;i<MSGSIZE;++i)
    {
        if(buf[i]=='\n')
        {
            int pid=fork();
            if(pid>0)
            {
                p=&buf[i+1];
                wait(0);
            }
            else
            {
                buf[i]=0;
                xargv[xargc++]=p;
                xargv[xargc++]=0;

                exec(xargv[0],xargv);
                exit(0);
            }
        }
    }
    wait(0);
    exit(0);
}