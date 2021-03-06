/* blah_debug.c
	Defines routines for debugging */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

#include "blah_debug.h"
#include "blah_list.h"
#include "blah_util.h"
#include "blah_file.h"
#include "blah_macros.h"
#include "blah_error.h"
#include "blah_console.h"

/* Static Globals - Private to blah_debug.c */
static Blah_List logList = {
    .name = "debug_logs",
    .destroyElementFunction = (blah_list_element_dest_func*)Blah_Debug_Log_destroy,  // List of all entities, defaults to empty
};

/* Private Function Declarations */
extern void blah_message_writeToFileVA(FILE* file, const char* messageFormat, va_list varArgs);
extern void blah_message_writeErrorToFileVA(FILE* file, int errorCode, const char* messageFormat, va_list varArgs);

// Creates a new log file with given log name and returns FILE handle.
// If the file already exists, it is replaced with a new empty one.
static FILE* Blah_Debug_Log_createFile(char* logName)
{
	char tempFileName[BLAH_DEBUG_LOG_NAME_LENGTH + 4]; //Add 4 chars for ".log"
    snprintf(tempFileName, blah_countof(tempFileName), "%s.log", logName);
    FILE* newFile = fopen(tempFileName, BLAH_FILE_MODE_OVERWRITE);
    // TODO - Exit here if log file could not be created
	if (newFile == NULL) { blah_error_raise(errno, "Failed to create logfile '%s'", logName); }

	return newFile;  //If file creation failed, NULL pointer will be returned
}

/* Public Functions */

bool Blah_Debug_Log_close(Blah_Debug_Log *log)
{
    // Closes the file attached to the log and set file pointer to NULL.
    // Returns true if the log file could not be closed.
	bool closeOK = false;

	if (log->filePointer != NULL) {
        closeOK = fclose(log->filePointer) == 0; // fclose() returns 0 on success
        log->filePointer = NULL;  //Set file pointer to NULL, indicating no open log file
	}

	return closeOK;
}

// Unregisters log from log list, frees memory and deallocates file resources
void Blah_Debug_Log_destroy(Blah_Debug_Log *log)
{
	Blah_List_removeElement(&logList, log); //First remove from list of logs
	Blah_Debug_Log_close(log); //Close the log file
	free(log);
}

// Cleanup routine to do garbage collection for logs exit
void blah_debug_log_destroyAll()
{
	Blah_List_destroyElements(&logList);
}

// Disables Log.  Reverses initialisation.  Closes log.
void Blah_Debug_Log_disable(Blah_Debug_Log *log)
{
	Blah_Debug_Log_close(log);
}

// Initialises a given log data structure as a new log with new open file pointer
// to a log file on the file system with the same name as the given log name.
void Blah_Debug_Log_init(Blah_Debug_Log *log, const char *logName)
{
	log->filePointer = NULL;
	log->numEntries = 0;
	blah_util_strncpy(log->name, logName, BLAH_DEBUG_LOG_NAME_LENGTH); //Copy name string
	Blah_Debug_Log_open(log);
}

// Append the given string to the specified log with a following new line char
// Returns TRUE if success
bool Blah_Debug_Log_message(Blah_Debug_Log* log, const char* messageFormat, ...)
{
	if (log->filePointer == NULL) { // If invalid file pointer, return false immediately
        blah_console_message("Failed to write to log: %s - FILE NOT OPEN", log->name);
		return false;
    }

    // Try to write message to file using the variable args and then release them
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_message_writeToFileVA(log->filePointer, messageFormat, varArgs);
    va_end(varArgs);
    return true;
}

bool Blah_Debug_Log_error(Blah_Debug_Log* log, blah_error errorCode, const char* messageFormat, ...)
{
	if (log->filePointer == NULL) { // If invalid file pointer, return false immediately
        blah_console_message("Failed to write to log: %s - FILE NOT OPEN", log->name);
		return false;
    }

    // Try to write message to file using the variable args and then release them
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_message_writeErrorToFileVA(log->filePointer, errorCode, messageFormat, varArgs);
    va_end(varArgs);
    return true;
}

// Creates a new debugging log with given name
Blah_Debug_Log *Blah_Debug_Log_new(const char *logName)
{
	Blah_Debug_Log *newLog = malloc(sizeof(Blah_Debug_Log));  //Allocate memory for new log structure;

	if (newLog != NULL) { //If memory allocation ok
		Blah_Debug_Log_init(newLog, logName);
		Blah_List_appendElement(&logList, newLog); //Register log in resource list
	} else {
		blah_error_raise(errno, "Memory allocation for new log failed");
	}

	return newLog;
}

bool Blah_Debug_Log_open(Blah_Debug_Log *log)
{	// Attaches a new file to the log.
	// Implicitly closes the previously associated file if still currently open.
	if (log->filePointer) { fclose(log->filePointer); } //If there is a current file attached, close it

	log->filePointer = Blah_Debug_Log_createFile(log->name);
	log->numEntries = 0;

	return log->filePointer != NULL; // Return true if file creation was successful, else false
}
