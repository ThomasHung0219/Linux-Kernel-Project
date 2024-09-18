# Project1
>https://hackmd.io/t2tM_VFoTkyike6zlCmLWA?view

## 題目
1. 寫一個 system call 從 kernel space 找出指定 process(task) 的 code(text) 段的位址
2. user 段程式利用 system call 傳入 pid 並取得結果放至 user space 的 buffer 上，再輸出至 terminal 上。

## 實驗
1. 增加一個新的 system call number 至 kernel 的 system call table 中(我們使用的系統為64bit，修改檔案 syscall_64.tbl)。
```
$ vim ~/linux-3.10.104/arch/x86/syscalls/syscall_64.tbl
```

![](/fig/add_system_call_table.png)

2. 定義 system call
```
$ cd linux-3.10.104
$ mkdir pass_kernel_data
$ cd pass_kernel_data
$ vim pass_kernel_data.c
```
[task_struct](https://elixir.bootlin.com/linux/v3.10.104/source/include/linux/sched.h#L1041) : linux 用於描述 process。

[mm_struct](https://elixir.bootlin.com/linux/v3.10.104/source/include/linux/mm_types.h#L366)

從 init_task 開始遍歷 task_list 至指定 task，傳出指定資料至指定記憶體段。

pass_kernel_data.c :

```C++
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/init_task.h>
#include <linux/syscalls.h>


// you need to pass this data struct to userspace and print on terminal
struct data_segment
{
    unsigned long start_code;
    unsigned long end_code;
};

SYSCALL_DEFINE2(pass_kernel_data, pid_t, user_pid, void* __user, user_address)
{
    struct data_segment my_data_segment;
    struct task_struct *task;
    for_each_process(task)
    {
        if(task->pid == user_pid)
        {
            my_data_segment.start_code = task->mm->start_code;
            
            my_data_segment.end_code = task->mm->end_code;
           
            copy_to_user(user_address, &my_data_segment, sizeof(struct data_segment));
        }
    }

    return 0;
}
```

2. 建立 pass_kernel_data 資料夾的 makefile
```
$ vim Makefile
```
makefile 的內容:
```
obj-y := pass_kernel_data.o
```
3. 返回上一層(linux-3.10.104)，修改 makefile
```
$ cd ..    // to linux-3.10.104/
$ vim Makefile
```
找到 core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/

在最後面新增 pass_kernel_data/

4. 將新增的 system call 加入 syscall.h 中。
```
$ vim ~/linux-3.10.104/include/linux/syscalls.h
```
在最下面(#endif前)新增一行 :
```
asmlinkage int pass_kernel_data(pid_t user_pid, void* __user user_address);
```

5. 重新編譯 kernel。
```
$ sudo make menuconfig    // 使用預設即可
$ sudo make -j$(nproc)    // 包含make bzImage、make modules...等動作
$ sudo make modules_install -j$(nproc) 
$ sudo make install -j$(nproc)        // 安裝kernel
```

6. grub

    a. 設定 grub 選單。
```
$ sudo vim /etc/default/grub
更改以下設定
GRUB_TIMEOUT=0 -> GRUB_TIMEOUT=-1
GRUB_TIMEOUT_STYLE=menu
```
<br>

b. 更新 grub 選單，並重啟。

```
$ sudo update-grub
$ sudo reboot
```
c. 重啟後選擇編譯的 kernel 版本

   ![](/fig/grub1.png)
   ![](/fig/grub2.png)

7. 創建 project1.c 來呼叫 mysyscall。
```
$ vim project1.c
$ gcc project1.c -o test
$ ./project1
$ dmesg | tail    // see the kernel log info
```

project1.c :
```C++
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
```

8. 測試

    開啟兩個 terminal，一邊執行project1，另一邊使用```$ ps -a``` 指令可觀看目前同個終端下的執行程式，從而獲取正在執行 process 的 pid。

    ![](/fig/test1.png)

    使用 pid 取得 process 的位址分布。
    ```
    $ cd /proc/2488    //每次執行分配的pid不同，如下圖pid=2488
    $ cat maps
    ```

    ![](/fig/test2.png)

## 問題