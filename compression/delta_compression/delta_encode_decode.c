#include <stdio.h> // puts and printf
#include <string.h> // strlen

// delta encoding and decoding on ASCII strings
// basically the char type is converted into an int type to allow for addition and subtraction
// doing so is basically doing arithmetic on the ASCII codepoints

void delta_encode(signed char * buffer, int length) {
    signed char last = 0;
    for (int i = 0; i < length; i++) {
        signed char current = buffer[i];
        buffer[i] = current - last;
        last = current;
    }
}

void delta_decode(signed char * buffer, int length) {
    signed char last = 0;
    for (int i = 0; i < length; i++) {
        signed char delta = buffer[i];
        buffer[i] = delta + last;
        last = buffer[i];
    }
}

int main (int argc, char * argv[]) {

    // a char is 1 byte - 8 bits - 2 hex digits, signed gives us -128 - 127
    signed char string[] = "aaabbbccbbdd";

    puts((char *) string);

    size_t size = sizeof(string);
    size_t length = strlen((char *) string);

    // the length and size will differ, as the size contains the last null termination byte
    // we only want to delta encode the length, not the entire size
    printf("Length:\t%lu\n", length);
    printf("Size:\t%lu\n", size);

    printf("Char:\t");
    for (unsigned int i = 0; i < length; ++i) {
        printf("%c\t", string[i]);
    }
    printf("\n");


    printf("Code:\t");
    for (unsigned int i = 0; i < length; ++i) {
        // we know we will only have positive ASCII code, so we can use %u
        printf("%u\t", string[i]);
    }
    printf("\n");

    printf("Hex:\t");
    for (unsigned int i = 0; i < length; ++i) {
        // we know a char is a 1 byte, 2 hex digits, so the minimum amount to print is 2, while padding with 0
        printf("0x%02x\t", string[i]);
    }
    printf("\n");

    delta_encode(string, length);

    printf("Char:\t");
    for (unsigned int i = 0; i < length; ++i) {
        // delta encoded ASCII string is not human readable
        // at most it should show the first character, anything else doesn't actually make any sense
        printf("%c\t", string[i]);
    }
    printf("\n");

    printf("Code:\t");
    for (unsigned int i = 0; i < length; ++i) {
        // our deltas can be negative
        printf("%i\t", string[i]);
    }
    printf("\n");

    printf("Hex:\t");
    for (unsigned int i = 0; i < length; ++i) {
        // technically printf promotes types to an unsigned int
        // however we have signed char, and we need to represent them as so
        // so the `hh` modifier causes printf to expect a char type
        // note that -1 in an unsigned char is 1111 1111 using 2's complement
        // it is therefore 0xFF in hex
        printf("0x%02hhx\t", string[i]);
    }
    printf("\n");

    // The delta encoding of "aaabbbccbbdd" results in:
    // 97 0 0 1 0 0 1 0 -1 0 2 0
    // Each delta is a patch on the previous version
    // The end result is easier compression

    delta_decode(string, length);

    puts((char *) string);

    return 0;

}
