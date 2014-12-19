/* 
BK/BT Checksum Algorithm Reference Implementation (bryc)
--------------------------------------------------------
This version attempts to replicate the algorithm as closely as possible, following the asm line by line. 
It is pretty accurate (I'd say), but not very optimized.

If you put a breakpoint on `PC=8025C20C`, it will halt immediately after the algorithm
has finished, andthe checksum values are in S3 and S4.
You can compare the registers at this breakpoint with the output of this program.

BK's final checksum is S3 ^ S4. BT's final checksum is (S3 << 32) + S4.
*/

#include <stdio.h>
#include <stdint.h>

int main(void)
{
    
    /* Initialize registers */
    
    /* R0  */ uint64_t R0 = 0x00;
    /* R1  */ uint64_t AT = 0x01;
    /* R2  */ uint64_t V0 = 0x80283420;
    /* R3  */ uint64_t V1 = 0x00;
    /* R4  */ uint64_t A0 = 0x80283400;
    /* R5  */ uint64_t A1 = 0x8028341C;
    /* R6  */ uint64_t A2 = 0x8027BC68;
    /* R7  */ uint64_t A3 = 0x80283400;
    /* R8  */ uint64_t T0 = 0x2000FF01;
    /* R9  */ uint64_t T1 = 0x80281700;
    /* R10 */ uint64_t T2 = 0x0C;
    /* R11 */ uint64_t T3 = 0x8028170C;
    /* R12 */ uint64_t T4 = 0x802816E8;
    /* R13 */ uint64_t T5 = 0x00;
    /* R14 */ uint64_t T6 = 0x8F809F47;
    /* R15 */ uint64_t T7 = 0x3108B3C1;
    /* R16 */ uint64_t S0 = 0x00283400;
    /* R17 */ uint64_t S1 = 0x00;
    /* R18 */ uint64_t S2 = 0x8027BC30;
    /* R19 */ uint64_t S3 = 0x00;
    /* R20 */ uint64_t S4 = 0x00;
    /* R21 */ uint64_t S5 = 0x8028341C;
    /* R22 */ uint64_t S6 = 0x00;
    /* R23 */ uint64_t S7 = 0x00;
    /* R24 */ uint64_t T8 = 0x802816E8;
    /* R25 */ uint64_t T9 = 0x80277370;
    /* R26 */ uint64_t K0 = 0xA430000C;
    /* R27 */ uint64_t K1 = 0x00000AAA;
    /* R28 */ uint64_t GP = 0x00;
    /* R29 */ uint64_t SP = 0x8027BBE8; // (0x8027BBE8 + 0x48) = 8027BC30
    /* R30 */ uint64_t S8 = 0x00;
    /* R31 */ uint64_t RA = 0x8033C034;
    
    uint64_t mem_DWORD  = 0x8F809F473108B3C1;
    
    unsigned char bytes[] = {
        0xC9, 0x08, 0xC5, 0x2F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    int i;
    for(i = 0; i < sizeof(bytes); i++)
    {
        T8 = bytes[i];
        T5 = (mem_DWORD & 0xFFFFFFFF);
        T9 = S1 & 0x000F;
        T0 = T8 << T9;
        T4 = (mem_DWORD >> 32);
        T7 = T0 + T5;
        T2 = T0 >> 0x1F;
        if (T7 < T5) { AT = 1; } else { AT = 0; }
        T6 = AT + T2;
        T6 = T6 + T4;
        mem_DWORD = (T6 << 32) + T7;
        
        A3 = mem_DWORD;
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
        mem_DWORD = A3;
        V0 = V0 >> (0x00 + 32);
        
        S3 = S3 ^ V0;
        
        S1 = S1 + 0x0007;
    }

    for(i = i - 1; i >= 0; i--)
    {
        T1 = bytes[i];
        T3 = (mem_DWORD & 0xFFFFFFFF);
        T8 = S1 & 0x000F;
        T9 = T1 << T8;
        T2 = (mem_DWORD >> 32);
        T5 = T9 + T3;
        T0 = T9 >> 0x1F;
        if (T5 < T3) { AT = 1; } else { AT = 0; }
        T4 = AT + T0;
        T4 = T4 + T2;
        mem_DWORD = (T4 << 32) + T5;
        
        A3 = mem_DWORD;
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
        mem_DWORD = A3;
        V0 = V0 >> (0x00 + 32);
        
        S4 = S4 ^ V0;
        
        S1 = S1 + 0x0003;
        
    }
    
    printf("Result (%d bytes):\n", sizeof(bytes));
    printf("\tV0: %08llX\n", V0);
    printf("\tS3: %08llX\n", S3);
    printf("\tS4: %08llX\n\n", S4);
    
    printf("\tBK: %08llX\n", S3 ^ S4);
    printf("\tBT: %016llX\n", (S3 << 32) + S4  );

    return 0;
}
