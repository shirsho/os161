#include <types.h>
#include <kern/errno.h>
#include <kern/limits.h>
#include <kern/unistd.h>
#include <lib.h>
#include <array.h>
#include <thread.h>
#include <synch.h>
#include <pid.h>
#include <curthread.h>


struct pidinfo {
	int pi_pid;			
	int pi_ppid;			
	volatile int pi_exited;		
	int pi_exitstatus;		
	struct cv *pi_cv;		
};



static struct lock *pidlock;		
static struct pidinfo *pidinfo[PROCS_MAX]; 
static pid_t nextpid;			
static int nprocs;			




static
struct pidinfo *
pidinfo_create(pid_t pid, pid_t ppid)
{
	struct pidinfo *pi;

	pi = kmalloc(sizeof(struct pidinfo));
	if (pi==NULL) {
		return NULL;
	}

	pi->pi_cv = cv_create("pidinfo cv");
	if (pi->pi_cv == NULL) {
		kfree(pi);
		return NULL;
	}

	pi->pi_pid = pid;
	pi->pi_ppid = ppid;
	pi->pi_exited = 0;
	pi->pi_exitstatus = 0xbeef;  

	return pi;
}


static
void
pidinfo_destroy(struct pidinfo *pi)
{
	
	cv_destroy(pi->pi_cv);
	kfree(pi);
}


void
pid_bootstrap(void)
{
	int i;

	pidlock = lock_create("pidlock");
	if (pidlock == NULL) {
		panic("Out of memory creating pid lock\n");
	}

	
	for (i=0; i<PROCS_MAX; i++) {
		pidinfo[i] = NULL;
	}

	pidinfo[BOOTUP_PID] = pidinfo_create(BOOTUP_PID, INVALID_PID);
	if (pidinfo[BOOTUP_PID]==NULL) {
		panic("Out of memory creating bootup pid data\n");
	}
        curthread->t_pid = 1;

	nextpid = PID_MIN;
	nprocs = 1;
}


static
struct pidinfo *
pi_get(pid_t pid)
{
	int i;
	for(i = 1; i < PROCS_MAX; i++)
	{
		if(pidinfo[i] != NULL){
		
		if(pidinfo[i]->pi_pid == pid)
		{
			return pidinfo[i];
		}	}
	}
	return NULL;
}


static
void
pi_put(pid_t pid, struct pidinfo *pi)
{
	(void) pid;
        int i;
        for (i= 1; i<PROCS_MAX; i++){

	if(pidinfo[i] == NULL){
	pidinfo[i] = pi;
        break;
}
}


	nprocs++;
}


static
void
pi_drop(pid_t pid)
{
        int i;
        for (i = 1; i<PROCS_MAX; i++){
            if (pidinfo[i] != NULL){
		if (pidinfo[i]->pi_pid == pid){
            pidinfo_destroy(pidinfo[i]);			
}
}
}
	nprocs--;
}


static
void
inc_nextpid(void)
{

	nextpid++;
	if (nextpid > PID_MAX) {
		nextpid = PID_MIN;
	}
}


int
pid_alloc(pid_t *retval)
{
	struct pidinfo *pi;
	pid_t pid;


	
	lock_acquire(pidlock);
        
	if (nprocs == PROCS_MAX) {
		lock_release(pidlock);
		return EAGAIN;
	}

	

	pid = nextpid;

	pi = pidinfo_create(pid, curthread->t_pid);
	if (pi==NULL) {
		lock_release(pidlock);
		return ENOMEM;
	}

	pi_put(pid, pi);

	inc_nextpid();

	lock_release(pidlock);

	*retval = pid;
	return 0;
}


void
pid_unalloc(pid_t theirpid)
{
	struct pidinfo *them;

	

	lock_acquire(pidlock);

	them = pi_get(theirpid);
	

	
	them->pi_exitstatus = 0xdead;
	them->pi_exited = 1;
	them->pi_ppid = INVALID_PID;

	pi_drop(theirpid);

	lock_release(pidlock);
}


void
pid_disown(pid_t theirpid)
{
	struct pidinfo *them;

	

	lock_acquire(pidlock);

	them = pi_get(theirpid);


	them->pi_ppid = INVALID_PID;
	if (them->pi_exited) {
		pi_drop(them->pi_pid);
	}

	lock_release(pidlock);
}


void
pid_setexitstatus(int status)
{
	struct pidinfo *us;
	int i;


	lock_acquire(pidlock);

	
	for (i=0; i<PROCS_MAX; i++) {
		if (pidinfo[i]==NULL) {
			continue;
		}
		if (pidinfo[i]->pi_ppid == curthread->t_pid) {
			pidinfo[i]->pi_ppid = INVALID_PID;
			if (pidinfo[i]->pi_exited) {
				pi_drop(pidinfo[i]->pi_pid);
			}
		}
	}

	
	us = pi_get(curthread->t_pid);

	us->pi_exitstatus = status;
	us->pi_exited = 1;

	if (us->pi_ppid == INVALID_PID) {
		
		pi_drop(curthread->t_pid);
	}
	else {
              
		cv_broadcast(us->pi_cv, pidlock);
	}

	lock_release(pidlock);
}


int
pid_wait(pid_t theirpid, int *status, int flags, pid_t *ret)
{
	struct pidinfo *them;


	
	if (theirpid == curthread->t_pid) {
		return EINVAL;
	}
      
	if (theirpid == INVALID_PID || theirpid<0) {
		return EINVAL;
	}

	
	if (flags != 0 && flags != WNOHANG) {
		return EINVAL;
	}

	lock_acquire(pidlock);

	them = pi_get(theirpid);
	if (them==NULL) {
		lock_release(pidlock);
		return ESRCH;
	}


	
	if (them->pi_ppid != curthread->t_pid) {
		lock_release(pidlock);
		return EPERM;
	}

	if (them->pi_exited==0) {
		if (flags==WNOHANG) {
			lock_release(pidlock);
			*ret = 0;
			return 0;
		}
		
		cv_wait(them->pi_cv, pidlock);
	}

	if (status != NULL) {
		*status = them->pi_exitstatus;
	}
	if (ret != NULL) {
		
		*ret = theirpid;
	}

	them->pi_ppid = 0;
	pi_drop(them->pi_pid);

	lock_release(pidlock);
	return 0;
}
