PWD := $(shell pwd)
#KVERSION := $(shell uname -r)
KERNEL_DIR = /lib/modules/5.15.64/build/ #/usr/src/$(KVERSION)

MODULE_NAME = testIO
obj-m := $(MODULE_NAME).o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f output.txt
