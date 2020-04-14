/* blah_time.c
    Time related routines
*/

#include "blah_time.h"

/* Private Local Types */
typedef struct tm tm;

/* Static Private Functions */

// Formats given raw tm structure into readable text according to given format.
// Format must be in the same format used by strftime().
// The raw time does not include time zone information, so no conversion can be performed
static void blah_time_formatTM(const tm* noTimeZone, const char* formatString, char* dest, size_t charCount)
{
    const size_t maxBytes = charCount * sizeof(char);
    strftime(dest, maxBytes, formatString, noTimeZone);
}

// Convert time value since epoch GMT into a raw time in the local time zone.
// The resulting raw time does not contain time zone information
static void blah_time_toLocalTM(const blah_time* epochTime, tm* localTM)
{
    // Use localtime_s or locatime_r depending upon which one is available
    localtime_s(localTM, epochTime);
}

/* Public Functions */

// Retrieve the current time in UTC into timespec pointed by 'dest'
void blah_time_getCurrentUTC(blah_time* dest)
{
    time((time_t*)dest);
}

// Formats given epoch GMT time as a time-only value in the local zone
void Blah_Time_toLocalTimeString(const blah_time* epochTime, char* dest, size_t charCount)
{
    tm localTM;
    blah_time_toLocalTM(epochTime, &localTM);
    blah_time_formatTM(&localTM, "%H:%M:%S", dest, charCount);
}

