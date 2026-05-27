# Makefile för out-of-tree kernel modul
#
# användning:
#   make            — bygger .ko filen
#   make clean      — tabort och bygger artifakter
#
# Kräver KDIR görs till kernel källa/build directory:
#   make KDIR=/path/to/kernel-build

obj-m += hello_kernel.o

# Default till Buildroot's kernel build directory om KDIR inte är definerad
KDIR ?= /home/uxaiembed/ws/buildroot/output/build/linux-6.18.7

# Cross-compiler från Buildroot
CROSS_COMPILE ?= /home/uxaiembed/ws/buildroot/output/host/bin/aarch64-buildroot-linux-gnu-
ARCH ?= arm64

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
