ifneq ($(KERNELRELEASE),)
	obj-m := hellopa.o
else
	KERNELDIR ?= /lib/modules/2.6.18/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
