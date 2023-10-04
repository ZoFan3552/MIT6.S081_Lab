#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc ,char *argv[]){
    uint time;
    if (argc < 2)
    {
        fprintf(2,"usage : sleep + time\n");
        exit(1);
    }

    time = atoi(argv[1]);
    fprintf(1 , "开始休眠\n");
    sleep(time);
    fprintf(1 , "休眠结束\n");
    exit(0);
}