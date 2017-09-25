#include <stdio.h>

/**
 * The main power of C programming language is "Address Arithmetic".
 * It's means you are outperforming turing complete computation model in every
 * programming aspect. We will see that array and pointer almost similar in C.
 * The rest of implementation depend on programmer intelligent, for example:
 * Some maybe create more space efficient storage using binary tree, while
 * others keeps using bizarre size of arrays because their goal more important
 * than efficiency issues.
 */

/**
 * Pointer or array arithmetic can be used to count allocated chars in memory
 * @param  s string
 * @return   ('\0' address) - (first address)
 */
int strlen(char *s)
{
    char *p = s;
    while (*p != '\0')
        p++;
    return p - s;
}

void f1(int arr[]){}
void f2(int *arr){}

/**
 * Main testing program
 * @param  argc Argument count
 * @param  argv Argument variable
 * @return      integer
 */
int main(int argc, char const *argv[])
{
    int x = 4, y, z[10];
    int *p;  // p point to int

    for (int i = 0; i < 10; ++i)
    {
        z[i] = i;
    }

    p = &x;  // p is now 4
    y = *p;  // y is now 4
    *p = 0;  // x is now 0
    p = &z[0];  // p is now point to z[0]
    p = z;  // equivalent to p = &z[0]
    p++;  // Access next address

    y = *(z+2);  // Evaluating each address in array
    y = z[2];  // equivalent to y = *(z+2)

    // All arguments below are equivalent
    f1(&z[2]);
    f2(&z[2]);
    f1(z+2);
    f2(z+2);

    char amsg[] = "Array of character";
    amsg[1] = 'B';  // This is allowed
    char *pmsg = "Pointer to an array of character";
    // *(pmsg + 1) = "B";  // This is not allowed

    return 0;
}