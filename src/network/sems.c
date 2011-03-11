#include "sems.h"
#include "errors.h"

extern int errno;
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: initsem
  --
  -- DATE: Feb 9, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int initsem(void)

  --
  -- RETURNS: -1 on failure semaphore ID on success.
  --
  -- NOTES:
  -- This function attempts to either create, or open a semaphore.
  ----------------------------------------------------------------------------------------------------------------------*/
int initsem (void) {

    int sid, status = 0;


    if ((sid = semget(SEM_KEY, 1, 0666|IPC_CREAT|IPC_EXCL)) == -1) {

	if (errno == EEXIST) {
	    sid = semget (SEM_KEY, 1, 0);
	}

    }

    status = semctl (sid, 0, SETVAL, 1);

    if ((sid == -1) || (status == -1)) {
	serv_err (SEM_ERR, "initsem");
	return (-1);
    }
    return (sid);
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: P
  --
  -- DATE: Feb 2, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void P(int sid)
  --		- sid - the ID of the semaphore set to modify
  --
  -- RETURNS: void
  --
  -- NOTES:
  -- This function increments a semaphore within a specified set.
  ----------------------------------------------------------------------------------------------------------------------*/
void P(int sid) {
  
  
    struct sembuf sembuf = {0, -1, SEM_UNDO};

    if ((semop(sid, &sembuf, 1)) == -1) {
	serv_err(SEM_ERR, "semop");
    }
  
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: V
  --
  -- DATE: Feb 9, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void V(int sid)
  --		- sid - the ID of the semaphore set to modify.
  --
  -- RETURNS: void
  --
  -- NOTES:
  -- This function decrements a semaphore within a specified set.
  ----------------------------------------------------------------------------------------------------------------------*/
void V(int sid) {
  
    struct sembuf sembuf = {0, 1, SEM_UNDO};
  
    if ((semop(sid, &sembuf , 1)) == -1) {
	serv_err(SEM_ERR, "semop");
    }
  
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: get_sem_val
  --
  -- DATE: Feb 9, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int get_sem_val(int sid)
  --		- sid - the ID of the semaphore set.
  --
  -- RETURNS: the value of the semaphore.
  --
  -- NOTES:
  -- This function gets the value of a semaphore.
  ----------------------------------------------------------------------------------------------------------------------*/
int get_sem_val(int sid) {
    return semctl(sid, 0, GETVAL);
}
