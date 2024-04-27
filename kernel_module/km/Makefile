ifneq ($(KERNELRELEASE),)
        obj-m := Laserkeyboard.o 
else
        KERNELDIR := /usr/src/linux-headers-5.10.168-ti-r72/ 
        PWD := $(shell pwd)
	ARCH := arm

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) modules

clean:

	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) clean

endif

