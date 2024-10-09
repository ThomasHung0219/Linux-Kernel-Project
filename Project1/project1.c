#include <syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

struct data_segment
{
    unsigned long start_code;
    unsigned long end_code;
};

int main()
{
    struct data_segment my_data_segment;
    
    int val = syscall(314, getpid(), (void*)&my_data_segment); // 我們新增的 System call 為 no.314
    printf("system call return %d\n", val);
    printf("Start: %lx\nEnd: %lx\n", my_data_segment.start_code, my_data_segment.end_code);
    int tmp;
    scanf("%d",&tmp);    // 卡住程式
    return 0;
}