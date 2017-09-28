#include <stdlib.h>
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
int myStrlen(char *s)
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
    ps[1] = "kamu"; // First pointer pointed to array "kamu"

    // Self reference structure
    struct A {
        unsigned int value;
        struct A *next;  // Pointer of self
    } A;

    // Pointer to structure
    struct A *a = malloc(sizeof(A));
    (*a).value = 112;

    // Array of structure
    struct B {
        unsigned int count;
        struct A **child;  // This is array of structure
    } B;
    struct B *b = malloc(sizeof(B));
    (*b).child = malloc(sizeof(A) * 2);

    // Allocate each index of array of structure
    *((*b).child) = malloc(sizeof(A));
    (*(*((*b).child))).value = 113;
    (*(*((*b).child))).next = NULL;
    *(*((*b).child) + 1) = malloc(sizeof(A));
    (*(*((*b).child) + 1)).value = 114;
    (*(*((*b).child) + 1)).next = NULL;

    // Above equivalent to this:
    // b->child[0] = malloc(sizeof(A));
    // (b->child[0])->value = 113;

    printf("%d\n", (*(*((*b).child))).value);
    free(a); free(b);

    return 0;
}