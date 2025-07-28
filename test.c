#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>  // 用于查找 sys_call_table

// 原始 sys_call_table 地址（需动态获取）
static unsigned long *sys_call_table = NULL;

// 原始 open 系统调用函数指针
asmlinkage long (*orig_open)(const char __user *filename, int flags, umode_t mode);

// 自定义 Hook 函数
asmlinkage long my_open(const char __user *filename, int flags, umode_t mode) {
    printk(KERN_INFO "[Hook] Open called: %s\n", filename);
    return orig_open(filename, flags, mode);
}

// 修改内存页权限（绕过写保护）
static void disable_page_protection(void) {
    write_cr0(read_cr0() & (~0x10000));  // 清除 CR0.WP 位（x86 架构需调整）
}

static void enable_page_protection(void) {
    write_cr0(read_cr0() | 0x10000);
}

// 初始化模块
static int __init hook_init(void) {
    // 1. 获取 sys_call_table 地址（需 root 权限）
    sys_call_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
    if (!sys_call_table) {
        printk(KERN_ALERT "Failed to find sys_call_table!\n");
        return -1;
    }

    // 2. 保存原始 open 系统调用
    orig_open = (void *)sys_call_table[__NR_open];

    // 3. 修改内存权限并替换 sys_call_table 项
    disable_page_protection();
    sys_call_table[__NR_open] = (unsigned long)my_open;
    enable_page_protection();

    printk(KERN_INFO "Syscall hook installed!\n");
    return 0;
}

// 卸载模块
static void __exit hook_exit(void) {
    if (sys_call_table && orig_open) {
        disable_page_protection();
        sys_call_table[__NR_open] = (unsigned long)orig_open;
        enable_page_protection();
    }
    printk(KERN_INFO "Syscall hook removed.\n");
}

module_init(hook_init);
module_exit(hook_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourName");
