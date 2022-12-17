#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>


MODULE_DESCRIPTION("hello_world");
MODULE_LICENSE("GPL");

static int file_init(void)
{
	static char arr[262];
	static char arr2[262];
	loff_t pos = 0;
	int i;
	struct file *cfile;
	struct file *cfile2;
	cfile = filp_open("./input.txt", O_RDONLY, 0);
	if(IS_ERR(cfile)){
		printk(KERN_INFO "File open failed !\n");
	}
	
	kernel_read(cfile, arr, sizeof(arr), &pos);
	cfile2 = filp_open("/home/ray/Desktop/111-1-Embedded-System-HW/hw01/part_II/output.txt", O_CREAT|O_RDWR, 0644);
	pos = 0;
	for(i=0;i<strlen(arr);i++){
		arr2[i] = arr[strlen(arr)-i-1];
	}
	kernel_write(cfile2, arr2, strlen(arr2), &pos);
	
	filp_close(cfile, NULL);
	filp_close(cfile2, NULL);
	return 0;
}

static void file_exit(void)
{
	printk(KERN_INFO "Bye !\n");
}

module_init(file_init);
module_exit(file_exit);
