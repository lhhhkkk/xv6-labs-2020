#include"kernel/types.h"
#include"user/user.h"

#define RD 0
#define WR 1

void primes(int pipe_left[2])
{
    int prime;
    close (pipe_left[WR]);//关闭写
    if(read(pipe_left[RD],&prime,sizeof(prime))==0)//递归的终止条件，当没有数据时，退出，一层一层往上返回。
    {
        exit(0);
    }
    printf("prime %d\n",prime);//打印质数
    int pipe_right[2];//创建右管道，父进程读，子进程写
    pipe(pipe_right);
    int num;
    while(read(pipe_left[RD],&num,sizeof(num)))
    {
        if(num%prime!=0)//筛选出不是质数的数
        {
            write(pipe_right[WR],&num,sizeof(num));
        }
    }
    if(fork()==0)
    {
        primes(pipe_right);//递归调用
    }
    else
    {
        close(pipe_right[WR]);//关闭写
        close(pipe_right[RD]);//关闭读
        wait(0);
    }
    exit(0);   

}




int main(int argc,char const* argv[])
{
    int pipe_left[2];//创建左管道，父进程写，子进程读
    pipe(pipe_left);

    for(int i=2;i<=35;++i)//写入2-35
    {
        write(pipe_left[WR],&i,sizeof(i));
    }

    if(fork()==0)//创建子进程
    {
        primes(pipe_left);
    }
    else{
        close(pipe_left[WR]);//关闭写
        close(pipe_left[RD]);//关闭读
        wait(0);
    }
    exit(0);
}