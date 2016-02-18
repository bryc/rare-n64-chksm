### *Rare's* Checksum Algorithm

This is an implementation of Rare's checksum algorithm in C and Javascript. The checksum is used in a couple of Rare's N64 titles to verify EEPROM data integrity (and possibly used elsewhere in memory). This came about when trying to modify Banjo-Kazooie saves. I had no clue what checksum was being used in them, so I used Nemu64's disassembler to find the code responsible for calculating it, and reproduced it in C so I could update the checksums.

Highly useful for anyone wanting to modify save files for the below games, or just a general checksum function on any length of input.

Games that use this checksum:
  1. Banjo Kazooie (type A)
  2. Banjo Tooie (type B)
  3. Goldeneye 007 (type B)

**Online (JavaScript) version [here](http://jsbin.com/bicare/quiet)**
