#include <fx2types.h>
/**
 * This function is called after the rdwr_data has been received from the host
 * and before any handlers are called.  You can use this function to set the
 * firmware in a state usable by all the handlers.
 * 
 * @return TRUE if things are OK or FALSE if you want to abort the rdwr command.
 **/

BOOL handlers_init() {
 return TRUE;
}
