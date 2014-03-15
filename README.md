### Banjo-Kazooie EEPROM Checksum Algorithm

In the past, I became interested in the little .eep save files that N64 emulators created. This was how N64 carts could load your saved games, and apparently the data in .eep files were exactly the same used in the actual cartridges. I thought it would be cool to figure out how these save files worked and see *exactly* what stuff was saved.

I started off with Super Mario 64, and eventually figured out almost everything about it. One thing I found was that it stored a checksum, a simple sum of the bytes in a specific region in the file. Different games can have different checksum methods. Pilotwings 64 used a simple sum, but stored as a single byte that constantly overflowed, thus changing the final value. Super Mario World subtracts the sum from a very specific initial value, giving the checksum. Donkey Kong 64 uses a standard CRC-32.

When I came across Banjo-Kazooie, I was stumped. It looked like a CRC-32, but I couldn't figure out how to recalculate it. I eventually learned that it *couldn't* be a CRC-32 due to a kind of superposition principle. I decided it was time to get my hands dirty and actually try debugging the game in an emulator and isolate the checksum algorithm.

After tons of messing about with Nemu64, I eventually made sense of the registers and breakpoints and memory addresses, and started manually writing out assembly instructions and the register values. I changed values in memory and in the registers while going from breakpoint to breakpoint, and eventually saw how it affected the final checksum. Once I got all the assembly instructions logged, I used some N64ops.txt document to tell me what each instruction did, and even had C-like examples to help translating code.

After everything was said and done, I translated the asm line-by-line to C, and eventually saw the checksum output correctly. I had done it. I deciphered a friggin' checksum algorithm from a damn N64 game.

I hardly understand what it's doing besides doing tons of shifting, ANDs, and XORs, but it could very well be a custom algorithm made by  the developers.

I later learned thanks to cxd4 that **Banjo-Tooie** pretty much used the exact same algorithm. I was oblivious since it initially seened to be a 64-bit checksum, but it was simply two checksums using a *(very)* slightly modified version of the BK algorithm. I literally only had to change two lines to the original.
