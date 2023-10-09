#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_SIZE 1024

int main(int argc, char *argv[])
{
    char line[128];
    char *args[64];
    for (int i = 1, j = 0; i < argc; i++, j++) // 去除命令xargs
    {
        args[j] = argv[i];
    }

    do
    {
        gets(line, sizeof(line));
        if (strlen(line) == 0)
        {
            break;
        }

        char *p = line;
        // printf("line : %s$", line);
        while (1)//去掉换行符
        {
            if (*p == '\n')
            {
                *p = '\0';
                break;
            }
            p++;
        }
        args[argc - 1] = line;
        if (fork() == 0)
        {
            // for (int i = 0; i < argc; i++)
            // {
            //     printf("arg%d : %s $", i, args[i]);
            // }
            // printf("\n");
            exec(args[0], args);
            exit(0);
        }
        wait(0);
    } while (strlen(line) != 0);
    // 应在循环外等待所有子进程退出后父进程再退出
    wait(0);
    exit(0);
}
