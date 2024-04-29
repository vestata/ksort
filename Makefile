obj-m += sort.o
sort-objs := \
    sort_mod.o \
    sort_impl.o

obj-m += xoro.o
xoro-objs := \
    xoro_mod.o

CFLAGS = -Wall -Werror

GIT_HOOKS := .git/hooks/applied

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: $(GIT_HOOKS) user test_xoro
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

user: user.c
	$(CC) $(CFLAGS) -o $@ $^

test_xoro: test_xoro.c
	$(CC) $(CFLAGS) -o $@ $^

insmod: all rmmod
	sudo insmod sort.ko
	sudo insmod xoro.ko

rmmod:
	@sudo rmmod sort 2>/dev/null || echo
	@sudo rmmod xoro 2>/dev/null || echo

check: all
	sudo dmesg -C
	$(MAKE) insmod
	sudo ./user
	sudo ./test_xoro
	$(MAKE) rmmod
	sudo dmesg | grep "Sorting took" | awk '{print $$(NF-1)}' >> out.csv

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) user test_xoro
