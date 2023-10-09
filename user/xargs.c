#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_SIZE 1024

int main(int argc, char *argv[])
{
    char line[128] = {" "};
    char *args[64];
    char command[128];
    for (int i = 1, j = 0; i < argc; i++, j++) // 去除命令xargs
    {
        args[j] = argv[i];
    }

    // int pid = fork();
    while (strlen(line) != 0)
    {
        gets(line, sizeof(line));
        if (strlen(line) == 0)
        {
            // exit(0);
            break;
        }

        char *p = line;
        char *q = command;
        while (*p != '\n')
        {
            *q = *p;
            p++;
            q++;
        }
        // printf("命令：%s\n", command);
        args[argc - 1] = command;
        // for (int  i = 0; i < argc; i++)
        // {
        //     printf("命令：%s\n", args[i]);
        // }
        
        if (fork() == 0)
        {
            exec(args[0], args);
            continue;
            exit(0);
        }
        // else
        // {
        //     wait(0);
        //     exit(0);
        // }
    }
    // 应在循环外等待所有子进程退出后父进程再退出
    wait(0);
    exit(0);
}
