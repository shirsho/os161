/*
 * catlock.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use LOCKS/CV'S to solve the cat syncronization problem in 
 * this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>


/*
 * 
 * Constants
 *
 */

/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2


/*
 * 
 * Function Definitions
 * 
 */
 
 
 static struct lock * bowl1_lock;
 static struct lock * bowl2_lock;
 static struct cv* cat_eating;
 static struct cv* mouse_eating;
 static struct lock * cat_appear;
 static struct lock * mouse_appear;

/* who should be "cat" or "mouse" */
static void
lock_eat(const char *who, int num, int bowl, int iteration)
{
        kprintf("%s: %d starts eating: bowl %d, iteration %d\n", who, num, 
                bowl, iteration);
        clocksleep(1);
        kprintf("%s: %d ends eating: bowl %d, iteration %d\n", who, num, 
                bowl, iteration);
}

/*
 * catlock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS -
 *      1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
catlock(void * unusedpointer, 
        unsigned long catnumber)
{
		const char *cat = "cat";
		int n =0;
	(void)	unusedpointer;
    for (n = 0; n<4; n++){
		while(lock_do_i_hold(mouse_appear)!=0){
			cv_wait(cat_eating,cat_appear);}
	    
		
	/*	if(lock_do_i_hold(mouse_appear) == 0){ 
		cv_signal(cat_eating,cat_appear);}
     */   
		if (lock_do_i_hold(mouse_appear) == 0){
		if (lock_do_i_hold(bowl1_lock) == 0){
		lock_acquire(bowl1_lock);
		if(lock_do_i_hold(cat_appear) == 0){
		lock_acquire(cat_appear);}
		lock_eat(cat,catnumber,1,n);
		if (lock_do_i_hold(bowl2_lock) == 0){
		lock_release(cat_appear);
		lock_release(bowl1_lock);
		cv_signal(mouse_eating,mouse_appear);
		//cv_broadcast(cat_eating,cat_appear);
		
		}
		else if(lock_do_i_hold(bowl2_lock)!= 0){
		//lock_release(cat_appear);
		lock_release(bowl1_lock);
		cv_wait(cat_eating,cat_appear);
		}
		}
		
		else if (lock_do_i_hold(bowl2_lock) == 0){
		
		lock_acquire(bowl2_lock);
		if(lock_do_i_hold(cat_appear) == 0){
		lock_acquire(cat_appear);}
		lock_eat(cat,catnumber,2,n);
		if (lock_do_i_hold(bowl1_lock) == 0){
		lock_release(cat_appear);
		lock_release(bowl2_lock);
		//cv_broadcast(cat_eating,cat_appear);
		cv_signal(mouse_eating,mouse_appear);
		
		}
		else if(lock_do_i_hold(bowl1_lock)!= 0){
		//lock_release(cat_appear);
		lock_release(bowl2_lock);
		cv_wait(cat_eating,cat_appear);
		
		}
		}
		}}
		
		
		
		

}
	

/*
 * mouselock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to 
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
mouselock(void * unusedpointer,
          unsigned long mousenumber)
{
		(void)	unusedpointer;
       const char *mouse = "mouse";
		int n = 0;
		for (n = 0; n<4; n++){
	   while(lock_do_i_hold(cat_appear)!=0){
			cv_wait(mouse_eating,mouse_appear);}
	
		/*if(lock_do_i_hold(cat_appear) == 0){ 
		cv_signal(mouse_eating,mouse_appear);
		}*/
		
		if(lock_do_i_hold(cat_appear) == 0){
		
		if (lock_do_i_hold(bowl1_lock) == 0){
		
		lock_acquire(bowl1_lock);

		if(lock_do_i_hold(mouse_appear) == 0){
		lock_acquire(mouse_appear);}
		lock_eat(mouse,mousenumber,1,n);
		
		if (lock_do_i_hold(bowl2_lock) == 0){
		lock_release(mouse_appear);
		lock_release(bowl1_lock);
		cv_signal(cat_eating,cat_appear);
		}
		else if(lock_do_i_hold(bowl2_lock)!= 0){
		//lock_release(mouse_appear);
		lock_release(bowl1_lock);
		cv_wait(mouse_eating,mouse_appear);
		
		}
		
		}
		
		else if (lock_do_i_hold(bowl2_lock) == 0){
		
		lock_acquire(bowl2_lock);
		if(lock_do_i_hold(mouse_appear) == 0){
		lock_acquire(mouse_appear);}
		lock_eat(mouse,mousenumber,2,n);
		
	    if (lock_do_i_hold(bowl1_lock) == 0){
		lock_release(mouse_appear);
		lock_release(bowl2_lock);
		cv_signal(cat_eating,cat_appear);
		
		}
		else if(lock_do_i_hold(bowl1_lock)!= 0){
		//lock_release(mouse_appear);
		lock_release(bowl2_lock);
		cv_wait(mouse_eating,mouse_appear);
		}
		
		
		}
		}}
	    
		
}


/*
 * catmouselock()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catlock() and mouselock() threads.  Change
 *      this code as necessary for your solution.
 */

int
catmouselock(int nargs,
             char ** args)
{
        int index, error;
   
        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;
		
		if (bowl1_lock == NULL) {
		bowl1_lock = lock_create("bowl1_lock");
			if (bowl1_lock == NULL) {
			panic("synchtest: lock_create failed\n");
		}
		}
		if (bowl2_lock == NULL) {
		bowl2_lock = lock_create("bowl2_lock");
			if (bowl2_lock == NULL) {
			panic("synchtest: lock_create failed\n");
		}
		}
		if (cat_eating == NULL) {
		cat_eating = cv_create("cat_eating");
			if (cat_eating == NULL) {
			panic("synchtest: cv_create failed\n");
		}
		}
		if (mouse_eating == NULL) {
		mouse_eating = cv_create("mouse_eating");
			if (mouse_eating == NULL) {
			panic("synchtest: cv_create failed\n");
		}
		}
		if (cat_appear == NULL) {
		cat_appear = lock_create("cat_appear");
			if (cat_appear == NULL) {
			panic("synchtest: lock_create failed\n");
		}
		}
		if (mouse_appear == NULL) {
		mouse_appear = lock_create("mouse_appear");
			if (mouse_appear == NULL) {
			panic("synchtest: lock_create failed\n");
		}
		}
		
        /*
         * Start NCATS catlock() threads.
         */

        for (index = 0; index < NCATS; index++) {
           
                error = thread_fork("catlock thread", 
                                    NULL, 
                                    index, 
                                    catlock, 
                                    NULL
                                    );
                
                /*
                 * panic() on error.
                 */

                if (error) {
                 
                        panic("catlock: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        /*
         * Start NMICE mouselock() threads.
         */

        for (index = 0; index < NMICE; index++) {
   
                error = thread_fork("mouselock thread", 
                                    NULL, 
                                    index, 
                                    mouselock, 
                                    NULL
                                    );
      
                /*
                 * panic() on error.
                 */

                if (error) {
         
                        panic("mouselock: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        return 0;
}

/*
 * End of catlock.c
 */
