/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
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
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */

struct semaphore *bigsem;
struct semaphore *NW;
struct semaphore *SW;
struct semaphore *NE;
struct semaphore *SE;

static const char *directions[] = { "N", "W", "S", "E" };

static const char *msgs[] = {
        "approaching:",
        "region1:    ",
        "region2:    ",
        "region3:    ",
        "leaving:    "
};

/* use these constants for the first parameter of message */
enum { APPROACHING, REGION1, REGION2, REGION3, LEAVING };

static void
message(int msg_nr, int carnumber, int cardirection, int destdirection)
{
        kprintf("%s car = %2d, direction = %s, destination = %s\n",
                msgs[msg_nr], carnumber,
                directions[cardirection], directions[destdirection]);
}
 
/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
        P(bigsem);
        
        if(cardirection == 0)
        {
        	P(SW);
        	P(NW);
        	message(APPROACHING, carnumber, cardirection, 2);
        	message(REGION1, carnumber, cardirection, 2);
        	V(NW);
       		message(REGION2, carnumber, cardirection, 2);
       		message(LEAVING, carnumber, cardirection, 2);
       		V(SW);
        }
        else if(cardirection == 1)
        {
        	P(SE);
        	P(SW);
        	message(APPROACHING, carnumber, cardirection, 3);
        	message(REGION1, carnumber, cardirection, 3);
        	V(SW);
       		message(REGION2, carnumber, cardirection, 3);
       		message(LEAVING, carnumber, cardirection, 3);
       		V(SE);
        }
        else if(cardirection == 2)
        {
       		P(NE);
        	P(SE);
        	message(APPROACHING, carnumber, cardirection, 0);
        	message(REGION1, carnumber, cardirection, 0);
        	V(SE);
       		
       		message(REGION2, carnumber, cardirection, 0);
       		message(LEAVING, carnumber, cardirection, 0);
       		V(NE);
        }
        else if(cardirection == 3)
        {
        	
       		P(NW);
        	P(NE);
        	message(APPROACHING, carnumber, cardirection, 1);
        	message(REGION1, carnumber, cardirection, 1);
        	V(NE);
       		
       		message(REGION2, carnumber, cardirection, 1);
       		message(LEAVING, carnumber, cardirection, 1);
       		V(NW);
        }   
        V(bigsem);
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
         P(bigsem);
         if(cardirection == 0)
        {
        	P(SE);
        	P(SW);
        	P(NW);
        	message(APPROACHING, carnumber, cardirection, 3);
        	message(REGION1, carnumber, cardirection, 3);
        	V(NW);
       		
       		
       		message(REGION2, carnumber, cardirection, 3);
       		V(SW);
       		
       		
       		message(REGION3, carnumber, cardirection, 3);
       		message(LEAVING, carnumber, cardirection, 3);
       		V(SE);
        }
        else if(cardirection == 1)
        {
        	P(NE);
        	P(SE);
        	P(SW);
        	message(APPROACHING, carnumber, cardirection, 0);
        	message(REGION1, carnumber, cardirection, 0);
        	V(SW);
       		
       		
       		message(REGION2, carnumber, cardirection, 0);
       		V(SE);
       		
       		
       		message(REGION3, carnumber, cardirection, 0);
       		message(LEAVING, carnumber, cardirection, 0);
       		V(NE);
        }
        else if(cardirection == 2)
        {
        	P(NW);
        	P(NE);
        	P(SE);
        	message(APPROACHING, carnumber, cardirection, 1);
        	message(REGION1, carnumber, cardirection, 1);
        	V(SE);
       		
       		
       		message(REGION2, carnumber, cardirection, 1);
       		V(NE);
       		
       		
       		message(REGION3, carnumber, cardirection, 1);
       		message(LEAVING, carnumber, cardirection, 1);
       		V(NW);
        }
        else if(cardirection == 3)
        {
        	P(SW);
        	P(NW);
        	P(NE);
        	message(APPROACHING, carnumber, cardirection, 2);
        	message(REGION1, carnumber, cardirection, 2);
        	V(NE);
       		
       		
       		message(REGION2, carnumber, cardirection, 2);
       		V(NW);
       		
       		message(REGION3, carnumber, cardirection, 2);
       		message(LEAVING, carnumber, cardirection, 2);
       		V(SW);
        }   
	V(bigsem);
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
	P(bigsem);
        if(cardirection == 0)
        {
        	P(NW);
        	message(APPROACHING, carnumber, cardirection, 1);
        	message(REGION1, carnumber, cardirection, 1);
        	message(LEAVING, carnumber, cardirection, 1);
       		V(NW);
        }
        else if(cardirection == 1)
        {
        	P(SW);
        	message(APPROACHING, carnumber, cardirection, 2);
        	message(REGION1, carnumber, cardirection, 2);
        	message(LEAVING, carnumber, cardirection, 2);
        
       		V(SW);
        }
        else if(cardirection == 2)
        {
        	P(SE);
        	message(APPROACHING, carnumber, cardirection, 3);
        	message(REGION1, carnumber, cardirection, 3);
        	message(LEAVING, carnumber, cardirection, 3);
       		V(SE);
        }
        else if(cardirection == 3)
        {
        	P(NE);
        	message(APPROACHING, carnumber, cardirection, 0);
        	message(REGION1, carnumber, cardirection, 0);
        	message(LEAVING, carnumber, cardirection, 0);
       		V(NE);
        }
        V(bigsem);
        
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */

static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection;
        int cardestination;

        /*
         * Avoid unused variable and function warnings.
         */

        (void) unusedpointer;
        //(void) carnumber;
	//(void) gostraight;
	//(void) turnleft;
	//(void) turnright;

        /*
         * cardirection is set randomly.
         */
	// where it is coming/arriving from
        cardirection = random() % 4;
        cardestination = random() % 4;
        
        while(cardestination == cardirection) // keep getting new destinations until its not the same as direction (from which it arrives)
        	cardestination = random() % 4;
        // Complete turn
        int turn_val = cardestination - cardirection;
        /*
        if(cardirection == 0)
        {
        	if(cardestination == 1) turnright(cardirection, carnumber);
        	
        	if(cardestination == 2) gostraight(cardirection, carnumber);
        	
        	if(cardestination == 1)	turnleft(cardirection, carnumber);
        
        }
        if(cardirection == 1)
        {
        	if(cardestination == 0) turnleft(cardirection, carnumber);
        	
        	if(cardestination == 2) gostraight(cardirection, carnumber);
        	
        	if(cardestination == 3) turnright(cardirection, carnumber);
        
        
        }
        if(cardirection == 2)
        {
                if(cardestination == 0) gostraight(cardirection, carnumber);
        	
        	if(cardestination == 1) turnleft(cardirection, carnumber);
        	
        	if(cardestination == 3) turnright(cardirection, carnumber);

        
        }
        if(cardirection == 3)
        {
                if(cardestination == 0) turnright(cardirection, carnumber);
        	
        	if(cardestination == 1) gostraight(cardirection, carnumber);
        	
        	if(cardestination == 2) turnleft(cardirection, carnumber);

        
        }
        */
        //kprintf("Dir: %d Dest: %d, turnval: %d\n", cardirection, cardestination, turn_val);
       	
        if(turn_val == 1 || turn_val == -3)
        {
        	//kprintf("right\n");
        	
        	turnright(cardirection, carnumber);
        	thread_yield();
        
        }
        
        else if(turn_val == 2 || turn_val == -2)
        {
        	//kprintf("Straight\n");
        	gostraight(cardirection, carnumber);
        	thread_yield();
        }
        else
        {
        	//kprintf("left\n");
        
        	turnleft(cardirection, carnumber);
        	thread_yield();	
        }
        
        
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

        /*
         * Start NCARS approachintersection() threads.
         */

	bigsem = sem_create("bigsem", 2);
	NW = sem_create("NW", 1);
	NE = sem_create("NE", 1);
	SW = sem_create("SW", 1);
	SE = sem_create("SE", 1);
	
        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        return 0;
}
