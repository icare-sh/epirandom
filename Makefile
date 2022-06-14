greentext = "\e[32m$(1)\e[0m"
yellowtext = "\e[33m$(1)\e[0m"
redtext = "\e[31m$(1)\e[0m"
bluetext = "\e[34m$(1)\e[0m"

ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)
MAJOR := .major

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $@

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $@

check:
	@sudo dmesg -C
	@sudo insmod epirandom.ko
	@echo $(call greentext, "Checking if epirandom is loaded...\n\n")
	@touch $(PWD)/$(MAJOR)
	@echo "sudo mknod /dev/epirandom c \\"> $(PWD)/$(MAJOR)
	@sudo dmesg | grep "epirandom: major" | cut -d '"' -f 2 >>  $(MAJOR)
	@echo 0 >> $(PWD)/$(MAJOR)
	@sudo dmesg -T | grep epirandom
	@$(shell bash $(PWD)/$(MAJOR))


nod:
	@touch $(PWD)/$(MAJOR)
	@echo "sudo mknod /dev/epirandom c \\"> $(PWD)/$(MAJOR)
	@sudo dmesg | grep "epirandom: major" | cut -d '"' -f 2 >>  $(MAJOR)
	@echo 0 >> $(PWD)/$(MAJOR)
	@sudo dmesg -T | grep epirandom
	@echo $(call yellowtext, "\n\nRun: bash $(MAJOR)")

clean:
	$(RM) *.o *~ core .depend .*.cmd *.ko *.mod.c *.tmp_versions *.mod *.order *.symvers $(MAJOR)
	@-sudo rmmod epirandom
	@-sudo $(RM) /dev/epirandom
	@-sudo dmesg -T | grep epirandom
	@sudo dmesg -C
	@-echo $(call redtext, "\n\nEpirandom removed.")

.PHONY: modules modules_install clean check

else

obj-m := epirandom.o
endif
