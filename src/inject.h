#ifndef __INJECT_H__
#define __INJECT_H__

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>

#include <linux/ptrace.h>

#include "ignotum_ptrace.h"
#include "ignotum_mem.h"

#if defined (__x86_64__) || defined (__i386__)
    #include <sys/reg.h>
    #define BREAKPOINT "\xcc"
    #define BREAKPOINT_LEN 1
    typedef struct user_regs_struct regs_t;
#endif

#ifdef __x86_64__
    #define instruction_point rip
    #define IP RIP
#elif __i386__
    #define instruction_point eip
    #define IP EIP
#elif __arm__
    #define instruction_point regs.ARM_pc
    #define IP 15 /* PC register */
    #define BREAKPOINT "\xe7\xf0\x01\xf0"
    #define BREAKPOINT_LEN 4
    typedef struct user regs_t;
#else
    #error unsupported architeture
#endif

#define default_inject (inject_t){ 1, 1, 0 }

typedef ssize_t (*writecb)(int, const void *, size_t, off_t);
typedef ssize_t (*readcb)(int, void *, size_t, off_t);

extern writecb memwrite;
extern readcb memread;

typedef struct {
    int restore;
    int restore_ip;
    pid_t pid;
} inject_t;


void ps_inject(const char *sc, size_t len, inject_t *options);


#endif
