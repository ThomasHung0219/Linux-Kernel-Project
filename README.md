# Linux-Kernel-Project
中央大學 許富皓教授 Linux作業系統專題
---------------------------------------
## Environment
OS:[Ubuntu 16.04](https://releases.ubuntu.com/16.04/?_ga=2.247640007.1301183573.1726571101-1581265748.1726571101)
<br>
Linux Kernel Version:[3.10.104](https://mirrors.edge.kernel.org/pub/linux/kernel/v3.0/linux-3.10.104.tar.gz)

## Set up VM
1. 使用 VirtualBox 創建一個新的虛擬機，給予空間不要太小。
2. 解壓縮 kernel 檔案，並進入資料夾。
   ```
   $ tar –xvf linux-3.10.104.tar.gz –C ~/
   $ cd linux-3.10.104
   ```
3. 重新編譯 kernel。
   ```
   $ sudo make menuconfig    // 使用預設即可
   $ sudo make -j$(nproc)    // 包含make bzImage、make modules...等動作
   $ sudo make modules_install -j$(nproc) 
   $ sudo make install -j$(nproc)        // 安裝kernel
   ```
4. grub
   a. 設定 grub 選單。
   ```
   $ sudo vim /etc/default/grub
   更改以下設定
   GRUB_TIMEOUT=0 -> GRUB_TIMEOUT=-1
   GRUB_TIMEOUT_STYLE=menu
   ```
   b. 更新 grub 選單，並重啟。
   ```
   $ sudo update-grub
   $ sudo reboot
   ```
   c. 重啟後選擇編譯的 kernel 版本

## Project
### Project1
>https://hackmd.io/t2tM_VFoTkyike6zlCmLWA?view
1. 寫一個 system call 從 kernel space 找出指定 process(task) 的 code(text) 段的位址
2. user 段程式利用 system call 傳入 pid 並取得結果放至 user space 的 buffer 上，再輸出至 terminal 上。

## 參考資料
>https://blog.kaibro.tw/2016/11/07/Linux-Kernel%E7%B7%A8%E8%AD%AF-Ubuntu/

