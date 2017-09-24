#include <stdio.h>

void f1(int arr[]){}
void f2(int *arr){}

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

    f1(&z[2]);
    f2(&z[2]);
    f1(z+2);
    f2(z+2);

    return 0;
}