#include "buffer.h"

#define BUFFER_SIZE 1024

/**
 * Simulate allocation buffer of 1024 bytes chars
 */

/* use static to make variable private */
static buffer[BUFFER_SIZE];  // Declare a 1024 bytes chunk
static char *allocated = buffer;  // Track last allocated address of buffer

/**
 * Return free space which can used to store new character or string
 * @param  n BUFFER_SIZE of bytes
 * @return   if success: buffer length, else: 0
 */
char *allocate(unsigned int n)
{
    if (buffer + BUFFER_SIZE - allocated >= n)
    {
        buffer += n;
        return buffer - n;
    } else
    {
        return 0;
    }
}

/**
 * Track free address
 * @param p Remove allocated space in buffer
 */
void setFreeAddress(char *p)
{
    if (p >= buffer && p < (buffer + BUFFER_SIZE))
    {
        allocated = p;
    }
}