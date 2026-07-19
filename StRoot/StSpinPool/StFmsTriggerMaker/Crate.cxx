#include "Crate.hh"
#include "RTS/trg/include/trgDataDefs.h"
#include <algorithm>
#include <cassert>
#include <byteswap.h>
#include <cstdio>
#include <functional>

void Crate::clear()
{
  std::for_each(boards, boards + NBOARDS, std::mem_fun_ref(&Board::clear));
}

void Crate::read(const TriggerDataBlk&, int)
{
  std::printf("CRATE::READ CALLED!!!\n");
}

void Crate::decodeQT(const QTBlock& qt, int crate, int t)
{
  int sz = qt.length / 4;
  assert(qt.data[sz - 1] == 0xac10);
  for (int i = 0; i < sz - 1;) {
    int h = qt.data[i++];
    assert(crate == (h >> 24 & 0xff));
    int nlines = h & 0xff;
    int addr = h >> 16 & 0xff;
    assert(0x10 <= addr && addr < 0x20);
    addr -= 0x10;
    while (nlines--) {
      int d = qt.data[i++];
      int ch = d >> 27 & 0x1f;
      assert(0 <= ch && ch < 32);
      boards[addr].channels[t][ch] = d & 0xfff;
    }
  }
}

unsigned long long Crate::swapLL(unsigned long long x)
{
  return ((x & 0xffff000000000000ull) >> 48 |
          (x & 0x0000ffff00000000ull) >> 16 |
          (x & 0x00000000ffff0000ull) << 16 |
          (x & 0x000000000000ffffull) << 48);
}

void Crate::copy_and_swap(unsigned char* dest, const unsigned char* src)
{
  unsigned long long* x = (unsigned long long*)src;
  unsigned long long* y = (unsigned long long*)dest;
  *y++ = bswap_64(*x++);
  *y++ = bswap_64(*x++);
}

void Crate::copy_and_swap(unsigned short* dest, const unsigned short* src)
{
  unsigned long long* x = (unsigned long long*)src;
  unsigned long long* y = (unsigned long long*)dest;
  *y++ = swapLL(*x++);
  *y++ = swapLL(*x++);
}

void Crate::unpack(unsigned short* dest, const unsigned char* src)
{
  const unsigned char* cpMax = src + 15;
  unsigned short* sp = dest;
  for (const unsigned char* cp = src; cp < cpMax; cp += 3) {
    const unsigned int* ip = (const unsigned int*)cp;
    *sp++ = *ip & 0xfff;
    *sp++ = *ip >> 12 & 0xfff;
  }
}
