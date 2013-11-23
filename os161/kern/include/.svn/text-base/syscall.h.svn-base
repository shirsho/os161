#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);

int sys_fork(struct trapframe *tf);



void sys_exit(int code);
//int sys_execv(userptr_t prog, userptr_t args);
int sys_waitpid(pid_t pid, userptr_t returncode, int flags, pid_t *retval);
int sys_getpid(pid_t *retval);
int sys_write(int fd, char *buf, size_t nbytes, int *retval);
int sys_read(int fd,char * buf,size_t buflen, int* retval);
#endif /* _SYSCALL_H_ */
