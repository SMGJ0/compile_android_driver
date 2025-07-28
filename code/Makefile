# 内核源码路径（不要加 /out！）
KERNEL_DIR = /home/ubunto/k50rubens

# 编译选项
ARCH = arm64
CROSS_COMPILE = $(HOME)/GCC64-4.9/bin/aarch64-linux-android-
LLVM = 1
CLANG_TRIPLE = aarch64-linux-gnu-

# 模块名称（test.c -> test.o -> test.ko）
obj-m := test.o

# 编译命令
all:
	$(MAKE) -C $(KERNEL_DIR) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		LLVM=$(LLVM) \
		CLANG_TRIPLE=$(CLANG_TRIPLE) \
		M=$(PWD) \
		modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
