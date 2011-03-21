#include "errors.h"
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: serv_err
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void serv_err(int retval, char* msg)
  --                    - retval -- the value for the program to exit with.
  --                    - msg    -- the message to be printed upon exit.
  --
  -- RETURNS: void
  --
  -- NOTES:
  -- returns with an error code and a message.
  ----------------------------------------------------------------------------------------------------------------------*/
void serv_err(int retval, char* msg) {
    perror(msg);
    exit(retval);
}
