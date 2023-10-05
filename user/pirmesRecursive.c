#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 35
#define READ 0
#define WRITE 1

//递归版本
int read_error(int ,int);
int write_error(int ,int);
void main(){
    int p1[2];
    //第一个进程将数字2到35输入管道
    pipe(p1);
    int pid = fork();
    int exit_stat = 0;
    if (pid < 0)
    {
        fprintf(2 ,"fork error!");
        exit(1);
    }
    if (pid == 0)
    {
        for (int i = 2; i <= MAX_NUM; i++)
        {
            close(p1[READ]);
            write(p1[WRITE],&i ,sizeof(int));   
        }
        exit(0);
    }else{
        wait(0);
        getPrime(33 ,p1);
    }
    
}

void getPrime(int count, int pipe[2]){
    if (count == 0)
    {
        return;       
    }
    
    int pid = fork();
    int exit_stat = 0;
    if (pid < 0)
    {
        fprintf(2 ,"fork error!");
        exit(1);
    }
    int temp_count = 0;
    if (pid == 0)
    {
        
        for (int i = 0; i < count; i++)
        {
            int temp;
            read(pipe[READ] ,&temp ,sizeof(int));
            if (i == 1)
            {
                fprintf(1 ,"prime %d" ,temp);
            }
            if (!(temp % i == 0))
            {
                temp_count++;
                write(pipe[WRITE],&temp ,sizeof(int));
            }  
        }
        close(pipe[READ]);
        close(pipe[WRITE]);
        exit(0);

    }else{
        wait(0);
        exit(0);
    }
    getPrime(temp_count,pipe[2]);
}

int read_error(int read_buf_size ,int buf_size){
    if (read_buf_size != buf_size)
    {
        fprintf(2,"read error\n!");
        return 1;
    }else{
        return 0;
    }
    
}

int write_error(int write_buf_size , int buf_size){
    if (write_buf_size != buf_size)
    {
        fprintf(2 ,"write error\n!");
        return 1;
    }else{
        return 0;
    }
    
}