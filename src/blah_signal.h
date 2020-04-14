/* blah_signal.h
    C runtime Signal handling
*/

#ifndef _BLAH_SIGNAL

#define _BLAH_SIGNAL

#include <signal.h>
#include <stdbool.h>

// Set up all the signal handlers
bool blah_signal_init();


#endif // _BLAH_SIGNAL
