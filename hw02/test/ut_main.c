#include <linux/kernel.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BUFFER_SIZE 100 // 定義 buffer 大小

int main(void)
{

    char test[BUFFER_SIZE] = "I love Artoria <3"; // 要寫進 character device 的第一個測試資料
    char test2[BUFFER_SIZE] = "Excalibur! ";      // 要寫進 character device 的第二個測試資料
    char response[BUFFER_SIZE];                   // 從 character device 中讀取出來的空間 1
    char response2[BUFFER_SIZE];                  // 從 character device 中讀取出來的空間 2
    char fd[5];                                   // 開啟 character device 的 char
    int i;                                        // 宣告變數給後面要開啟 character device 的 for 迴圈使用

    system("sudo rm -rf /dev/mycdev_*");        // 將之前建立過的 character device 刪掉
    system("sudo mknod /dev/mycdev_1 c 45 0"); // 建立一個 device
    fd[0] = open("/dev/mycdev_1", O_RDWR);     // 開啟他
    system("sudo mknod /dev/mycdev_2 c 45 1"); // 建立第二個 device
    fd[1] = open("/dev/mycdev_2", O_RDWR);     // 開啟他
    system("sudo mknod /dev/mycdev_3 c 45 2"); // 建立第三個 device
    fd[2] = open("/dev/mycdev_3", O_RDWR);     // 開啟他
    system("sudo mknod /dev/mycdev_4 c 45 3"); // 建立第四個 device
    fd[3] = open("/dev/mycdev_4", O_RDWR);     // 開啟他
    system("sudo mknod /dev/mycdev_5 c 45 4"); // 建立第五個 device
    fd[4] = open("/dev/mycdev_5", O_RDWR);     // 開啟他

    // 檢查是否開啟成功
    for (i = 0; i < 5; i++)
    {
        if (*(fd + i) < 0)
        {
            /* handle error */
            printf("FD[%hhd] not open\n", i);
        }
    }

    write(fd[0], test, strlen(test) + 1);      // 將測資 1 寫進 device1
    write(fd[1], test2, strlen(test2) + 1);    // 將測資 2 寫進 device2
    read(fd[0], response, strlen(test) + 1);   // 將從 device1 讀取出來的值放入 response 中
    read(fd[1], response2, strlen(test2) + 1); // 將從 device2 讀取出來的值放入 response2 中
    printf("%s\n", response);                  // 印出 response 的值
    printf("%s\n", response2);                 // 印出 response2 的值

    // 關閉所有 device
    for (i = 0; i < 5; i++)
        close(fd[i]);
    return 0;
}
