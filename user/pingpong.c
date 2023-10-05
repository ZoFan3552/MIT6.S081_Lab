#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

int read_error(int ,int);
int write_error(int ,int);
void main(){
    char buf[1] = {'1'};
    int p_to_c[2];
    int c_to_p[2];
    pipe(p_to_c);
    pipe(c_to_p);
    int pid = fork();
    int exit_stat = 0;
    if (pid == 0)
    {
        close(p_to_c[WRITE]);
        close(c_to_p[READ]);
        if (read_error((read(p_to_c[READ],buf ,1)),sizeof(buf)))
        {
            exit_stat = 1;
        }
        fprintf(1,"%d: received ping\n",getpid());
        if (write_error((write(c_to_p[WRITE] , buf ,1)),sizeof(buf)))
        {
            exit_stat = 1;
        }
        close(p_to_c[READ]);
        close(c_to_p[WRITE]);
        exit(exit_stat);
    }else if (pid < 0)
    {
        fprintf(2 ,"fork error!");
        exit(1);
    }
    
    else{
        close(p_to_c[READ]);
        close(c_to_p[WRITE]);
        if (write_error((write(p_to_c[WRITE] , buf ,1)),sizeof(buf)))
        {
            exit_stat = 1;
        }
        wait(0);
        if (read_error((read(c_to_p[READ],buf ,1)),sizeof(buf)))
        {
            exit_stat = 1;
        }
        close(p_to_c[WRITE]);
        close(p_to_c[WRITE]);
        fprintf(1,"%d: received pong\n",getpid());
        exit(exit_stat);
    }
    exit(exit_stat);
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