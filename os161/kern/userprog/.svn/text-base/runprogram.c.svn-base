/*
 * Sample/test code for running a user program.  You can use this for
 * reference when implementing the execv() system call. Remember though
 * that execv() needs to do more than this function does.
 */

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */
int
runprogram(char *progname, char **argv)
{
	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;



	/* get argc*/
	int argc=0;
	if (argv!=NULL)
	{
		while(argv[argc]!=NULL)
			argc++;
	}
	int i;
	//for (i=0;i<argc;i++)
	//	kprintf("0x%lx : argv[%d]: %s\n",argv[i],i,argv[i]);
	//kprintf("argc: %d\n",argc);


	/* Open the file. */
	result = vfs_open(progname, O_RDONLY, &v);
	if (result) {
		return result;
	}

	/* We should be a new thread. */
	assert(curthread->t_vmspace == NULL);

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}

	/* copyout argv into userstack */
	int len;
	int offset;
	size_t actual;
	vaddr_t argv_addr[argc+1];
	if (argv!=NULL)
	{
		
		for(i=argc-1;i>=0;i--)
		{
			len = strlen(argv[i]) + 1;
			offset = len % 4;
			stackptr -= len + (4 - offset);
			result = copyoutstr( argv[i],(userptr_t) stackptr,(size_t)len,&actual);
			if (result)
			{
				kprintf("argv coyout NOT OK:\n");
				return result;
			}
			argv_addr[i] = stackptr;
		}
		argv_addr[argc] = 0;
		
		//for loop copyout argv_addr the same way
		for(i=argc;i>=0;i--)
		{
			stackptr -= sizeof(size_t);
			result = copyout( &argv_addr[i],(userptr_t) stackptr,sizeof(vaddr_t));
			if (result)
			{
				kprintf("argv_addr coyout NOT OK:\n");
				return result;
			}
		}
	}
	//kprintf("coyout OK:\n");
	
	/* Warp to user mode. */
	md_usermode(argc, (userptr_t) stackptr,stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}

