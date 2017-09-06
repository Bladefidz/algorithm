#define cpuid __asm __emit 0fh __asm __emit 0a2h
#define rdtsc __asm __emit 0fh __asm __emit 031h

#include <stdio.h>

void main () {
    int time, subtime;
    float x = 5.0f;

    __asm {
        // Make three warm-up passes through the timing routine to make
        // sure that the CPUID and RDTSC instruction are ready

        cpuid
        rdtsc
        mov     subtime, eax
        cpuid
        rdtsc
        sub     eax, subtime
        mov     subtime, eax

        cpuid
        rdtsc
        mov     subtime, eax
        cpuid
        rdtsc
        sub     eax, subtime
        mov     subtime, eax

        cpuid
        rdtsc
        mov     subtime, eax
        cpuid
        rdtsc
        sub     eax, subtime
        mov     subtime, eax // Only the last value of subtime is kept
        // subtime should now represent the overhead cost of the
        // MOV and CPUID instructions

        fld     x
        fld     x
        cpuid               // Serialize execution
        rdtsc               // Read time stamp to EAX
        mov     time, eax
        fdiv                // Perform division
        cpuid               // Serialize again for time-stamp read
        rdtsc
        sub     eax, time   // Find the difference
        mov     time, eax
        }
    time = time - subtime; // Subtract the overhead
    printf ("%d\n", time); // Print total time of divide to screen
}