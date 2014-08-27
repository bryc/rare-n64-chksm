/*
  BK/BT Algorithm (Javascript)
  -----------------------------
  This shows the checksum algorithm in pure JS, using the 32-bit code.
  Javascript's bitwise operations are fixed at 32-bits, and are signed
  unless you use the >>> operator (right shift unsigned).
  For the most part, it should behave like the C version of the 32-bit algo,
  unless you use the unsigned long type instead of unsigned int.
  
  The Closure library contains a constructor for 64-bit int operations:
  http://docs.closure-library.googlecode.com/git/class_goog_math_Long.html
*/
function bkchk(byteArray)
{
    function hx(a)
    {
        return ("00000000"+(a>>>0).toString(16).toUpperCase()).slice(-8);
    }
    
    function algo()
    {
        value   += byteArray[i] << (sd & 0xF);
        value2   = (value >>> 1 | carry << 31) ^ (value & 0xFFFFF) << 12;
        carry    = value & 0x1;
        value    = value2 ^ (value2 >> 20) & 0xFFF;
        checksum = value ^ checksum;
    }
    
    var i = 0, s = byteArray.length, value = 0x3108B3C1,
        sd = 0, value2, carry = 1, checksum = 0, bt;
    
    while(i < s)
    {
        algo();
        i++; sd += 7;
    }
        bt = checksum;
        i--;
    
    while(i >= 0)
    {
        algo();
        i--; sd += 3;
    }
    
    var out = {
        "BK" : hx(checksum),
        "BT" : hx(bt) + hx(bt ^ checksum)
    };
    return out;
}

var bytes = [
    // Real BK Checksum = 32C9A1E6
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
];

bkchk(bytes);
