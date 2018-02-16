#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * The main power of C programming language is "Address Arithmetic".
 * It's means you are outperforming turing complete computation model in every
 * programming aspect. We will see that array and pointer almost similar in C.
 * The rest of implementation depend on programmer intelligent, for example:
 * Some maybe create more space efficient storage using binary tree, while
 * others keeps using bizarre size of arrays because their goal more important
 * than efficiency issues.
 */


// Dummy function used to test pointer as argument
void f1(int arr[]){}
void f2(int *arr){}

/**
 * Pointer or array arithmetic can be used to count allocated chars in memory
 * @param  s string
 * @return   ('\0' address) - (first address)
 */
int myStrlen(char *s)
{
    char *p = s;
    while (*p != '\0')
        p++;
    return p - s;
}

/**
 * Implementing pointer to copy string "t" to string "s"
 * @param s Copy destination
 * @param t String to be copied
 */
void myStrcpy(char *s, char *t)
{
    while ((*s++ = *t++) != '\0');
}
void myStrcpy1(char *s, char *t)
{
    while (*s++ = *t++);
}

/**
 * Compare string s to string t
 * @return   0 if s<t, 0 if s==t, >0 if s>t
 */
int myStrcmp(char *s, char *t)
{
    for ( ; *s == *t; s++, t++)
        if (*s == '\0')
            return 0;
    return *s - *t;
}

/**
 * Get specific index in array of pointer
 * @param  i [description]
 * @return   [description]
 */
char *getTerm(unsigned int i)
{
    char *arrStr[] = {"pointer", "is", "beautiful!"};
    return arrStr[i];
}

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

    p = &x;     // p is now 4
    y = *p;     // y is now 4
    *p = 0;     // x is now 0
    p = &z[0];  // p is now point to z[0]
    p = z;      // equivalent to p = &z[0]
    (*p)++;     // Increment x by 1
    p++;        // Access next address

    y = *(z+2);  // Evaluating each address in array
    y = z[2];  // equivalent to y = *(z+2)

    // All arguments below are equivalent
    f1(&z[2]);
    f2(&z[2]);
    f1(z+2);
    f2(z+2);

    // Array
    char amsg[] = "Array of character";  // Allocate as array
    amsg[1] = 'B';  // This is allowed

    // Pointer of Array
    char *pmsg = "Pointer to an array of character";  // Pointer to fixed size array
    // *(pmsg + 1) = "B";  // This is not allowed

    // Pointer of Pointer of array
    char *ss[] = {"aku", "kamu", "dia", "dan", "mereka"};
    char **ps;
    ps = malloc(sizeof(char) * 2);  // Allocate Pointer
    ps[0] = "aku";  // First pointer pointed to array "aku"
    ps[1] = malloc(sizeof(char) * 5);
    for (int i = 0; i < 5; ++i)
    {
        ps[1][i] = 'a'+i;
    }

    // Self reference structure
    struct A {
        char *value;
        struct A *next;  // Pointer of self
    } A;

    // Pointer to structure
    struct A *a = malloc(sizeof(A));
    (*a).value = "aku";

    // Pointer of pointer to structure
    struct B {
        unsigned int count;
        struct A **child;  // or array of structure
    } B;
    struct B *b = malloc(sizeof(B));
    (*b).child = malloc(sizeof(A) * 2);

    // Allocate each index of array of structure
    *((*b).child) = malloc(sizeof(A));
    (*(*((*b).child))).value = malloc(sizeof(char) * 4);
    for (int i = 0; i < 4; ++i)
    {
        *((*(*((*b).child))).value + i) = 'a'+i;
    }
    (*(*((*b).child))).next = NULL;
    b->child[1] = malloc(sizeof(A));
    b->child[1]->value = malloc(sizeof(char) * 5);
    for (int i = 0; i < 4; ++i)
    {
        b->child[1]->value[i] = 'f'+i;
    }
    b->child[1]->next = NULL;

    free(a); free(b);

    // Working on string using pointer is more portable than using array
    char *s1 = "aku";
    char *s2 = NULL;  // Set destination pointer to NULL to allocate latter
    s2 = malloc(sizeof(s1) * strlen(s1));  // Allocate string length
    strcpy(s2, s1);
    s2[1] = 'l';  // Update the copied string

    // We can not update 'not allocated' string
    s1 = "kamu";
    //s1[0] = "j";  // Because the address is not owned by pointer nor anything

    return 0;
}