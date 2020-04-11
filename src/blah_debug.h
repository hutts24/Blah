/* blah_debug.h
	Defines debugging routines */

#ifndef _BLAH_DEBUG

#define _BLAH_DEBUG

#include <stdio.h>

#include "blah_types.h"

/* Constant Definitions */

#define BLAH_DEBUG_LOG_NAME_LENGTH 128 //Doesn't include NULL char
#define BLAH_DEBUG_MESSAGE_LENGTH 100	//FIXME! - this should not be needed in future

/* Structure Definitions */

typedef struct Blah_Debug_Log
{	//A log file for debugging messages
	char name[BLAH_DEBUG_LOG_NAME_LENGTH+1]; //This is set by funcions
	FILE *filePointer; //At all times this should be either NULL or a valid pointer
	int numEntries;
} Blah_Debug_Log;

//typedef struct Blah_Debug_Log BLAH_DEBUG_LOG;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

blah_bool Blah_Debug_Log_close(Blah_Debug_Log *log);
	//Closes the file attached to the log.
	//Returns true apon success

void Blah_Debug_Log_destroy(Blah_Debug_Log *log);
	//Dregisters log from log list, frees memory and deallocates file resources

void blah_debug_log_destroyAll();
	//Cleanup routine to do garbage collection for logs on engine exit

void Blah_Debug_Log_disable(Blah_Debug_Log *log);
	//Disables Log.  Reverses initialisation.  Closes log.

void Blah_Debug_Log_init(Blah_Debug_Log *log, char *log_name);
	//Initialises a given log data structure as a new log with new open file pointer
	//to a log file on the file system with the same name as the given log name.

blah_bool Blah_Debug_Log_message(Blah_Debug_Log *log, char *message);
	//Append the given string to the specified log.  Returns TRUE if success

Blah_Debug_Log *Blah_Debug_Log_new(char *log_name);
	//Creates a new debugging log with given name and attached file
	//Memory is allocated and pointer returned

blah_bool Blah_Debug_Log_open(Blah_Debug_Log *log);
	//Attaches a new file to the log.  Implicitly closes the previously associated file
	//if still currently open. Returns true apon success, else false

#ifdef __cplusplus
	}
#endif //__cplusplus


#endif
