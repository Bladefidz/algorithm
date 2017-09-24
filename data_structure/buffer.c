#define BUFFER_SIZE 1024

/**
 * Simulate allocation buffer of 1024 bytes chars
 */

/* use static to make variable private */
static buffer[BUFFER_SIZE];  // Declare a 1024 bytes chunk
static char *allocated = buffer;  // Track last allocated address of buffer, in this case track first address or &address[0]

/**
 * Return free space which can used to store new character or string
 * @param  n BUFFER_SIZE of bytes
 * @return   [description]
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
 * Clear storage that used by string or char of p.
 * Or in short, remove addresses pointed by p.
 * @param p [description]
 */
void freeSpace(char *p)
{
    if (p >= buffer && p < (buffer + BUFFER_SIZE))
    {
        allocated = p;
    }
}

/**
 * Pointer or array arithmetic can be used to count allocated chars in memory
 * @param  s [description]
 * @return   [description]
 */
int strlen(char *s)
{
    char *p = s;
    while (*p != '\0')
        p++;
    return p - s;
}