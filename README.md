### Rare N64 Checksum Algorithm

This is the checksum algorithm used to detect corrupted save data in a number of Rare games. The _mixing function_ also appears to be used as a random number generator in some games. This is mostly useful for those into ROM hacking or modifying save files. Code is provided in C but there is a [online JS version](http://jsbin.com/bicare/quiet) you can just copy and paste hex bytes into.

This algorithm is known to be used in Banjo-Kazooie (type A), Goldeneye 007 (type B), Banjo-Tooie (type B) and Perfect Dark (type C). 

Type B is a 64-bit result consisting of `S4 + (S3 << 32)`, while Type A is a 32-bit result consisting of `S4 ^ S3`. Type C is a 32-bit result consisting of `S4 & 0xFFFF | (S3 & 0xFFFF) << 16`. Perfect Dark also seems to use yet another variant in certain sections of EEPROM that I haven't figured out yet.
