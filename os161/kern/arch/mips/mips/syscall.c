#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <machine/trapframe.h>
#include <kern/callno.h>
#include <syscall.h>
#include <curthread.h>
#include <addrspace.h>
#include <thread.h>
#include <pid.h>


/*
 * System call handler.
 *
 * A pointer to the trapframe created during exception entry (in
 * exception.S) is passed in.
 *
 * The calling conventions for syscalls are as follows: Like ordinary
 * function calls, the first 4 32-bit arguments are passed in the 4
 * argument registers a0-a3. In addition, the system call number is
 * passed in the v0 register.
 *
 * On successful return, the return value is passed back in the v0
 * register, like an ordinary function call, and the a3 register is
 * also set to 0 to indicate success.
 *
 * On an error return, the error code is passed back in the v0
 * register, and the a3 register is set to 1 to indicate failure.
 * (Userlevel code takes care of storing the error code in errno and
 * returning the value -1 from the actual userlevel syscall function.
 * See src/lib/libc/syscalls.S and related files.)
 *
 * Upon syscall return the program counter stored in the trapframe
 * must be incremented by one instruction; otherwise the exception
 * return code will restart the "syscall" instruction and the system
 * call will repeat forever.
 *
 * Since none of the OS/161 system calls have more than 4 arguments,
 * there should be no need to fetch additional arguments from the
 * user-level stack.
 *
 * Watch out: if you make system calls that have 64-bit quantities as
 * arguments, they will get passed in pairs of registers, and not
 * necessarily in the way you expect. We recommend you don't do it.
 * (In fact, we recommend you don't use 64-bit quantities at all. See
 * arch/mips/include/types.h.)
 */

void
mips_syscall(struct trapframe *tf)
{
	int callno;
	int32_t retval;
	int err;

	assert(curspl==0);

	callno = tf->tf_v0;


	retval = 0;

	switch (callno) {
	    case SYS_reboot:
		err = sys_reboot(tf->tf_a0);
		break;

	 case SYS__exit:
	      sys_exit(tf->tf_a0);
	      panic("Returning from exit\n");
		break;
	    case SYS_execv:
	     // err = sys_execv((userptr_t)tf->tf_a0, (userptr_t)tf->tf_a1);
	      break;
	    case SYS_fork:
	      retval = sys_fork(tf);
		if (retval < 0)
			err = -retval;
		else
			err = 0;
	      break;
	    case SYS_waitpid:

	      err = sys_waitpid(tf->tf_a0, (userptr_t)tf->tf_a1, tf->tf_a2, 
		  &retval);
	      break;
	    case SYS_getpid:
	      err = sys_getpid(&retval);
	      break;
	      case SYS_write:
	      err = sys_write(tf->tf_a0,(char*)tf->tf_a1,(size_t)tf->tf_a2, 
          &retval );
              break;
              case SYS_read:
	      err = sys_read(tf->tf_a0,(char*)tf->tf_a1,(size_t)tf->tf_a2, 
          &retval);
              break;
             
	    default:
		kprintf("Unknown syscall %d\n", callno);
		err = ENOSYS;
		break;
	}


	if (err) {
		/*
		 * Return the error code. This gets converted at
		 * userlevel to a return value of -1 and the error
		 * code in errno.
		 */
		tf->tf_v0 = err;
		tf->tf_a3 = 1;      /* signal an error */
	}
	else {
		/* Success. */
		tf->tf_v0 = retval;
		tf->tf_a3 = 0;      /* signal no error */
	}
	
	/*
	 * Now, advance the program counter, to avoid restarting
	 * the syscall over and over again.
	 */
	
	tf->tf_epc += 4;

	/* Make sure the syscall code didn't forget to lower spl */
	assert(curspl==0);
}

void
md_forkentry(struct trapframe *tf,unsigned long as_data)
{
	struct trapframe tf_local;
       struct trapframe *newtf; 
       struct addrspace *newas = (struct addrspace*) as_data;
       tf_local = *tf;
       newtf = &tf_local;

       newtf->tf_v0 = 0;
       newtf->tf_a3 = 0;
       newtf->tf_epc += 4;
       kfree(tf);
       curthread->t_vmspace = newas;

       as_activate(curthread->t_vmspace);
       
  

mips_usermode(&tf_local);
}

int sys_write(int fd, char *buf, size_t nbytes, int *retval)
{
       size_t bytes_written=0;       
        if( fd == 1) 
        {
                size_t i = 0;
                while( i < nbytes)
                {
                        if(sizeof(buf[i]) == sizeof(int) )
                                kprintf("%d", buf[i]  );
                        else            
                                kprintf("%c",buf[i]);
                      
                        i++;
                }
                bytes_written = i;
        }    
        *retval = bytes_written;
        return 0;

	
}

int sys_read(int fd,char * buf,size_t buflen, int* retval){

 if (buf == NULL || retval == NULL){
          return EINVAL;
}
       char * kbuffer = (char*) kmalloc(buflen);
	size_t bytes_read = 0;
          if(fd==0)
        {
                char ch = (char) getch();
                kbuffer[bytes_read] = ch;
                bytes_read++;           

                int result = copyout( (const void *)kbuffer, (userptr_t)buf, buflen);
                if( result != 0 ) 
                        return result;  
        }
        else{
                kprintf("ERROR: fd does not equal 0. It is: %d\n",fd);
        }

        kfree(kbuffer);
        *retval = bytes_read;

        return 0;

	
}

int sys_fork(struct trapframe *tf)
{
         struct thread* newthread;
         struct trapframe* newtf; 
         struct addrspace* newas; 
         int result;
     
         
       	 newtf = (struct trapframe*) kmalloc(sizeof(struct trapframe));
	 if (newtf == NULL){
          return  ENOMEM;
} 
        *newtf = *tf;

        int err;

        err= as_copy(curthread->t_vmspace,&newas);

        if (err){
        return ENOMEM;
}
        as_activate(curthread->t_vmspace);
      
         result = thread_fork(curthread->t_name, newtf,(unsigned long)newas,
                 (void (*)(void *, unsigned long)) md_forkentry,&newthread);

	
         if (result) 
         	return -ENOMEM;
         return newthread->t_pid;
}

int
sys_getpid(pid_t *retval)
{
	*retval = curthread->t_pid;
	return 0;
}

void
sys_exit(int status)
{
        pid_setexitstatus(status);
	thread_exit();
}

int
sys_waitpid(pid_t pid, userptr_t retstatus, int flags, pid_t *retval)
{
	int status; 
	int result;

	result = pid_wait(pid, &status, flags, retval);
	if (result) {
		return result;
	}
  status = _MKWAIT_EXIT(status);	
	return copyout(&status, retstatus, sizeof(int));
}
