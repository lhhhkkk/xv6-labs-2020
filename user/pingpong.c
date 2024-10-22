#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"




int main()
{
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);


    int pid=fork();
    if(pid==0)
    {
        char buf[32]={0};
        close(c2p[0]);//关闭子通道读
        close(p2c[1]);//关闭父通道写

        read(p2c[0],buf,sizeof(buf));
        close(p2c[0]);
        printf("%d, receive ping\n",getpid());
        write(c2p[1],"pong",4);
        close(c2p[1]);
        exit(0);
    }

    else
    {
        char buf[32]={0};
        close(p2c[0]);//关闭父通道读
        close(c2p[1]);//关闭子通道写

        write(p2c[1],"ping",4);
        close(p2c[1]);
        read(c2p[0],buf,sizeof(buf));
        printf("%d: received pong\n",getpid());
        close(c2p[0]);

        exit(0);
    }
}