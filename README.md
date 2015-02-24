### Rare's Checksum Algorithm
This is an implementation of Rare's checksum algorithm in plain C. The algorithm was reverse-engineered from raw MIPS assembly using Nemu64's debugger. It is used in a couple of Rare's N64 titles to verify data integrity, notably in the EEPROM data. 

Valid checksums can be generated from any number of bytes. This effectively allows checksums to be correct when arbitrarily modifying save data, or implemented in a save editor application.

Generally speaking, the algorithm runs twice; once forwards, saving the result in `S3`, and then continuing in reverse, saving the result in `S4`. Both Goldeneye and Banjo-Tooie use `(S3 << 8) + S4` as the final checksum, resembling a 64-bit checksum, whereas Banjo-Kazooie uses `S3  ^ S4`; both checksums are XORed, resembling a 32-bit checksum.


Games that use this checksum:
  1. Banjo Kazooie
  2. Banjo Tooie
  3. Goldeneye 007 (same checksum as Tooie)

**Online (JavaScript) version [here](http://jsbin.com/bicare/quiet)**
