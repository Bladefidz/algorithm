// Code for testing a small, stand-alone section of code
// for repeatable results.
// This code will work as is in Microsoft(R) Visual C++(R) 5.0 compiler.
// Simply add in your own code in the marked sections and compile
// it.

#include <stdio.h>

#define CPUID __asm __emit 0fh __asm __emit 0a2h
#define RDTSC __asm __emit 0fh __asm __emit 031h

unsigned TestFunc();

void main () {
    unsigned base=0;
    unsigned cyc, cycles1, cycles2, cycles3, cycles4, cycles5;

    // The following tests run the basic cycle counter to find
    // the overhead associated with each cycle measurement.
    // It is run multiple times simply because the first call
    // to CPUID normally takes longer than subsequent calls.
    // Typically after the second run the results are
    // consistent. It is run three times just to make sure.

    __asm {
        CPUID
        RDTSC
        mov     cyc, eax
        CPUID
        RDTSC
        sub     eax, cyc
        mov     base, eax

        CPUID
        RDTSC
        mov     cyc, eax
        CPUID
        RDTSC
        sub     eax, cyc
        mov     base, eax

        CPUID
        RDTSC
        mov     cyc, eax
        CPUID
        RDTSC
        sub     eax, cyc
        mov     base, eax
    } // End inline assembly

    // This section calls the function that contains the user's
    // code. It is called multiple times to eliminate instruction
    // cache effects and get repeatable results.
    cycles1 = TestFunc();
    cycles2 = TestFunc();
    cycles3 = TestFunc();
    cycles4 = TestFunc();
    cycles5 = TestFunc();

    // By the second or third run, both data and instruction
    // cache effects should have been eliminated, and results
    // will be consistent.
    printf("Base : %d\n", base);
    printf("Cycle counts:\n");
    printf("%d\n", cycles1-base);
    printf("%d\n", cycles2-base);
    printf("%d\n", cycles3-base);
    printf("%d\n", cycles4-base);
    printf("%d\n", cycles5-base);
}
unsigned TestFunc() {
    //*** REPLACE THIS SECTION WITH YOUR OWN VARIABLES
    float z,q,x,y;
    float result;
    unsigned cycles;

    // Load the values here, not at creation, to make sure
    // the data is moved into the cache.
    cycles=0;
    result=0.0f;
    x=2.0f;
    z=12.0f;
    q=5.0f;
//*** END OF USER SECTION

    __asm {
        CPUID
        RDTSC
        mov     cycles, eax
    }

    //*** REPLACE THIS SECTION WITH THE CODE TO BE TESTED
    z += y;
    q *= x;
    result = z/q;
    //*** END OF USER SECTION

    __asm {
        CPUID
        RDTSC
        sub     eax, cycles
        mov     cycles, eax
    }
    return cycles;
}