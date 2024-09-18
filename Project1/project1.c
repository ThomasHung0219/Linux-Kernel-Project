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
    
    // no.448的System call
    int a = syscall(314, getpid(), (void*)&my_data_segment);
    printf("system call return %d\n",a);
    printf("Start: %lx\nEnd: %lx\n", my_data_segment.start_code, my_data_segment.end_code);
    int b;
    scanf("%d",&b);    // 卡住程式
    return 0;
}