#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <asm/ioctl.h>

#define MY_CDEB_MAJOR 45		// 定義 driver 的 major 編號
#define MY_CDEV_NAME "ray_char" // 定義 driver 的名字
#define MY_MAX_MINORS 5			// 定義 driver 可以被開啟的 minor number
#define MY_SIZE 8192			// 定義 buffer size

MODULE_LICENSE("Dual BSD/GPL");										 // kernel module 的 license
MODULE_DESCRIPTION("Embedded System HW02 Character Device Driver."); // kernel module 的 description
MODULE_AUTHOR("Chen-Jui Tu<raytu.tpe@gmail.com>");					 // kernel module 的 作者資訊

// 宣告一個可以儲存 character device 以及 kernel 裡的 buffer 的 structure
struct my_device_data
{
	struct cdev cdev;
	char *buffer;
};

struct my_device_data devs[MY_MAX_MINORS]; // 最多可能有 MY_MAX_MINORS 個 cdev 被開啟
										   // 因此先宣告一個大小為 MY_MAX_MINORS 的 array

// 實作 open 的操作
static int ray_open(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data;										// 宣告一個 my_device_data 物件
	my_data = container_of(inode->i_cdev, struct my_device_data, cdev); // 從 inode 中取得 cdev 並存在剛剛宣告的物件中
	file->private_data = my_data;										// 將剛才的物件存到 open 的 file 中
	my_data->buffer = (char *)kmalloc(MY_SIZE, GFP_KERNEL);				// 分配空間給 kernel buffer

	return 0;
}

// 實作 read 的操作
static ssize_t ray_read(struct file *file, char __user *user_buffer,
						size_t size, loff_t *offset)
{
	*offset = 0;																  // 將 offset 初始化為 0，才可以從 kernel buffer 的開頭開始讀
	struct my_device_data *my_data = (struct my_device_data *)file->private_data; // 透過 file 引數獲取 open 時的 cdev 物件
	ssize_t len = min(MY_SIZE - *offset, size);									  // 計算可讀取的長度

	if (len <= 0)
		return 0; // 若不超過0，則直接回傳

	if (copy_to_user(user_buffer, my_data->buffer + *offset, len))
		return -EFAULT; // 其餘情況則將 kernel buffer 裡的值，根據長度及 offset 數值寫回到 user buffer 裡

	*offset += len; // 透過 offset 來記錄讀取的長度
	return len;
}

// 實作 write 的操作
static ssize_t ray_write(struct file *file, const char __user *user_buffer,
						 size_t size, loff_t *offset)
{
	*offset = 0;																  // 將 offset 初始化為 0，才可以從 kernel buffer 的開頭開始寫
	struct my_device_data *my_data = (struct my_device_data *)file->private_data; // 透過 file 引數獲取 open 時的 cdev 物件
	ssize_t len = min(MY_SIZE - *offset, size);									  // 計算可寫入的長度

	if (len <= 0)
		return 0; // 若不超過0，則直接回傳

	if (copy_from_user(my_data->buffer + *offset, user_buffer, len))
		return -EFAULT; // 其餘情況則將 user buffer 裡的值，根據長度及 offset 數值寫到 kernel buffer 裡

	*offset += len; // 透過 offset 來記錄寫入的長度
	return len;
}

// 實作 release 的操作
static int ray_release(struct inode *inode, struct file *filp)
{
	struct my_device_data *my_data = (struct my_device_data *)filp->private_data; // 透過 file 引數獲取 open 時的 cdev 物件
	printk("Check buffer: %s\n", my_data->buffer);								  // 查看 release 前 buffer 的值
	kfree(my_data->buffer);														  // free 掉先前配置給 kernel buffer 的空間
	printk("Check buffer: %s\n", my_data->buffer);								  // 檢查 release 後 buffer 的值
	return 0;
}

// 實作這個 character device 的 file operation（將前面定義過的 function assign 進來）
const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = ray_open,
	.read = ray_read,
	.write = ray_write,
	.release = ray_release,
};

// kernel module 的 init function
static __init int ray_char_init(void)
{
	int i, err;
	err = register_chrdev_region(MKDEV(MY_CDEB_MAJOR, 0), MY_MAX_MINORS, MY_CDEV_NAME); // 向 kernel 註冊 character device
	if (err != 0)
	{
		return err; // 回報錯誤
	}
	for (i = 0; i < MY_MAX_MINORS; i++) // 遍歷所有 devs array 中的　my_device_data 物件
	{
		cdev_init(&devs[i].cdev, &my_fops);					 // 將 my_device_data 中的 cdev 初始化
		cdev_add(&devs[i].cdev, MKDEV(MY_CDEB_MAJOR, i), 1); // 將初始化後的 cdev 根據 major number 與 minors number 進行註冊
	}
	printk(KERN_INFO "Hello cdev! \n"); // 在 kernel log 印出 init 的訊息
	return 0;
}

// kernel module 的 exit function
static void __exit ray_char_exit(void)
{
	int i;
	printk(KERN_INFO "Goodbye cdev! \n"); // 在 kernel log 印出 exit 的訊息

	for (i = 0; i < MY_MAX_MINORS; i++) // 遍歷所有 devs array 中的　my_device_data structure
	{
		cdev_del(&devs[i].cdev); // 刪除所有 cdev
	}
	unregister_chrdev_region(MKDEV(MY_CDEB_MAJOR, 0), MY_MAX_MINORS); // 解除 driver major number 與 minor number 的註冊
}

module_init(ray_char_init); // kernel module init
module_exit(ray_char_exit); // kernel module exit