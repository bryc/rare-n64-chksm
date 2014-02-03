#include <stdio.h>
#include <stdint.h>

#define NUMBYTES    28

#define EEPROM_MAXBLOCKS        64
#define EEPROM_BLOCK_SIZE       8

static void import_EEPROM(const char* filename);
static int is_slot_corrupt(int block, int size);
static unsigned long get_Banjo_checksum(int block, int size);

static unsigned char EEPROM[EEPROM_MAXBLOCKS * EEPROM_BLOCK_SIZE];

static void import_EEPROM(const char* filename)
{
    FILE* stream;
    register int block, byte;

    stream = fopen(filename, "rb");
    if (stream == NULL)
    {
        printf("Failed to open \"%s\" for reading.\n", filename);
        return;
    }
    for (block = 0; block < EEPROM_MAXBLOCKS; block++)
        for (byte = 0; byte < EEPROM_BLOCK_SIZE; byte++)
            EEPROM[EEPROM_BLOCK_SIZE*block + byte] = fgetc(stream);
    fclose(stream);
    return;
}

int main(void)
{
    int corrupt;

    import_EEPROM("NBKE.EEP");
    corrupt = is_slot_corrupt(0x000/EEPROM_BLOCK_SIZE, 120 - 4);
    corrupt = is_slot_corrupt(0x078/EEPROM_BLOCK_SIZE, 120 - 4) + corrupt;
    corrupt = is_slot_corrupt(0x0F0/EEPROM_BLOCK_SIZE, 120 - 4) + corrupt;
    corrupt = is_slot_corrupt(0x168/EEPROM_BLOCK_SIZE, 120 - 4) + corrupt;
    corrupt = is_slot_corrupt(0x1E0/EEPROM_BLOCK_SIZE,  32 - 4) + corrupt;
    printf("Number of checksum mismatches:  %i.\n", corrupt);
    return 0;
}

static int is_slot_corrupt(int block, int size)
{
    unsigned long checksum_proper, checksum_stored;

    checksum_proper = get_Banjo_checksum(block, size);
    checksum_stored =
        (EEPROM[EEPROM_BLOCK_SIZE*block + size + 0] << 24)
      | (EEPROM[EEPROM_BLOCK_SIZE*block + size + 1] << 16)
      | (EEPROM[EEPROM_BLOCK_SIZE*block + size + 2] <<  8)
      | (EEPROM[EEPROM_BLOCK_SIZE*block + size + 3] <<  0);
 /* temporary debugging code only */
    printf("block %i:  (proper, stored) = (0x%08lX, 0x%08lX)\n",
        block, checksum_proper, checksum_stored);
    return (checksum_stored != checksum_proper); /* idea -- (stored - proper) */
}

static unsigned long get_Banjo_checksum(int block, int size)
{
    uint64_t AT, V0;
    uint64_t A1, A2, A3;
    uint64_t T0, T1, T2, T3, T4, T5, T6, T7, T8, T9;
    uint64_t S1, S3;
    unsigned char* bytes;
    int i;

    A3 = ((uint64_t)(0x00000001) << 32) | 0x3108B3C1;
    S1 = ((uint64_t)(0x00000000) << 32) | 0x00000000;
    S3 = ((uint64_t)(0x00000000) << 32) | 0x00000000;
    bytes = EEPROM + 8*block;

    for (i = 0; i < size; i++)
    {
        T8 = bytes[i]; /* LBU */
        T5 = A3 & 0xFFFFFFFF; /* LW */
        T9 = S1 & 0x000F;
        T0 = T8 << T9;
        T4 = A3 >> 32 ; /* LW */
        T7 = T0 + T5;
        T2 = T0 >> 0x1F;
        AT = (T7 < T5); /* SLTU */
        T6 = AT + T2;
        T6 = T6 + T4;
        /* After JAL */
        A3 = (T6 << 32) + T7; /* LD */
        A2 = A3 << (0x1F + 32);
        A1 = A3 << 0x1F;
        A2 = A2 >> 0x1F;
        A1 = A1 >> (0x0 + 32);
        A3 = A3 << (0xC + 32);
        A2 = A2 | A1;
        A3 = A3 >> (0x0 + 32);
        A2 = A2 ^ A3;
        A3 = A2 >> 0x14;
        A3 = A3 & 0xFFF;
        A3 = A3 ^ A2;
        V0 = A3 << (0x0 + 32);
        /* After JR */
        V0 = V0 >> (0x0 + 32); 
        S1 += 0x7;
        /* After BNE */
        S3 = S3 ^ V0;
    }
    for (i = size - 1; i >= 0; i--)
    {
        T1 = bytes[i]; /* LBU */
        T3 = A3 & 0xFFFFFFFF; /* LW */
        T8 = S1 & 0x000F;
        T9 = T1 << T8;
        T2 = A3 >> 32 ; /* LW */
        T5 = T9 + T3;
        T0 = T9 >> 0x1F;
        AT = (T5 < T3); /* SLTU */
        T4 = AT + T0;
        T4 = T4 + T2;
        /* After JAL */
        A3 = (T4 << 32) + T5; /* LD */
        A2 = A3 << (0x1F + 32);
        A1 = A3 << 0x1F;
        A2 = A2 >> 0x1F;
        A1 = A1 >> (0x0 + 32);
        A3 = A3 << (0xC + 32);
        A2 = A2 | A1;
        A3 = A3 >> (0x0 + 32);
        A2 = A2 ^ A3;
        A3 = A2 >> 0x14;
        A3 = A3 & 0xFFF;
        A3 = A3 ^ A2;
        V0 = A3 << (0x0 + 32);
        /* After JR */
        V0 = V0 >> (0x0 + 32);
        S1 += 0x3;
        /* After BNE */
        S3 = S3 ^ V0;
    }
    return (S3 & 0x00000000FFFFFFFF);
    return 0;
}
