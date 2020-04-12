/* blah_debug.c
	Defines routines for debugging */

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "blah_debug.h"
#include "blah_list.h"
#include "blah_util.h"

/* Static Globals - Private to blah_debug.c */

Blah_List logList = {"", NULL, NULL, (blah_list_element_dest_func)Blah_Debug_Log_destroy};  //List of all entities, defaults to empty

/* Private Function Declarations */

static FILE *Blah_Debug_Log_createFile(char *logName)
{	//Creates a new log file with given log name and returns FILE handle
	FILE *newFile;
	char tempFileName[BLAH_DEBUG_LOG_NAME_LENGTH+4]; //Add 4 chars for ".log"

	strcpy(tempFileName, logName);
	strcat(tempFileName, ".log");
	newFile = fopen(tempFileName, "w");

	if (!newFile)
		fprintf(stderr,"Create log file failed\n");

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

void Blah_Debug_Log_destroy(Blah_Debug_Log *log)
{	//Dregisters log from log list, frees memory and deallocates file resources
	Blah_List_removeElement(&logList, log); //First remove from list of logs
	Blah_Debug_Log_close(log); //Close the log file
	free(log);
}

void blah_debug_log_destroyAll()
{	//Cleanup routine to do garbage collection for logs exit
	Blah_List_destroyElements(&logList);
}

void Blah_Debug_Log_disable(Blah_Debug_Log *log)
{	//Disables Log.  Reverses initialisation.  Closes log.
	Blah_Debug_Log_close(log);
}

void Blah_Debug_Log_init(Blah_Debug_Log *log, const char *logName)
{	//Initialises a given log data structure as a new log with new open file pointer
	//to a log file on the file system with the same name as the given log name.
	log->filePointer = NULL;
	log->numEntries = 0;
	blah_util_strncpy(log->name, logName, BLAH_DEBUG_LOG_NAME_LENGTH); //Copy name string
	Blah_Debug_Log_open(log);
}

bool Blah_Debug_Log_message(Blah_Debug_Log *log, char *message)
{	//Append the given string to the specified log with a following new line char
	//Returns TRUE if success
	if (log->filePointer != NULL) {  //If valid file pointer, attempt to write to log file
		if (fprintf(log->filePointer, "%s\n", message) < 0) {
			fprintf(stderr,"Failed to write to log:%s\n", log->name);
			return false; //Negative return from printf means write failed
		} else { //Success
			fflush(log->filePointer);
			log->numEntries++;	//Increment entry count
			return true;
		}
	} else {
		fprintf(stderr,"Failed to write to log:%s - FILE NOT OPEN\n", log->name);
		return false;  //Return false by default if no file to write to
	}
}

Blah_Debug_Log *Blah_Debug_Log_new(char *logName)
{	//Creates a new debugging log with given name
	Blah_Debug_Log *newLog = malloc(sizeof(Blah_Debug_Log));  //Allocate memory for new log structure;

	if (newLog) { //If memory allocation ok
		Blah_Debug_Log_init(newLog, logName);
		Blah_List_appendElement(&logList, newLog); //Register log in resource list
	} else {
		fprintf(stderr, "Memory allocation for new log failed\n");
	}

	return newLog;
}

bool Blah_Debug_Log_open(Blah_Debug_Log *log)
{	//Attaches a new file to the log.  Implicitly closes the previously associated file
	//if still currently open.
	if (log->filePointer)
		fclose(log->filePointer); //If there is a current file attached, close it

	log->filePointer = Blah_Debug_Log_createFile(log->name);
	log->numEntries = 0;

	if (log->filePointer)
		return true;
	else
		return false; //Return TRUE if file creation was successful, else FALSE
}
