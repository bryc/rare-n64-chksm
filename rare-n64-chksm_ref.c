/* 
    Rare Checksum Algorithm Reference Implementation (bryc)
    --------------------------------------------------------
    This version attempts to replicate the algorithm as closely as possible,
    following the asm line by line. It should be quite accurate to the original.

    The Rare checksum algorithm is used in Banjo-Kazooie, Banjo-Tooie, Goldeneye 007 and Perfect Dark.
    Banjo-Kazooie's final checksum is S3 ^ S4, while Goldeneye and Tooie use (S3<<32)+S4.
    Perfect Dark uses (S3 & 0xFFFF) << 16 | S4 & 0xFFFF.
*/
#include <stdio.h>
#include <stdint.h>

unsigned char bytes[] = { // BK: 32C9A1E6, BT: 98593921AA9098C7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

int main(void) {
    /* Initialize registers */
    /* R1  */ uint64_t AT = 0x00;
    /* R2  */ uint64_t V0 = 0x00;
    /* R5  */ uint64_t A1 = 0x00;
    /* R6  */ uint64_t A2 = 0x00;
    /* R7  */ uint64_t A3 = 0x00;
    /* R8  */ uint64_t T0 = 0x00;
    /* R9  */ uint64_t T1 = 0x00;
    /* R10 */ uint64_t T2 = 0x00;
    /* R11 */ uint64_t T3 = 0x00;
    /* R12 */ uint64_t T4 = 0x00;
    /* R13 */ uint64_t T5 = 0x00;
    /* R14 */ uint64_t T6 = 0x8F809F47;   // Initial value #1
    /* R15 */ uint64_t T7 = 0x3108B3C1;   // Initial value #2
    /* R17 */ uint64_t S1 = 0x00;         // Shift value
    /* R19 */ uint64_t S3 = 0x00;         // Final checksum #1
    /* R20 */ uint64_t S4 = 0x00;         // Final checksum #2
    /* R24 */ uint64_t T8 = 0x00;
    /* R25 */ uint64_t T9 = 0x00;
    
    uint64_t DWORD_MEM = (T6 << 32) + T7;

    int i;
    for(i = 0; i < sizeof(bytes); i++) {
        T8 = bytes[i];
        T5 = (DWORD_MEM & 0xFFFFFFFF);
        T9 = S1 & 0x000F;
        T0 = T8 << T9;
        T4 = (DWORD_MEM >> 32);
        T7 = T0 + T5;
        T2 = T0 >> 0x1F;
        if (T7 < T5) { AT = 1; } else { AT = 0; }
        T6 = AT + T2;
        T6 = T6 + T4;
        DWORD_MEM = (T6 << 32) + T7;
        
        A3 = DWORD_MEM;
        A2 = A3 << (0x1F + 32);
        A1 = A3 << 0x1F;
        A2 = A2 >> 0x1F;
        A1 = A1 >> (0x00 + 32);
        A3 = A3 << (0x0C + 32);
        A2 = A2 | A1;
        A3 = A3 >> (0x00 + 32);
        A2 = A2 ^ A3;
        A3 = A2 >> 0x14;
        A3 = A3 & 0x0FFF;
        A3 = A3 ^ A2;
        V0 = A3 << (0x00 + 32);
        DWORD_MEM = A3;
        V0 = V0 >> (0x00 + 32);
        
        S3 = S3 ^ V0;
        S1 = S1 + 0x0007;
    }

    for(i = i - 1; i >= 0; i--) {
        T1 = bytes[i];
        T3 = (DWORD_MEM & 0xFFFFFFFF);
        T8 = S1 & 0x000F;
        T9 = T1 << T8;
        T2 = (DWORD_MEM >> 32);
        T5 = T9 + T3;
        T0 = T9 >> 0x1F;
        if (T5 < T3) { AT = 1; } else { AT = 0; }
        T4 = AT + T0;
        T4 = T4 + T2;
        DWORD_MEM = (T4 << 32) + T5;
        
        A3 = DWORD_MEM;
        A2 = A3 << (0x1F + 32);
        A1 = A3 << 0x1F;
        A2 = A2 >> 0x1F;
        A1 = A1 >> (0x00 + 32);
        A3 = A3 << (0x0C + 32);
        A2 = A2 | A1;
        A3 = A3 >> (0x00 + 32);
        A2 = A2 ^ A3;
        A3 = A2 >> 0x14;
        A3 = A3 & 0x0FFF;
        A3 = A3 ^ A2;
        V0 = A3 << (0x00 + 32);
        DWORD_MEM = A3;
        V0 = V0 >> (0x00 + 32);
        
        S4 = S4 ^ V0;
        S1 = S1 + 0x0003;
    }
    
    printf("Result (%d bytes):\n", sizeof(bytes));
    printf("\tV0: %08llX\n"  , V0);
    printf("\tS3: %08llX\n"  , S3);
    printf("\tS4: %08llX\n\n", S4);
    printf("\tBK: %08llX\n"  , S3 ^ S4);
    printf("\tBT: %016llX\n" , (S3 << 32) + S4);
    printf("\tPD: %08llX\n"  , (S3 & 0xFFFF) << 16 | S4 & 0xFFFF);
    return 0;
}
