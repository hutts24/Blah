/* blah_time.h
    Time related routines
*/

#ifndef _BLAH_TIME

#define _BLAH_TIME

#include <time.h>

/* Type Definitions */

// For now, times are just going to be stored as he old dumb C time_t variable type.
// time_t is 'usually' stored as a number of seconds from the epoch GMT and 'most' systems do this.
// I wanted to use timespec but despite being part of C11, its implementation doesn't appear standard or even consistent with recommendations and reference material.
typedef time_t blah_time;

// Retrieve the current time in UTC into timespec pointed by 'dest'
void blah_time_getCurrentUTC(blah_time* dest);

// Formats given epoch GMT time as a time-only string in the local zone
void Blah_Time_toLocalTimeString(const blah_time* epochTime, char* dest, size_t charCount);

#endif // _BLAH_TIME
