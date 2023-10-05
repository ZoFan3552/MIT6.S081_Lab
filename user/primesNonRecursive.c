#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 35
#define READ 0
#define WRITE 1

//非递归版本
int isPrime(int);
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
    else if (pid == 0)
    {
        close(p1[READ]);
        for (int i = 2; i <= MAX_NUM; i++){
            if(write_error(write(p1[WRITE] , &i ,sizeof(int)),sizeof(int))){
                exit_stat = 1;
            }
        }
        exit(exit_stat);
    }else{
        wait(0);
        close(p1[WRITE]);
        for (int i = 2; i <= MAX_NUM; i++)
        {
            int temp;
            if(read_error(read(p1[READ],&temp,sizeof(int)),sizeof(int))){
                exit_stat = 1;
            }
            if (isPrime(temp))
            {
                fprintf(1 ,"prime %d\n" ,temp);
            }
        }

        close(p1[READ]);
        close(p1[WRITE]);
        exit(exit_stat);
    }
}

int isPrime(int num){
     if (num <= 1) {
        return 0; // 0和1都不是素数
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0; // 如果能被整除，则不是素数
        }
    }
    return 1; // 否则是素数
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