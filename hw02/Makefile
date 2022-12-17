PWD := $(shell pwd)
#KVERSION := $(shell uname -r)
KERNEL_DIR = /usr/src/linux-headers-5.15.0-43-generic #$(KVERSION)

MODULE_NAME = fileIO
obj-m := $(MODULE_NAME).o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
