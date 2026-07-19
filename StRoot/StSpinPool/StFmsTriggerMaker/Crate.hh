//
// Pibero Djawotho <pibero@tamu.edu>
// Texas A&M
// 16 July 2012
//

#ifndef CRATE_HH
#define CRATE_HH

// Keep low-level RTS trigger definitions out of ROOT 5 dictionary input.
struct TriggerDataBlk;
struct QTBlock;

// L1 crate
const int TF201_BASE_ADDRESS = 0x10000000;
const int VT201_BASE_ADDRESS = 0x12000000;
const int EM201_BASE_ADDRESS = 0x14000000;
const int EM202_BASE_ADDRESS = 0x15000000;
const int BX201_BASE_ADDRESS = 0x16000000;
const int BX202_BASE_ADDRESS = 0x18000000;
const int ST201_BASE_ADDRESS = 0x1a000000;
const int FP201_BASE_ADDRESS = 0x1c000000;
const int LD301_BASE_ADDRESS = 0x1e000000;

// BC1 crate
const int BC101_BASE_ADDRESS = 0x21000000;
const int BC102_BASE_ADDRESS = 0x10000000;
const int BC103_BASE_ADDRESS = 0x11000000;
const int BC104_BASE_ADDRESS = 0x12000000;
const int BC105_BASE_ADDRESS = 0x13000000;
const int BC106_BASE_ADDRESS = 0x14000000;
const int EE101_BASE_ADDRESS = 0x15000000;
const int EE102_BASE_ADDRESS = 0x16000000;
const int EE001_BASE_ADDRESS = 0x17000000;
const int EE002_BASE_ADDRESS = 0x18000000;
const int EE003_BASE_ADDRESS = 0x19000000;
const int EE004_BASE_ADDRESS = 0x1a000000;
const int EE005_BASE_ADDRESS = 0x1b000000;
const int EE006_BASE_ADDRESS = 0x1c000000;
const int EE007_BASE_ADDRESS = 0x1d000000;
const int EE008_BASE_ADDRESS = 0x1e000000;
const int EE009_BASE_ADDRESS = 0x1f000000;

// MIX crate
const int FE101_BASE_ADDRESS = 0x10000000;
const int MT101_BASE_ADDRESS = 0x11000000;
const int TF101_BASE_ADDRESS = 0x12000000;
const int TF001_BASE_ADDRESS = 0x14000000;
const int TF002_BASE_ADDRESS = 0x16000000;
const int TF003_BASE_ADDRESS = 0x18000000;
const int TF004_BASE_ADDRESS = 0x1a000000;
const int TF005_BASE_ADDRESS = 0x1c000000;
const int TF006_BASE_ADDRESS = 0x1e000000;

// BCW crate
const int BW001_BASE_ADDRESS = 0x10000000;
const int BW002_BASE_ADDRESS = 0x11000000;
const int BW003_BASE_ADDRESS = 0x12000000;
const int BW004_BASE_ADDRESS = 0x13000000;
const int BW005_BASE_ADDRESS = 0x14000000;
const int BW006_BASE_ADDRESS = 0x15000000;
const int BW007_BASE_ADDRESS = 0x16000000;
const int BW008_BASE_ADDRESS = 0x17000000;
const int BW009_BASE_ADDRESS = 0x18000000;
const int BW010_BASE_ADDRESS = 0x19000000;
const int BW011_BASE_ADDRESS = 0x1a000000;
const int BW012_BASE_ADDRESS = 0x1b000000;
const int BW013_BASE_ADDRESS = 0x1c000000;
const int BW014_BASE_ADDRESS = 0x1d000000;
const int BW015_BASE_ADDRESS = 0x1e000000;

// BCE crate
const int BE001_BASE_ADDRESS = 0x10000000;
const int BE002_BASE_ADDRESS = 0x11000000;
const int BE003_BASE_ADDRESS = 0x12000000;
const int BE004_BASE_ADDRESS = 0x13000000;
const int BE005_BASE_ADDRESS = 0x14000000;
const int BE006_BASE_ADDRESS = 0x15000000;
const int BE007_BASE_ADDRESS = 0x16000000;
const int BE008_BASE_ADDRESS = 0x17000000;
const int BE009_BASE_ADDRESS = 0x18000000;
const int BE010_BASE_ADDRESS = 0x19000000;
const int BE011_BASE_ADDRESS = 0x1a000000;
const int BE012_BASE_ADDRESS = 0x1b000000;
const int BE013_BASE_ADDRESS = 0x1c000000;
const int BE014_BASE_ADDRESS = 0x1d000000;
const int BE015_BASE_ADDRESS = 0x1e000000;

// FEQ crate

const int FE001_BASE_ADDRESS = 0x10000000;
const int FE002_BASE_ADDRESS = 0x11000000;
const int FE003_BASE_ADDRESS = 0x12000000;
const int FE004_BASE_ADDRESS = 0x13000000;
const int FS001_BASE_ADDRESS = 0x19000000;
const int FS002_BASE_ADDRESS = 0x1a000000;
const int FS003_BASE_ADDRESS = 0x1b000000;
const int FS004_BASE_ADDRESS = 0x1c000000;
const int FS005_BASE_ADDRESS = 0x1d000000;
const int FS006_BASE_ADDRESS = 0x1e000000;

// BBC crate
const int BB101_BASE_ADDRESS = 0x10000000;
const int BB102_BASE_ADDRESS = 0x12000000;
const int ZD101_BASE_ADDRESS = 0x14000000;
const int VP101_BASE_ADDRESS = 0x16000000;

// FMS crate
#define RUN15
#ifndef RUN15
const int FM001_BASE_ADDRESS = 0x10000000;
const int FM002_BASE_ADDRESS = 0x11000000;
const int FM003_BASE_ADDRESS = 0x12000000;
const int FM004_BASE_ADDRESS = 0x13000000;
const int FM101_BASE_ADDRESS = 0x14000000;
const int FM005_BASE_ADDRESS = 0x15000000;
const int FM006_BASE_ADDRESS = 0x16000000;
const int FM007_BASE_ADDRESS = 0x17000000;
const int FM008_BASE_ADDRESS = 0x18000000;
const int FM102_BASE_ADDRESS = 0x19000000;
const int FM009_BASE_ADDRESS = 0x1a000000;
const int FM010_BASE_ADDRESS = 0x1b000000;
const int FM011_BASE_ADDRESS = 0x1c000000;
const int FM012_BASE_ADDRESS = 0x1d000000;
const int FM103_BASE_ADDRESS = 0x1e000000;
#endif
#ifdef RUN15
const int FM001_BASE_ADDRESS = 0x21000000;
const int FM002_BASE_ADDRESS = 0x10000000;
const int FM101_BASE_ADDRESS = 0x11000000;
const int FM003_BASE_ADDRESS = 0x12000000;
const int FM004_BASE_ADDRESS = 0x13000000;
const int FM102_BASE_ADDRESS = 0x14000000;
const int FM005_BASE_ADDRESS = 0x15000000;
const int FM006_BASE_ADDRESS = 0x16000000;
const int FM007_BASE_ADDRESS = 0x17000000;
const int FM008_BASE_ADDRESS = 0x18000000;
const int FM103_BASE_ADDRESS = 0x19000000;
const int FM009_BASE_ADDRESS = 0x1a000000;
const int FM010_BASE_ADDRESS = 0x1b000000;
const int FM011_BASE_ADDRESS = 0x1c000000;
const int FM012_BASE_ADDRESS = 0x1d000000;
const int FM104_BASE_ADDRESS = 0x1e000000;
#endif

// BBQ crate
const int BB001_BASE_ADDRESS = 0x10000000;
const int BB002_BASE_ADDRESS = 0x12000000;
const int BB003_BASE_ADDRESS = 0x14000000;
const int VP001_BASE_ADDRESS = 0x16000000;
const int VP002_BASE_ADDRESS = 0x18000000;
const int ZD001_BASE_ADDRESS = 0x1e000000;

// MXQ crate
const int MT001_BASE_ADDRESS = 0x10000000;
const int PP001_BASE_ADDRESS = 0x12000000;
const int ZP001_BASE_ADDRESS = 0x14000000;
const int VP003_BASE_ADDRESS = 0x16000000;
const int VP004_BASE_ADDRESS = 0x18000000;
const int MT002_BASE_ADDRESS = 0x1a000000;

struct Crate {
  enum { NBOARDS = 18 };

  Board boards[NBOARDS];

  //const char* name() const { return crateNames[id]; }
  const Board& boardAt(int i) const { return boards[(i>>24)-0x10]; }
  Board& boardAt(int i) { return boards[(i>>24)-0x10]; }
  void clear();
  void read(const TriggerDataBlk& event, int id);
  void decodeQT(const QTBlock& qt, int crate, int t=MAXPP);
  unsigned long long swapLL(unsigned long long x);
  void copy_and_swap(unsigned char* dest, const unsigned char* src);
  void copy_and_swap(unsigned short* dest, const unsigned short* src);
  void unpack(unsigned short* dest, const unsigned char* src);
};

#endif // CRATE_HH
