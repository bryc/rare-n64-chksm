#include <stdio.h>

/*
  WARNING! The 32-bit method is not as accurate as the main 64-bit version.  In very rare circumstances, the checksum
  will be incorrect. I ran two algorithms on random data in parallel, catching mismatches.
  I inserted the random data directly into RDRAM to see the game calculate the legitimate checksum.
  The 32-bit checksum was incorrect, while the 64-bit one was correct.
  
  Use this only for testing or if you need a 32-bit implementation (JS, non-64 bit computer etc).

  BK/BT Algorithm (32 bit int)
  -----------------------------
  The original MIPS algorithm used 64 bits. This version uses 32 bits.
  I did a lot of testing on this, however in rare cases it may produce a different result
  than other versions of the algorithm. The prevalence of errors increase as the value of the input bytes increase.
  There is not much I can do except take a closer look back at the raw assembly, and create a thorough test suite.
  
  However it passed a list of 256 real msg pairs taken from the game itself, as well as all the extra pairs
  I have on hand. Discrepancies only occur with manual byte input, and happens only specific combinations of bytes.
  No combination of 0x00 will ever produces an error. 
*/
int main(void)
{
    unsigned char arr[] = {
        // Actual BK Game data, Checksum: 2C4F1C58
        0x11, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	    0xFF, 0xFF, 0x1E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEE,
	    0xEF, 0xFC, 0xFF, 0xFB, 0x6F, 0xDF, 0xFF, 0xFF, 0x0E, 0x00, 0x64, 0xC9,
	    0x93, 0x26, 0x4C, 0x99, 0x32, 0x64, 0x03, 0x2A, 0x08, 0xED, 0x07, 0xEE,
	    0x09, 0x60, 0x08, 0x36, 0x18, 0x66, 0x09, 0x28, 0x19, 0xEB, 0x0E, 0xBA,
	    0x08, 0x7D, 0x01, 0x74, 0xFB, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF,
	    0xFF, 0x7F, 0xFE, 0xBF, 0x3D, 0xA6, 0xF2, 0x67, 0x1E, 0xFE, 0xDF, 0xFF,
	    0xE7, 0x7D, 0xFF, 0xF3, 0xFC, 0xBF, 0xD6, 0xB8, 0xFF, 0x31, 0xF8, 0x17,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xC8, 0x64, 0x0A, 0x02, 0x00, 0x0F,
	    0xFF, 0xFF, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00
    };

   unsigned int a  = 0x3108B3C1;
   unsigned int t  = 1;
   unsigned int b  = 0;
   unsigned int bt = 0;
   long chk = 0;

    int i = 0, c = 0;

    for(i = 0; i < sizeof(arr); i++, c += 7)
    {
        a += arr[i] << (c & 0xF);
        b = (a >> 1 | t << 31) ^ (a & 0xFFFFF) << 12; t = a & 1;
        a = (b >> 20) & 0xFFF ^ b;
        chk = a ^ chk;
    }
    bt = chk;
    for(i--; i >= 0; i--, c += 3)
    {
        a += arr[i] << (c & 0xF);
        b = (a >> 1 | t << 31) ^ (a & 0xFFFFF) << 12; t = a & 1;
        a = (b >> 20) & 0xFFF ^ b;
        chk = a ^ chk;
    }
    printf("---\n%08X\n", chk);
    return 0;
}
