#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 35
#define READ 0
#define WRITE 1

//递归版本

int left_p_first(int *p ,int *dest){
    if ((read(p[READ] ,dest ,sizeof(int))) == sizeof(int))
    {
        fprintf(1 ,"prime %d\n",*dest);
        return 1;
    }

    return 0;
    
}

void transmit(int *lpipe ,int *rpipe ,int first){
    int data;
    while (read(lpipe[READ] ,&data ,sizeof(int)) == sizeof(int))
    {
        if (data % first)
        {
            write(rpipe[WRITE] ,&data ,sizeof(int));
        }
    }
    close(lpipe[READ]);
    close(rpipe[WRITE]);
}

void getPrime(int *lpipe){
    close(lpipe[WRITE]);
    int first;
    if (left_p_first(lpipe ,&first))
    {
        int rpipe[2];
        pipe(rpipe);
        transmit(lpipe ,rpipe ,first);

        if (fork() == 0)
        {
            getPrime(rpipe);
        }else{
            close(rpipe[READ]);
            wait(0);
        }

    }
    exit(0);

}
void main(){
    int p[2];
    pipe(p);

    for (int i = 2; i <= MAX_NUM; i++)
    {
        write(p[WRITE] , &i,sizeof(int));   
    }

    if (fork() == 0)
    {
        getPrime(p);
    }else{
        close(p[READ]);
        close(p[WRITE]);
        wait(0);
    }
    exit(0);
}



