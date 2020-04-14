/* blah_signal.c
    Defines routines for handling runtime signals
*/

#include <stdnoreturn.h>
#include <stdlib.h>
#include <errno.h>

#include "blah_signal.h"
#include "blah_console.h"

// Handle the abort signal.  If this signal occurs, something really bad has happened.
// This function calls abort because it's probably futile trying to perform normal cleanup.
static noreturn void handleAbort(int signal)
{
    blah_console_error(errno, "Received abort signal!");
    abort();
}

// Handle the terminate signal.  The OS is telling the program to die now.
// In this circumstance it should be possible to perform normal cleanup and release of resources.
static noreturn void handleTerminate(int signal)
{
    blah_console_message("Received terminate signal.  Exiting as normal.");
    exit(EXIT_SUCCESS);
}

// Handle the signal received when the program causes a segmentation fault / memory access violation.
// In this circumstance it should be possible to perform normal cleanup and release of resources.
static noreturn void handleSegmentationFault(int signal)
{
    const int err = errno;
    blah_console_error(err, "Segmentation fault!");
    exit(err);
}

// Set up all the signal handlers
bool blah_signal_init()
{
    return signal(SIGABRT, handleAbort) != SIG_ERR && signal(SIGSEGV, handleSegmentationFault) != SIG_ERR && signal(SIGTERM, handleTerminate) != SIG_ERR;
}
