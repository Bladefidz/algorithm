// This code will find an average number of cycles taken to go
// through a loop. There is no cache warming, so all cache effects
// are included in the cycle count.

// To use this in your own code, simply paste in the six marked
// sections into the designated locations in your code.
#include <stdio.h>
#include <windows.h>
// *** BEGIN OF INCLUDE SECTION 1
// *** INCLUDE THE FOLLOWING DEFINE STATEMENTS FOR MSVC++ 5.0
#define CPUID __asm __emit 0fh __asm __emit 0a2h
#define RDTSC __asm __emit 0fh __asm __emit 031h
// *** END OF INCLUDE SECTION 1

#define SIZE 5
// *** BEGIN OF INCLUDE SECTION 2
// *** INCLUDE THE FOLLOWING FUNCTION DECLARATION AND CORRESPONDING
// *** FUNCTION (GIVEN BELOW)
unsigned FindBase();
// *** END OF INCLUDE SECTION 2

void main () {
    int i;

    // *** BEGIN OF INCLUDE SECTION 3
    // *** INCLUDE THE FOLLOWING DECLARATIONS IN YOUR CODE
    // *** IMMEDIATELY AFTER YOUR DECLARATION SECTION.
    unsigned base=0, iterations=0, sum=0;
    unsigned cycles_high1=0, cycles_low1=0;
    unsigned cycles_high2=0, cycles_low2=0;
    unsigned __int64 temp_cycles1=0, temp_cycles2=0;
    __int64 total_cycles=0; // Stored signed so it can be converted
    // to a double for viewing

    double
    seconds=0.0L;
    unsigned mhz=150;       // If you want a seconds count instead
                            // of just cycles, enter the MHz of your
                            // machine in this variable.
    base=FindBase();
    //*** END OF INCLUDE SECTION 3

    for (i=0; i<SIZE; i++) {
        // *** BEGIN OF INCLUDE SECTION 4
        // *** INCLUDE THE FOLLOWING CODE IMMEDIATELY BEFORE SECTION
        // *** TO BE MEASURED
        __asm {
            pushad
            CPUID
            RDTSC
            mov     cycles_high1, edx
            mov     cycles_low1, eax
            popad
        }
        // *** END OF INCLUDE SECTION 4

        // User code to be measured is in this section.
        Sleep(3000);

        // *** BEGIN OF INCLUDE SECTION 5
        // *** INCLUDE THE FOLLOWING CODE IMMEDIATELY AFTER SECTION
        // *** TO BE MEASURED
        __asm {
            pushad
            CPUID
            RDTSC
            mov     cycles_high2, edx
            mov     cycles_low2, eax
            popad
        }
        // Move the cycle counts into 64-bit integers
        temp_cycles1 = ((unsigned __int64)cycles_high1 << 32) | cycles_low1;
        temp_cycles2 = ((unsigned __int64)cycles_high2 << 32) | cycles_low2;

        // Add to total cycle count
        total_cycles += temp_cycles2 - temp_cycles1 - base;
        iterations++;
        // *** END OF INCLUDE SECTION 5
    }
    // Now the total cycle count and iterations are available to
    // be used as desired.
    // Example:
    seconds = double(total_cycles)/double(mhz*1000000);
    printf("Average cycles per loop: %f\n",
    double(total_cycles/iterations));
    printf("Average seconds per loop: %f\n", seconds/iterations);
}

// *** BEGIN OF INCLUDE SECTION 6
// *** INCLUDE THE FOLLOWING FUNCTION WITH YOUR CODE TO MEASURE
// *** THE BASE TIME
unsigned FindBase() {
    unsigned base,base_extra=0;
    unsigned cycles_low, cycles_high;
    // The following tests run the basic cycle counter to find
    // the overhead associated with each cycle measurement.
    // It is run multiple times simply because the first call
    // to CPUID normally takes longer than subsequent calls.
    // Typically after the second run the results are
    // consistent. It is run three times just to make sure.

    __asm {
        pushad
        CPUID
        RDTSC
        mov     cycles_high, edx
        mov     cycles_low, eax
        popad
        pushad
        CPUID
        RDTSC
        popad

        pushad
        CPUID
        RDTSC
        mov     cycles_high, edx
        mov     cycles_low, eax
        popad
        pushad
        CPUID
        RDTSC
        popad
        pushad
        CPUID
        RDTSC
        mov     cycles_high, edx
        mov     cycles_low, eax
        popad
        pushad
        CPUID
        RDTSC
        sub     eax, cycles_low
        mov     base_extra, eax
        popad

        pushad
        CPUID
        RDTSC
        mov     cycles_high, edx
        mov     cycles_low, eax
        popad
        pushad
        CPUID
        RDTSC
        sub     eax, cycles_low
        mov     base, eax
        popad
    } // End inline assembly
    // The following provides insurance for the above code,
    // in the instance the final test causes a miss to the
    // instruction cache.
    if (base_extra < base)
        base = base_extra;
    return base;
}
//*** END OF INCLUDE SECTION 6