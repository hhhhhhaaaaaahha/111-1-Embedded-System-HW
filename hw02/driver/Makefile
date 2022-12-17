# Makefile for kernel
CONFIG_MODULE_SIG=n
PWD := $(shell pwd)
KVERSION := $(shell uname -r)
KERNEL_DIR = /lib/modules/5.15.64/build #/usr/src/linux-headers-$(KVERSION)

MODULE_NAME = char_dev
obj-m := $(MODULE_NAME).o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
