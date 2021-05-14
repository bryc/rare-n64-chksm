### Rare N64 Checksum Algorithm

This is the checksum algorithm used to detect corrupted save data in a number of Rare games. The _mixing function_ also appears to be used as a random number generator in some games. This is mostly useful for those into ROM hacking or modifying save files. Code is provided in C but there is a [online JS version](http://jsbin.com/bicare/quiet) you can just copy and paste hex bytes into.

This algorithm is known to be used in Goldeneye 007 (type A),  Banjo-Kazooie (type B), Perfect Dark (type C) and, Banjo-Tooie (type A) . 

Game | Release date | Final checksum formula
-- | -- | --
GoldenEye 007 | Aug-25-1997 | A, `(u64) (S3 << 32) \| S4`
Banjo-Kazooie | Jun-29-1998 | B, `(u32) S3 ^ S4`
Perfect Dark  | May-22-2000 | C, `(u32) (S3 << 16) \| (S4 & 0xFFFF)`
Banjo-Tooie   | Nov-20-2000 | A, `(u64) (S3 << 32) \| S4`

Note: Perfect Dark has an EEPROM bug where empty space is overwritten with unused duplicate data. This means you need specific lengths to know when to stop reading data. The true lengths for these data sections are **96-5**, **80-2** and **64-15**. The other two lengths which don't contain empty space are **160** and **8**. The other games are relatively straightforward for finding their offsets so I won't bother explaining the memory layout here...
