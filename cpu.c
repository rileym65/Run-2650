#include <stdio.h>
#include "cpu.h"

extern byte ram[32768];

byte readMem(word address) {
  return ram[address];
  }

void writeMem(word address, byte value) {
  ram[address] = value;
  }

void _setCC(CPU* cpu, byte v) {
  cpu->psl &= 0x3f;
  if (v & 0x80) cpu->psl |= 0x80;
  else if (v > 0) cpu->psl |= 0x40;
  }

byte _fetch(CPU* cpu) {
  byte b;
  b = readMem(cpu->page + cpu->pc++);
  cpu->pc &= 0x1fff;
  return b;
  }

word _relative(CPU* cpu) {
  word a;
  word b;
  word i;
  b = _fetch(cpu);
  a = b & 0x7f;
  if (a & 0x40) a |= 0xff80;
  a = (a + cpu->pc) & 0x1fff;
  if ((b & 0x80) == 0) return cpu->page + a;
  i = readMem(cpu->page+a) << 8;
  a = (a + 1) & 0x3fff;
  i |= readMem(cpu->page+a);
  i = i & 0x7fff;
  cpu->ts+=6;
  return i;
  }

word _absolute(CPU* cpu) {
  word a;
  byte b;
  word i;
  byte r;
  byte ofs;
  b = _fetch(cpu);
  a = (b & 0x1f) << 8;
  a |= _fetch(cpu);
  ofs = 0;
  if ((b & 0x60) != 0x00) {
    r = cpu->rs + (cpu->op & 0x3);
    if ((b & 0x60) == 0x20) cpu->r[r]++;
    if ((b & 0x60) == 0x40) cpu->r[r]--;
    ofs = cpu->r[r];
    }
  if ((b & 0x80) == 0) {
    a = (a + ofs) & 0x3fff;
    return cpu->page + a;
    }
  i = readMem(cpu->page+a) << 8;
  a = (a + 1) & 0x3fff;
  i |= readMem(cpu->page+a);
  i = (i + ofs) & 0x7fff;
  cpu->ts+=6;
  return i;
  }

word _absoluteBR(CPU* cpu) {
  word a;
  byte b;
  word i;
  b = _fetch(cpu);
  a = (b & 0x7f) << 8;
  a |= _fetch(cpu);
  if ((b & 0x80) == 0) {
    return a;
    }
  i = readMem(a) << 8;
  a = (a + 1);
  i |= readMem(a);
  cpu->ts+=6;
  return i;
  }

byte _add(CPU* cpu,byte a, byte b) {
  word wa,wb,wc;
  wa = a;
  wb = b;
  wc = wa + wb;
  if (cpu->psl & FLAG_WC) wc += (cpu->psl & 0x01);
  if ((wc & 0x0f) < (wa & 0x0f)) cpu->psl |= FLAG_IDC;
    else cpu->psl &= (~FLAG_IDC);
  if (wc >0xff) cpu->psl |= FLAG_C;
    else cpu->psl &= (~FLAG_C);
  cpu->psl &= 0x3f;
  if (wc & 0x80) cpu->psl |= 0x80;
  else if ((wc & 0xff) != 0) cpu->psl |= 0x40;
  if ((a & 0x80) == (b & 0x80) && (a & 0x80) != (wc &0x80)) cpu->psl |= FLAG_OVF;
    else cpu->psl &= (~FLAG_OVF);
  return wc & 0xff;
  }

byte _sub(CPU* cpu, byte a, byte b) {
  return _add(cpu, a, (b ^ 0xff) + 1);
  }

byte _and(CPU* cpu, byte a, byte b) {
  a &= b;
  cpu->psl &= 0x3f;
  if (a & 0x80) cpu->psl |= 0x80;
  else if ((a & 0xff) != 0) cpu->psl |= 0x40;
  return a;
  }

void _tmi(CPU* cpu, byte a, byte b) {
  cpu->psl &= 0x3f;
  if ((a & b) != b) {
    cpu->psl |= 0x80;
    }
  }

byte _ior(CPU* cpu, byte a, byte b) {
  a |= b;
  cpu->psl &= 0x3f;
  if (a & 0x80) cpu->psl |= 0x80;
  else if ((a & 0xff) != 0) cpu->psl |= 0x40;
  return a;
  }

byte _eor(CPU* cpu, byte a, byte b) {
  a ^= b;
  cpu->psl &= 0x3f;
  if (a & 0x80) cpu->psl |= 0x80;
  else if ((a & 0xff) != 0) cpu->psl |= 0x40;
  return a;
  }

void _com(CPU* cpu, byte a, byte b) {
  cpu->psl &= 0x3f;
  if (a == b) return;
  if (cpu->psl & FLAG_COM) {
    if (a > b) cpu->psl |= 0x40;
    else cpu->psl |= 0x80;
    }
  else {
    if (a < 0x80 && b >= 0x80) cpu->psl |= 0x40;
    else if (a >= 0x80 && b < 0x80) cpu->psl |= 0x80;
    else {
      if (a > b) cpu->psl |= 0x40; else cpu->psl |= 0x80;
      }
    }
  }

byte _rrl(CPU* cpu, byte a) {
  byte c;
  if (((a & 0x80) >> 7) != ((a & 0x40) >> 6)) cpu->psl |= FLAG_OVF;
    else cpu->psl &= (~FLAG_OVF);
  if (cpu->psl & FLAG_WC) {
    if (a & 0x10) cpu->psl |= FLAG_IDC;
      else cpu->psl &= (~FLAG_IDC);
    c = (a & 0x80) >> 7;
    a = (a << 1) | (cpu->psl & FLAG_C);
    cpu->psl = (cpu->psl & 0xfe) | c;
    }
  else {
    c = (a & 0x80) >> 7;
    a = (a << 1) | c;
    }
  cpu->psl &= 0x3f;
  if (a & 0x80) cpu->psl |= 0x80;
  else if ((a & 0xff) != 0) cpu->psl |= 0x40;
  return a;
  }

byte _rrr(CPU* cpu, byte a) {
  byte c;
  if (cpu->psl & FLAG_WC) {
    if (a & 0x40) cpu->psl |= FLAG_IDC;
      else cpu->psl &= (~FLAG_IDC);
    c = a & 0x01;
    a = (a >> 1) | ((cpu->psl & FLAG_C) << 7);
    cpu->psl = (cpu->psl & 0xfe) | c;
    }
  else {
    c = (a & 0x01) << 7;
    a = (a >> 1) | c;
    }
  if (((a & 0x80) >> 7) != ((a & 0x40) >> 6)) cpu->psl |= FLAG_OVF;
    else cpu->psl &= (~FLAG_OVF);
  cpu->psl &= 0x3f;
  if (a & 0x80) cpu->psl |= 0x80;
  else if ((a & 0xff) != 0) cpu->psl |= 0x40;
  return a;
  }

void _P0(CPU *cpu) {                   /* LODZ  0 */
  }

byte _dar(CPU* cpu, byte a) {
  if ((cpu->psl & FLAG_C) == 0 && (cpu->psl & FLAG_IDC) == 0) a += 0xaa;
  if ((cpu->psl & FLAG_C) == 0 && (cpu->psl & FLAG_IDC) != 0) a += 0xa0;
  if ((cpu->psl & FLAG_C) != 0 && (cpu->psl & FLAG_IDC) == 0) a += 0x0a;

  cpu->psl &= 0x3f;
  if (a & 0x80) cpu->psl |= 0x80;
  else if ((a & 0xff) != 0) cpu->psl |= 0x40;
  return a;
  }

void _P1(CPU *cpu) {                   /* LODZ  1 */
  cpu->r[0] = (cpu->psl & 0x10) ? cpu->r[4] : cpu->r[1];
  _setCC(cpu, cpu->r[0]);
  cpu->ts += 6;
  }

void _P2(CPU *cpu) {                   /* LODZ  2 */
  cpu->r[0] = (cpu->psl & 0x10) ? cpu->r[5] : cpu->r[2];
  _setCC(cpu, cpu->r[0]);
  cpu->ts += 6;
  }

void _P3(CPU *cpu) {                   /* LODZ  3 */
  cpu->r[0] = (cpu->psl & 0x10) ? cpu->r[6] : cpu->r[3];
  _setCC(cpu, cpu->r[0]);
  cpu->ts += 6;
  }

void _P4(CPU *cpu) {                   /* LODI,0 nn */
  cpu->r[0] = _fetch(cpu);
  _setCC(cpu, cpu->r[cpu->rs + 0]);
  cpu->ts += 6;
  }

void _P5(CPU *cpu) {                   /* LODI,1 nn */
  cpu->r[cpu->rs + 1] = _fetch(cpu);
  _setCC(cpu, cpu->r[cpu->rs + 1]);
  cpu->ts += 6;
  }

void _P6(CPU *cpu) {                   /* LODI,2 nn */
  cpu->r[cpu->rs + 2] = _fetch(cpu);
  _setCC(cpu, cpu->r[cpu->rs + 2]);
  cpu->ts += 6;
  }

void _P7(CPU *cpu) {                   /* LODI,3 nn */
  cpu->r[cpu->rs + 3] = _fetch(cpu);
  _setCC(cpu, cpu->r[cpu->rs + 3]);
  cpu->ts += 6;
  }

void _P8(CPU *cpu) {                   /* LODR,0 */
  word a;
  a = _relative(cpu);
  cpu->r[0] = readMem(a);
  _setCC(cpu, cpu->r[cpu->rs + 0]);
  cpu->ts += 9;
  }

void _P9(CPU *cpu) {                   /* LODR,1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs + 1] = readMem(a);
  _setCC(cpu, cpu->r[cpu->rs + 1]);
  cpu->ts += 9;
  }

void _PA(CPU *cpu) {                   /* LODR,2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs + 2] = readMem(a);
  _setCC(cpu, cpu->r[cpu->rs + 2]);
  cpu->ts += 9;
  }

void _PB(CPU *cpu) {                   /* LODR,3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs + 3] = readMem(a);
  _setCC(cpu, cpu->r[cpu->rs + 3]);
  cpu->ts += 9;
  }

void _PC(CPU *cpu) {                   /* LODA,0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = readMem(a);
    _setCC(cpu, cpu->r[0]);
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] = readMem(a);
    _setCC(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }
    
void _PD(CPU *cpu) {                   /* LODA,1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = readMem(a);
    _setCC(cpu, cpu->r[0]);
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] = readMem(a);
    _setCC(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }

void _PE(CPU *cpu) {                   /* LODA,2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = readMem(a);
    _setCC(cpu, cpu->r[0]);
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] = readMem(a);
    _setCC(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }

void _PF(CPU *cpu) {                   /* LODA,3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = readMem(a);
    _setCC(cpu, cpu->r[0]);
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] = readMem(a);
    _setCC(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }

void _P10(CPU *cpu) {
  }

void _P11(CPU *cpu) {
  }

void _P12(CPU *cpu) {                  /* SPSU */
  cpu->psu &= 0x7f;
  if (cpu->sense) cpu->psu |= FLAG_S;
  cpu->r[0] = cpu->psu & 0xe7;
  cpu->psl &= 0x3f;
  if (cpu->r[0] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[0] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P13(CPU *cpu) {                  /* SPSL */
  cpu->r[0] = cpu->psl;
  cpu->psl &= 0x3f;
  if (cpu->r[0] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[0] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P14(CPU *cpu) {                  /* RETC,0 */
  if ((cpu->psl & 0xc0) == 0x00) {
    cpu->pc = cpu->stack[cpu->psu & 0x7];
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
    }
  cpu->ts += 9;
  }

void _P15(CPU *cpu) {                  /* RETC,1 */
  if ((cpu->psl & 0xc0) == 0x40) {
    cpu->pc = cpu->stack[cpu->psu & 0x7];
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
    }
  cpu->ts += 9;
  }

void _P16(CPU *cpu) {                  /* RETC,2 */
  if ((cpu->psl & 0xc0) == 0x80) {
    cpu->pc = cpu->stack[cpu->psu & 0x7];
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
    }
  cpu->ts += 9;
  }

void _P17(CPU *cpu) {                  /* RETC,3 */
  cpu->pc = cpu->stack[cpu->psu & 0x7];
  cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
  cpu->ts += 9;
  }

void _P18(CPU *cpu) {                  /* BCTR,0 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) == 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _P19(CPU *cpu) {                  /* BCTR,1 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) == 0x40) cpu->pc = a;
  cpu->ts += 9;
  }

void _P1A(CPU *cpu) {                  /* BCTR,2 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) == 0x80) cpu->pc = a;
  cpu->ts += 9;
  }

void _P1B(CPU *cpu) {                  /* BCTR,3 */
  word a;
  a = _relative(cpu);
  cpu->pc = a;
  cpu->ts += 9;
  }

void _P1C(CPU *cpu) {                  /* BCTA,0 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P1D(CPU *cpu) {                  /* BCTA,1 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x40) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P1E(CPU *cpu) {                  /* BCTA,2 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x80) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P1F(CPU *cpu) {                  /* BCTA,3 */
  word a;
  a = _absoluteBR(cpu);
  cpu->pc = a & 0x1fff;
  cpu->page = a & 0xe000;
  cpu->ts += 9;
  }

void _P20(CPU *cpu) {                  /* EORZ 0 */
  cpu->r[0] = _eor(cpu, cpu->r[0], cpu->r[0]);
  cpu->ts += 6;
  }

void _P21(CPU *cpu) {                  /* EORZ 1 */
  cpu->r[0] = _eor(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[4] : cpu->r[1]);
  cpu->ts += 6;
  }

void _P22(CPU *cpu) {                  /* EORZ 2 */
  cpu->r[0] = _eor(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[5] : cpu->r[2]);
  cpu->ts += 6;
  }

void _P23(CPU *cpu) {                  /* EORZ 3 */
  cpu->r[0] = _eor(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[6] : cpu->r[3]);
  cpu->ts += 6;
  }

void _P24(CPU *cpu) {                  /* EORI 0 */
  cpu->r[0] = _eor(cpu, cpu->r[0], _fetch(cpu));
  cpu->ts += 6;
  }

void _P25(CPU *cpu) {                  /* EORI 1 */
  cpu->r[cpu->rs + 1] = _eor(cpu, cpu->r[cpu->rs + 1], _fetch(cpu));
  cpu->ts += 6;
  }

void _P26(CPU *cpu) {                  /* EORI 2 */
  cpu->r[cpu->rs + 2] = _eor(cpu, cpu->r[cpu->rs + 2], _fetch(cpu));
  cpu->ts += 6;
  }

void _P27(CPU *cpu) {                  /* EORI 3 */
  cpu->r[cpu->rs + 3] = _eor(cpu, cpu->r[cpu->rs + 3], _fetch(cpu));
  cpu->ts += 6;
  }

void _P28(CPU *cpu) {                  /* EORR 0 */
  word a;
  a = _relative(cpu);
  cpu->r[0] = _eor(cpu, cpu->r[0], readMem(a));
  cpu->ts += 9;
  }

void _P29(CPU *cpu) {                  /* EORR 1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+1] = _eor(cpu, cpu->r[cpu->rs+1], readMem(a));
  cpu->ts += 9;
  }

void _P2A(CPU *cpu) {                  /* EORR 2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+2] = _eor(cpu, cpu->r[cpu->rs+2], readMem(a));
  cpu->ts += 9;
  }

void _P2B(CPU *cpu) {                  /* EORR 3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+3] = _eor(cpu, cpu->r[cpu->rs+3], readMem(a));
  cpu->ts += 9;
  }

void _P2C(CPU *cpu) {                  /* EORA 0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _eor(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _eor(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P2D(CPU *cpu) {                  /* EORA 1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _eor(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _eor(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P2E(CPU *cpu) {                  /* EORA 2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _eor(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _eor(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P2F(CPU *cpu) {                  /* EORA 3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _eor(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _eor(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P30(CPU *cpu) {                  /* REDC 0 */
  cpu->r[0] = cpu->control;
  cpu->psl &= 0x3f;
  if (cpu->r[0] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[0] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P31(CPU *cpu) {                  /* REDC 1 */
  cpu->r[cpu->rs+1] = cpu->control;
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+1] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+1] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P32(CPU *cpu) {                  /* REDC 2 */
  cpu->r[cpu->rs+2] = cpu->control;
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+2] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+2] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P33(CPU *cpu) {                  /* REDC 3 */
  cpu->r[cpu->rs+3] = cpu->control;
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+3] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+3] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P34(CPU *cpu) {                  /* RETE,0 */
  if ((cpu->psl & 0xc0) == 0x00) {
    cpu->pc = cpu->stack[cpu->psu & 0x7];
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
    cpu->psu &= (~FLAG_II);
    }
  cpu->ts += 9;
  }

void _P35(CPU *cpu) {                  /* RETE,1 */
  if ((cpu->psl & 0xc0) == 0x40) {
    cpu->pc = cpu->stack[cpu->psu & 0x7];
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
    cpu->psu &= (~FLAG_II);
    }
  cpu->ts += 9;
  }

void _P36(CPU *cpu) {                  /* RETE,2 */
  if ((cpu->psl & 0xc0) == 0x80) {
    cpu->pc = cpu->stack[cpu->psu & 0x7];
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
    cpu->psu &= (~FLAG_II);
    }
  cpu->ts += 9;
  }

void _P37(CPU *cpu) {                  /* RETE,3 */
  cpu->pc = cpu->stack[cpu->psu & 0x7];
  cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) - 1) & 0x7);
  cpu->psu &= (~FLAG_II);
  cpu->ts += 9;
  }

void _P38(CPU *cpu) {                  /* BSTR,0 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) == 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _P39(CPU *cpu) {                  /* BSTR,1 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) == 0x40) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _P3A(CPU *cpu) {                  /* BSTR,2 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) == 0x80) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _P3B(CPU *cpu) {                  /* BSTR,3 */
  word a;
  a = _relative(cpu);
  cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
  cpu->stack[cpu->psu & 0x7] = cpu->pc;
  cpu->pc = a;
  cpu->ts += 9;
  }

void _P3C(CPU *cpu) {                  /* BSTA,0 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P3D(CPU *cpu) {                  /* BSTA,1 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x40) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P3E(CPU *cpu) {                  /* BSTA,2 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x80) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P3F(CPU *cpu) {                  /* BSTA,3 */
  word a;
  a = _absoluteBR(cpu);
  cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
  cpu->stack[cpu->psu & 0x7] = cpu->pc;
  cpu->pc = a & 0x1fff;
  cpu->page = a & 0xe000;
  cpu->ts += 9;
  }

void _P40(CPU *cpu) {
  cpu->halt = 0xff;
  }

void _P41(CPU *cpu) {                  /* ANDZ 1 */
  cpu->r[0] = _and(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[4] : cpu->r[1]);
  cpu->ts += 6;
  }

void _P42(CPU *cpu) {                  /* ANDZ 2 */
  cpu->r[0] = _and(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[5] : cpu->r[2]);
  cpu->ts += 6;
  }

void _P43(CPU *cpu) {                  /* ANDZ 3 */
  cpu->r[0] = _and(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[6] : cpu->r[3]);
  cpu->ts += 6;
  }

void _P44(CPU *cpu) {                  /* ANDI 0 */
  cpu->r[0] = _and(cpu, cpu->r[0], _fetch(cpu));
  cpu->ts += 6;
  }

void _P45(CPU *cpu) {                  /* ANDI 1 */
  cpu->r[cpu->rs + 1] = _and(cpu, cpu->r[cpu->rs + 1], _fetch(cpu));
  cpu->ts += 6;
  }

void _P46(CPU *cpu) {                  /* ANDI 2 */
  cpu->r[cpu->rs + 2] = _and(cpu, cpu->r[cpu->rs + 2], _fetch(cpu));
  cpu->ts += 6;
  }

void _P47(CPU *cpu) {                  /* ANDI 3 */
  cpu->r[cpu->rs + 3] = _and(cpu, cpu->r[cpu->rs + 3], _fetch(cpu));
  cpu->ts += 6;
  }

void _P48(CPU *cpu) {                  /* ANDR 0 */
  word a;
  a = _relative(cpu);
  cpu->r[0] = _and(cpu, cpu->r[0], readMem(a));
  cpu->ts += 9;
  }

void _P49(CPU *cpu) {                  /* ANDR 1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+1] = _and(cpu, cpu->r[cpu->rs+1], readMem(a));
  cpu->ts += 9;
  }

void _P4A(CPU *cpu) {                  /* ANDR 2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+2] = _and(cpu, cpu->r[cpu->rs+2], readMem(a));
  cpu->ts += 9;
  }

void _P4B(CPU *cpu) {                  /* ANDR 3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+3] = _and(cpu, cpu->r[cpu->rs+3], readMem(a));
  cpu->ts += 9;
  }

void _P4C(CPU *cpu) {                  /* ANDA 0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _and(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _and(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P4D(CPU *cpu) {                  /* ANDA 1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _and(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _and(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P4E(CPU *cpu) {                  /* ANDA 2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _and(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _and(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P4F(CPU *cpu) {                  /* ANDA 3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _and(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _and(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P50(CPU *cpu) {                  /* RRR,R0 */
  cpu->r[0] = _rrr(cpu, cpu->r[0]);
  cpu->ts += 6;
  }

void _P51(CPU *cpu) {                  /* RRR,R1 */
  if (cpu->psl & 0x10)
    cpu->r[4] = _rrr(cpu, cpu->r[4]);
  else
    cpu->r[1] = _rrr(cpu, cpu->r[1]);
  cpu->ts += 6;
  }

void _P52(CPU *cpu) {                  /* RRR,R2 */
  if (cpu->psl & 0x10)
    cpu->r[5] = _rrr(cpu, cpu->r[5]);
  else
    cpu->r[2] = _rrr(cpu, cpu->r[2]);
  cpu->ts += 6;
  }

void _P53(CPU *cpu) {                  /* RRR,R3 */
  if (cpu->psl & 0x10)
    cpu->r[6] = _rrr(cpu, cpu->r[6]);
  else
    cpu->r[3] = _rrr(cpu, cpu->r[3]);
  cpu->ts += 6;
  }

void _P54(CPU *cpu) {                  /* REDE 0 */
  byte b;
  b = _fetch(cpu);
  cpu->r[0] = cpu->ports[b];
  cpu->psl &= 0x3f;
  if (cpu->r[0] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[0] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 9;
  }

void _P55(CPU *cpu) {                  /* REDE 1 */
  byte b;
  b = _fetch(cpu);
  cpu->r[cpu->rs+1] = cpu->ports[b];
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+1] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+1] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 9;
  }

void _P56(CPU *cpu) {                  /* REDE 2 */
  byte b;
  b = _fetch(cpu);
  cpu->r[cpu->rs+2] = cpu->ports[b];
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+2] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+2] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 9;
  }

void _P57(CPU *cpu) {                  /* REDE 3 */
  byte b;
  b = _fetch(cpu);
  cpu->r[cpu->rs+3] = cpu->ports[b];
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+3] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+3] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 9;
  }

void _P58(CPU *cpu) {                  /* BRNR 0 */
  word a;
  a = _relative(cpu);
  if (cpu->r[0] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _P59(CPU *cpu) {                  /* BRNR 1 */
  word a;
  a = _relative(cpu);
  if (cpu->r[cpu->rs+1] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _P5A(CPU *cpu) {                  /* BRNR 2 */
  word a;
  a = _relative(cpu);
  if (cpu->r[cpu->rs+2] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _P5B(CPU *cpu) {                  /* BRNR 3 */
  word a;
  a = _relative(cpu);
  if (cpu->r[cpu->rs+3] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _P5C(CPU *cpu) {                  /* BRNA 0 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[0] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P5D(CPU *cpu) {                  /* BRNA 1 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[cpu->rs+1] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P5E(CPU *cpu) {                  /* BRNA 2 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[cpu->rs+2] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P5F(CPU *cpu) {                  /* BRNA 3 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[cpu->rs+3] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P60(CPU *cpu) {                  /* IORZ 0 */
  cpu->r[0] = _ior(cpu, cpu->r[0], cpu->r[0]);
  cpu->ts += 6;
  }

void _P61(CPU *cpu) {                  /* IORZ 1 */
  cpu->r[0] = _ior(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[4] : cpu->r[1]);
  cpu->ts += 6;
  }

void _P62(CPU *cpu) {                  /* IORZ 2 */
  cpu->r[0] = _ior(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[5] : cpu->r[2]);
  cpu->ts += 6;
  }

void _P63(CPU *cpu) {                  /* IORZ 3 */
  cpu->r[0] = _ior(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[6] : cpu->r[3]);
  cpu->ts += 6;
  }

void _P64(CPU *cpu) {                  /* IORI 0 */
  cpu->r[0] = _ior(cpu, cpu->r[0], _fetch(cpu));
  cpu->ts += 6;
  }

void _P65(CPU *cpu) {                  /* IORI 1 */
  cpu->r[cpu->rs + 1] = _ior(cpu, cpu->r[cpu->rs + 1], _fetch(cpu));
  cpu->ts += 6;
  }

void _P66(CPU *cpu) {                  /* IORI 2 */
  cpu->r[cpu->rs + 2] = _ior(cpu, cpu->r[cpu->rs + 2], _fetch(cpu));
  cpu->ts += 6;
  }

void _P67(CPU *cpu) {                  /* IORI 3 */
  cpu->r[cpu->rs + 3] = _ior(cpu, cpu->r[cpu->rs + 3], _fetch(cpu));
  cpu->ts += 6;
  }

void _P68(CPU *cpu) {                  /* IORR 0 */
  word a;
  a = _relative(cpu);
  cpu->r[0] = _ior(cpu, cpu->r[0], readMem(a));
  cpu->ts += 9;
  }

void _P69(CPU *cpu) {                  /* IORR 1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+1] = _ior(cpu, cpu->r[cpu->rs+1], readMem(a));
  cpu->ts += 9;
  }

void _P6A(CPU *cpu) {                  /* IORR 2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+2] = _ior(cpu, cpu->r[cpu->rs+2], readMem(a));
  cpu->ts += 9;
  }

void _P6B(CPU *cpu) {                  /* IORR 3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+3] = _ior(cpu, cpu->r[cpu->rs+3], readMem(a));
  cpu->ts += 9;
  }

void _P6C(CPU *cpu) {                  /* IORA 0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _ior(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _ior(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P6D(CPU *cpu) {                  /* IORA 1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _ior(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _ior(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P6E(CPU *cpu) {                  /* IORA 2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _ior(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _ior(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P6F(CPU *cpu) {                  /* IORA 3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _ior(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _ior(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P70(CPU *cpu) {                  /* REDD 0*/
  cpu->r[0] = cpu->port;
  cpu->psl &= 0x3f;
  if (cpu->r[0] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[0] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P71(CPU *cpu) {                  /* REDD 1*/
  cpu->r[cpu->rs+1] = cpu->port;
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+1] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+1] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P72(CPU *cpu) {                  /* REDD 2*/
  cpu->r[cpu->rs+2] = cpu->port;
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+2] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+2] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P73(CPU *cpu) {                  /* REDD 3*/
  cpu->r[cpu->rs+3] = cpu->port;
  cpu->psl &= 0x3f;
  if (cpu->r[cpu->rs+3] & 0x80) cpu->psl |= 0x80;
  else if ((cpu->r[cpu->rs+3] & 0xff) != 0) cpu->psl |= 0x40;
  cpu->ts += 6;
  }

void _P74(CPU *cpu) {                  /* CPSU */
  byte b;
  b = _fetch(cpu) ^ 0xff;
  cpu->psu = (cpu->psu & b) & 0xe7;
  cpu->ts += 9;
  }

void _P75(CPU *cpu) {                  /* CPSL */
  byte b;
  b = _fetch(cpu) ^ 0xff;
  cpu->psl &= b;
  cpu->ts += 9;
  }

void _P76(CPU *cpu) {                  /* PPSU */
  byte b;
  b = _fetch(cpu);
  cpu->psu = (cpu->psu | b) & 0xe7;
  cpu->ts += 9;
  }

void _P77(CPU *cpu) {                  /* PPSL */
  byte b;
  b = _fetch(cpu);
  cpu->psl |= b;
  cpu->ts += 9;
  }

void _P78(CPU *cpu) {                  /* BSNR 0 */
  word a;
  a = _relative(cpu);
  if (cpu->r[0] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _P79(CPU *cpu) {                  /* BSNR 1 */
  word a;
  a = _relative(cpu);
  if (cpu->r[cpu->rs+1] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _P7A(CPU *cpu) {                  /* BSNR 2 */
  word a;
  a = _relative(cpu);
  if (cpu->r[cpu->rs+2] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _P7B(CPU *cpu) {                  /* BSNR 3 */
  word a;
  a = _relative(cpu);
  if (cpu->r[cpu->rs+3] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _P7C(CPU *cpu) {                  /* BSNA 0 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[0] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P7D(CPU *cpu) {                  /* BSNA 1 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[cpu->rs+1] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P7E(CPU *cpu) {                  /* BSNA 2 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[cpu->rs+2] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P7F(CPU *cpu) {                  /* BSNA 3 */
  word a;
  a = _absoluteBR(cpu);
  if (cpu->r[cpu->rs+3] != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P80(CPU *cpu) {                  /* ADDZ 0 */
  cpu->r[0] = _add(cpu, cpu->r[0], cpu->r[0]);
  cpu->ts += 6;
  }

void _P81(CPU *cpu) {                  /* ADDZ 1 */
  cpu->r[0] = _add(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[4] : cpu->r[1]);
  cpu->ts += 6;
  }

void _P82(CPU *cpu) {                  /* ADDZ 2 */
  cpu->r[0] = _add(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[5] : cpu->r[2]);
  cpu->ts += 6;
  }

void _P83(CPU *cpu) {                  /* ADDZ 3 */
  cpu->r[0] = _add(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[6] : cpu->r[3]);
  cpu->ts += 6;
  }

void _P84(CPU *cpu) {                  /* ADDI 0 */
  cpu->r[0] = _add(cpu, cpu->r[0], _fetch(cpu));
  cpu->ts += 6;
  }

void _P85(CPU *cpu) {                  /* ADDI 1 */
  cpu->r[cpu->rs + 1] = _add(cpu, cpu->r[cpu->rs + 1], _fetch(cpu));
  cpu->ts += 6;
  }

void _P86(CPU *cpu) {                  /* ADDI 2 */
  cpu->r[cpu->rs + 2] = _add(cpu, cpu->r[cpu->rs + 2], _fetch(cpu));
  cpu->ts += 6;
  }

void _P87(CPU *cpu) {                  /* ADDI 3 */
  cpu->r[cpu->rs + 3] = _add(cpu, cpu->r[cpu->rs + 3], _fetch(cpu));
  cpu->ts += 6;
  }

void _P88(CPU *cpu) {                  /* ADDR 0 */
  word a;
  a = _relative(cpu);
  cpu->r[0] = _add(cpu, cpu->r[0], readMem(a));
  cpu->ts += 9;
  }

void _P89(CPU *cpu) {                  /* ADDR 1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+1] = _add(cpu, cpu->r[cpu->rs+1], readMem(a));
  cpu->ts += 9;
  }

void _P8A(CPU *cpu) {                  /* ADDR 2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+2] = _add(cpu, cpu->r[cpu->rs+2], readMem(a));
  cpu->ts += 9;
  }

void _P8B(CPU *cpu) {                  /* ADDR 3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+3] = _add(cpu, cpu->r[cpu->rs+3], readMem(a));
  cpu->ts += 9;
  }

void _P8C(CPU *cpu) {                  /* ADDA 0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _add(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _add(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P8D(CPU *cpu) {                  /* ADDA 1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _add(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _add(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P8E(CPU *cpu) {                  /* ADDA 2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _add(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _add(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P8F(CPU *cpu) {                  /* ADDA 3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _add(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _add(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _P90(CPU *cpu) {
  }

void _P91(CPU *cpu) {
  }

void _P92(CPU *cpu) {                  /* LPSU */
  cpu->psu = cpu->r[0] & 0xe7;
  cpu->ts += 6;
  }

void _P93(CPU *cpu) {                  /* LPSL */
  cpu->psl = cpu->r[0];
  cpu->rs = (cpu->psl & FLAG_RS) ? 3 : 0;
  cpu->ts += 6;
  }

void _P94(CPU *cpu) {                  /* DAR,0 */
  cpu->r[0] = _dar(cpu, cpu->r[0]);
  cpu->ts += 9;
  }

void _P95(CPU *cpu) {                  /* DAR,1 */
  cpu->r[cpu->rs+1] = _dar(cpu, cpu->r[cpu->rs+1]);
  cpu->ts += 9;
  }

void _P96(CPU *cpu) {                  /* DAR,2 */
  cpu->r[cpu->rs+2] = _dar(cpu, cpu->r[cpu->rs+2]);
  cpu->ts += 9;
  }

void _P97(CPU *cpu) {                  /* DAR,3 */
  cpu->r[cpu->rs+3] = _dar(cpu, cpu->r[cpu->rs+3]);
  cpu->ts += 9;
  }

void _P98(CPU *cpu) {                  /* BCFR,0 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _P99(CPU *cpu) {                  /* BCFR,1 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) != 0x40) cpu->pc = a;
  cpu->ts += 9;
  }

void _P9A(CPU *cpu) {                  /* BCFR,2 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) != 0x80) cpu->pc = a;
  cpu->ts += 9;
  }

void _P9B(CPU *cpu) {                  /* ZBRR */
  byte b;
  word a;
  word i;
  b = _fetch(cpu);
  if ((b & 0x40) == 0) {
    a = (b & 0x3f) & 0x1fff;
    }
  else {
    a = 8191 - ((b & 0x3f) ^ 0x3f);
    }
  if (b & 0x80) {
    i = a;
    a = readMem(i) << 8;
    i = (i+1) & 0x1fff;
    a |= readMem(i);
    a &= 0x1fff;
    cpu->ts += 6;
    }
  cpu->page = 0;
  cpu->pc = a;
  cpu->ts += 9;
  }

void _P9C(CPU *cpu) {                  /* BCFA,0 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P9D(CPU *cpu) {                  /* BCFA,1 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x40) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P9E(CPU *cpu) {                  /* BCFA,2 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) == 0x80) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _P9F(CPU *cpu) {                  /* BXA */
  word a;
  a = (_absoluteBR(cpu) + cpu->r[cpu->rs+3]) & 0x7fff;
  cpu->pc = a & 0x1fff;
  cpu->page = a & 0xe000;
  cpu->ts += 9;
  }

void _PA0(CPU *cpu) {                  /* SUBZ 0 */
  cpu->r[0] = _sub(cpu, cpu->r[0], cpu->r[0]);
  cpu->ts += 6;
  }

void _PA1(CPU *cpu) {                  /* SUBZ 1 */
  cpu->r[0] = _sub(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[4] : cpu->r[1]);
  cpu->ts += 6;
  }

void _PA2(CPU *cpu) {                  /* SUBZ 2 */
  cpu->r[0] = _sub(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[5] : cpu->r[2]);
  cpu->ts += 6;
  }

void _PA3(CPU *cpu) {                  /* SUBZ 3 */
  cpu->r[0] = _sub(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[6] : cpu->r[3]);
  cpu->ts += 6;
  }

void _PA4(CPU *cpu) {                  /* SUBI 0 */
  cpu->r[0] = _sub(cpu, cpu->r[0], _fetch(cpu));
  cpu->ts += 6;
  }

void _PA5(CPU *cpu) {                  /* SUBI 1 */
  cpu->r[cpu->rs + 1] = _sub(cpu, cpu->r[cpu->rs + 1], _fetch(cpu));
  cpu->ts += 6;
  }

void _PA6(CPU *cpu) {                  /* SUBI 2 */
  cpu->r[cpu->rs + 2] = _sub(cpu, cpu->r[cpu->rs + 2], _fetch(cpu));
  cpu->ts += 6;
  }

void _PA7(CPU *cpu) {                  /* SUBI 3 */
  cpu->r[cpu->rs + 3] = _sub(cpu, cpu->r[cpu->rs + 3], _fetch(cpu));
  cpu->ts += 6;
  }

void _PA8(CPU *cpu) {                  /* SUBR 0 */
  word a;
  a = _relative(cpu);
  cpu->r[0] = _sub(cpu, cpu->r[0], readMem(a));
  cpu->ts += 9;
  }

void _PA9(CPU *cpu) {                  /* SUBR 1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+1] = _sub(cpu, cpu->r[cpu->rs+1], readMem(a));
  cpu->ts += 9;
  }

void _PAA(CPU *cpu) {                  /* SUBR 2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+2] = _sub(cpu, cpu->r[cpu->rs+2], readMem(a));
  cpu->ts += 9;
  }

void _PAB(CPU *cpu) {                  /* SUBR 3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+3] = _sub(cpu, cpu->r[cpu->rs+3], readMem(a));
  cpu->ts += 9;
  }

void _PAC(CPU *cpu) {                   /* SUBA 0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _sub(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _sub(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PAD(CPU *cpu) {                   /* SUBA 1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _sub(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _sub(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PAE(CPU *cpu) {                   /* SUBA 2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _sub(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _sub(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PAF(CPU *cpu) {                   /* SUBA 3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    cpu->r[0] = _sub(cpu, cpu->r[0], readMem(a));
    }
  else {
    cpu->r[cpu->rs + (cpu->op & 0x03)] =
      _sub(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PB0(CPU *cpu) {                  /* WRTC 0 */
  cpu->control = cpu->r[0];
  cpu->ts += 6;
  }

void _PB1(CPU *cpu) {                  /* WRTC 1 */
  cpu->control = cpu->r[cpu->rs+1];
  cpu->ts += 6;
  }

void _PB2(CPU *cpu) {                  /* WRTC 2 */
  cpu->control = cpu->r[cpu->rs+2];
  cpu->ts += 6;
  }

void _PB3(CPU *cpu) {                  /* WRTC 3 */
  cpu->control = cpu->r[cpu->rs+3];
  cpu->ts += 6;
  }

void _PB4(CPU *cpu) {                  /* TPSU */
  byte b;
  b = _fetch(cpu);
  cpu->psu &= 0x7f;
  if (cpu->sense) cpu->psu |= FLAG_S;
  if ((cpu->psu & b) == b) {
    cpu->psl &= 0x3f;
    }
  else {
    cpu->psl &= 0x3f;
    cpu->psl |= 0x80;
    }
  cpu->ts += 9;
  }

void _PB5(CPU *cpu) {                  /* TPSL */
  byte b;
  b = _fetch(cpu);
  if ((cpu->psl & b) == b) {
    cpu->psl &= 0x3f;
    }
  else {
    cpu->psl &= 0x3f;
    cpu->psl |= 0x80;
    }
  cpu->ts += 9;
  }

void _PB6(CPU *cpu) {
  }

void _PB7(CPU *cpu) {
  }

void _PB8(CPU *cpu) {                  /* BSFR 0 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _PB9(CPU *cpu) {                  /* BSFR 1 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) != 0x40) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _PBA(CPU *cpu) {                  /* BSFR 2 */
  word a;
  a = _relative(cpu);
  if ((cpu->psl & 0xc0) != 0x80) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a;
    }
  cpu->ts += 9;
  }

void _PBB(CPU *cpu) {                  /* ZBSR */
  byte b;
  word a;
  word i;
  b = _fetch(cpu);
  if ((b & 0x40) == 0) {
    a = (b & 0x3f) & 0x1fff;
    }
  else {
    a = 8191 - ((b & 0x3f) ^ 0x3f);
    }
  if (b & 0x80) {
    i = a;
    a = readMem(i) << 8;
    i = (i+1) & 0x1fff;
    a |= readMem(i);
    a &= 0x1fff;
    cpu->ts += 6;
    }
  cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
  cpu->stack[cpu->psu & 0x7] = cpu->pc;
  cpu->page = 0;
  cpu->pc = a;
  cpu->ts += 9;
  }

void _PBC(CPU *cpu) {                  /* BSFA 0 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) != 0x00) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PBD(CPU *cpu) {                  /* BSFA 1 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) != 0x40) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PBE(CPU *cpu) {                  /* BSFA 2 */
  word a;
  a = _absoluteBR(cpu);
  if ((cpu->psl & 0xc0) != 0x80) {
    cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
    cpu->stack[cpu->psu & 0x7] = cpu->pc;
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PBF(CPU *cpu) {                  /* BSXA */
  word a;
  a = (_absoluteBR(cpu) + cpu->r[cpu->rs+3]) & 0x7fff;
  cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
  cpu->stack[cpu->psu & 0x7] = cpu->pc;
  cpu->pc = a & 0x1fff;
  cpu->page = a & 0xe000;
  cpu->ts += 9;
  }

void _PC0(CPU *cpu) {                  /* NOP */
  cpu->ts += 6;
  }

void _PC1(CPU *cpu) {                  /* STRZ R1 */
  cpu->r[cpu->rs+1] = cpu->r[0];
  _setCC(cpu, cpu->r[cpu->rs+1]);
  cpu->ts += 6;
  }

void _PC2(CPU *cpu) {                  /* STRZ R2 */
  cpu->r[cpu->rs+2] = cpu->r[0];
  _setCC(cpu, cpu->r[cpu->rs+2]);
  cpu->ts += 6;
  }

void _PC3(CPU *cpu) {                  /* STRZ R3 */
  cpu->r[cpu->rs+3] = cpu->r[0];
  _setCC(cpu, cpu->r[cpu->rs+3]);
  cpu->ts += 6;
  }

void _PC4(CPU *cpu) {
  }

void _PC5(CPU *cpu) {
  }

void _PC6(CPU *cpu) {
  }

void _PC7(CPU *cpu) {
  }

void _PC8(CPU *cpu) {                  /* STRR,R0 */
  word a;
  a = _relative(cpu);
  writeMem(a, cpu->r[cpu->rs + 0]);
  cpu->ts += 9;
  }

void _PC9(CPU *cpu) {                  /* STRR,R1 */
  word a;
  a = _relative(cpu);
  writeMem(a, cpu->r[cpu->rs + 1]);
  cpu->ts += 9;
  }

void _PCA(CPU *cpu) {                  /* STRR,R2 */
  word a;
  a = _relative(cpu);
  writeMem(a, cpu->r[cpu->rs + 2]);
  cpu->ts += 9;
  }

void _PCB(CPU *cpu) {                  /* STRR,R3 */
  word a;
  a = _relative(cpu);
  writeMem(a, cpu->r[cpu->rs + 3]);
  cpu->ts += 9;
  }

void _PCC(CPU *cpu) {                   /* STRA,R0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    writeMem(a,cpu->r[0]);
    }
  else {
    writeMem(a,cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }

void _PCD(CPU *cpu) {                   /* STRA,R1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    writeMem(a,cpu->r[0]);
    }
  else {
    writeMem(a,cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }

void _PCE(CPU *cpu) {                   /* STRA,R2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    writeMem(a,cpu->r[0]);
    }
  else {
    writeMem(a,cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }

void _PCF(CPU *cpu) {                   /* STRA,R3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    writeMem(a,cpu->r[0]);
    }
  else {
    writeMem(a,cpu->r[cpu->rs + (cpu->op & 0x03)]);
    }
  cpu->ts += 12;
  }

void _PD0(CPU *cpu) {                  /* RRL,R0 */
  cpu->r[0] = _rrl(cpu, cpu->r[0]);
  cpu->ts += 6;
  }

void _PD1(CPU *cpu) {                  /* RRL,R1 */
  if (cpu->psl & 0x10)
    cpu->r[4] = _rrl(cpu, cpu->r[4]);
  else
    cpu->r[1] = _rrl(cpu, cpu->r[1]);
  cpu->ts += 6;
  }

void _PD2(CPU *cpu) {                  /* RRL,R2 */
  if (cpu->psl & 0x10)
    cpu->r[5] = _rrl(cpu, cpu->r[5]);
  else
    cpu->r[2] = _rrl(cpu, cpu->r[2]);
  cpu->ts += 6;
  }

void _PD3(CPU *cpu) {                  /* RRL,R3 */
  if (cpu->psl & 0x10)
    cpu->r[6] = _rrl(cpu, cpu->r[6]);
  else
    cpu->r[3] = _rrl(cpu, cpu->r[3]);
  cpu->ts += 6;
  }

void _PD4(CPU *cpu) {                  /* WRTE 0 */
  byte b;
  b = _fetch(cpu);
  cpu->ports[b] = cpu->r[0];
  cpu->ts += 9;
  }

void _PD5(CPU *cpu) {                  /* WRTE 1 */
  byte b;
  b = _fetch(cpu);
  cpu->ports[b] = cpu->r[cpu->rs+1];
  cpu->ts += 9;
  }

void _PD6(CPU *cpu) {                  /* WRTE 2 */
  byte b;
  b = _fetch(cpu);
  cpu->ports[b] = cpu->r[cpu->rs+2];
  cpu->ts += 9;
  }

void _PD7(CPU *cpu) {                  /* WRTE 3 */
  byte b;
  b = _fetch(cpu);
  cpu->ports[b] = cpu->r[cpu->rs+3];
  cpu->ts += 9;
  }

void _PD8(CPU *cpu) {                  /* BIRR,0 */
  word a;
  a = _relative(cpu);
  cpu->r[0]++;
  if (cpu->r[0] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PD9(CPU *cpu) {                  /* BIRR,1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+1]++;
  if (cpu->r[cpu->rs+1] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PDA(CPU *cpu) {                  /* BIRR,2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+2]++;
  if (cpu->r[cpu->rs+2] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PDB(CPU *cpu) {                  /* BIRR,3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+3]++;
  if (cpu->r[cpu->rs+3] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PDC(CPU *cpu) {                  /* BIRA,0 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[0]++;
  if (cpu->r[0] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PDD(CPU *cpu) {                  /* BIRA,1 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[cpu->rs+1]++;
  if (cpu->r[cpu->rs+1] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PDE(CPU *cpu) {                  /* BIRA,2 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[cpu->rs+2]++;
  if (cpu->r[cpu->rs+2] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PDF(CPU *cpu) {                  /* BIRA,3 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[cpu->rs+3]++;
  if (cpu->r[cpu->rs+3] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PE0(CPU *cpu) {                  /* COMZ 0 */
  _com(cpu, cpu->r[0], cpu->r[0]);
  cpu->ts += 6;
  }

void _PE1(CPU *cpu) {                  /* COMZ 1 */
  _com(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[4] : cpu->r[1]);
  cpu->ts += 6;
  }

void _PE2(CPU *cpu) {                  /* COMZ 2 */
  _com(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[5] : cpu->r[2]);
  cpu->ts += 6;
  }

void _PE3(CPU *cpu) {                  /* COMZ 3 */
  _com(cpu, cpu->r[0], (cpu->psl & 0x10) ? cpu->r[6] : cpu->r[3]);
  cpu->ts += 6;
  }

void _PE4(CPU *cpu) {                  /* COMI 0 */
  _com(cpu, cpu->r[0], _fetch(cpu));
  cpu->ts += 6;
  }

void _PE5(CPU *cpu) {                  /* COMI 1 */
  _com(cpu, cpu->r[cpu->rs + 1], _fetch(cpu));
  cpu->ts += 6;
  }

void _PE6(CPU *cpu) {                  /* COMI 2 */
  _com(cpu, cpu->r[cpu->rs + 2], _fetch(cpu));
  cpu->ts += 6;
  }

void _PE7(CPU *cpu) {                  /* COMI 3 */
  _com(cpu, cpu->r[cpu->rs + 3], _fetch(cpu));
  cpu->ts += 6;
  }

void _PE8(CPU *cpu) {                  /* COMR 0 */
  word a;
  a = _relative(cpu);
  _com(cpu, cpu->r[0], readMem(a));
  cpu->ts += 9;
  }

void _PE9(CPU *cpu) {                  /* COMR 1 */
  word a;
  a = _relative(cpu);
  _com(cpu, cpu->r[cpu->rs+1], readMem(a));
  cpu->ts += 9;
  }

void _PEA(CPU *cpu) {                  /* COMR 2 */
  word a;
  a = _relative(cpu);
  _com(cpu, cpu->r[cpu->rs+2], readMem(a));
  cpu->ts += 9;
 }

void _PEB(CPU *cpu) {                  /* COMR 3 */
  word a;
  a = _relative(cpu);
  _com(cpu, cpu->r[cpu->rs+3], readMem(a));
  cpu->ts += 9;
  }

void _PEC(CPU *cpu) {                  /* COMA 0 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    _com(cpu, cpu->r[0], readMem(a));
    }
  else {
    _com(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PED(CPU *cpu) {                  /* COMA 1 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    _com(cpu, cpu->r[0], readMem(a));
    }
  else {
    _com(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PEE(CPU *cpu) {                  /* COMA 2 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    _com(cpu, cpu->r[0], readMem(a));
    }
  else {
    _com(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PEF(CPU *cpu) {                  /* COMA 3 */
  word a;
  byte b;
  b = ram[cpu->page + cpu->pc];
  a = _absolute(cpu);
  if ((b & 0x60) != 0) {
    _com(cpu, cpu->r[0], readMem(a));
    }
  else {
    _com(cpu, cpu->r[cpu->rs + (cpu->op & 0x03)], readMem(a));
    }
  cpu->ts += 12;
  }

void _PF0(CPU *cpu) {                  /* WRTD 0 */
  cpu->port = cpu->r[0];
  cpu->ts += 6;
  }

void _PF1(CPU *cpu) {                  /* WRTD 1 */
  cpu->port = cpu->r[cpu->rs+1];
  cpu->ts += 6;
  }

void _PF2(CPU *cpu) {                  /* WRTD 2 */
  cpu->port = cpu->r[cpu->rs+2];
  cpu->ts += 6;
  }

void _PF3(CPU *cpu) {                  /* WRTD 3 */
  cpu->port = cpu->r[cpu->rs+3];
  cpu->ts += 6;
  }

void _PF4(CPU *cpu) {                  /* TMI,0 */
  _tmi(cpu, cpu->r[0], _fetch(cpu));
  cpu->ts += 9;
  }

void _PF5(CPU *cpu) {                  /* TMI,1 */
  _tmi(cpu, cpu->r[cpu->rs+1], _fetch(cpu));
  cpu->ts += 9;
  }

void _PF6(CPU *cpu) {                  /* TMI,2 */
  _tmi(cpu, cpu->r[cpu->rs+2], _fetch(cpu));
  cpu->ts += 9;
  }

void _PF7(CPU *cpu) {                  /* TMI,3 */
  _tmi(cpu, cpu->r[cpu->rs+3], _fetch(cpu));
  cpu->ts += 9;
  }

void _PF8(CPU *cpu) {                  /* BDRR,0 */
  word a;
  a = _relative(cpu);
  cpu->r[0]--;
  if (cpu->r[0] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PF9(CPU *cpu) {                  /* BDRR,1 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+1]--;
  if (cpu->r[cpu->rs+1] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PFA(CPU *cpu) {                  /* BDRR,2 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+2]--;
  if (cpu->r[cpu->rs+2] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PFB(CPU *cpu) {                  /* BDRR,3 */
  word a;
  a = _relative(cpu);
  cpu->r[cpu->rs+3]--;
  if (cpu->r[cpu->rs+3] != 0x00) cpu->pc = a;
  cpu->ts += 9;
  }

void _PFC(CPU *cpu) {                  /* BDRA,0 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[0]--;
  if (cpu->r[0] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PFD(CPU *cpu) {                  /* BDRA,1 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[cpu->rs+1]--;
  if (cpu->r[cpu->rs+1] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PFE(CPU *cpu) {                  /* BDRA,2 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[cpu->rs+2]--;
  if (cpu->r[cpu->rs+2] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void _PFF(CPU *cpu) {                  /* BDRA,3 */
  word a;
  a = _absoluteBR(cpu);
  cpu->r[cpu->rs+3]--;
  if (cpu->r[cpu->rs+3] != 0x00) {
    cpu->pc = a & 0x1fff;
    cpu->page = a & 0xe000;
    }
  cpu->ts += 9;
  }

void cpu_prepare(CPU *cpu) {
  cpu->Inst[0x00]=_P0; cpu->Inst[0x01]=_P1; cpu->Inst[0x02]=_P2; cpu->Inst[0x03]=_P3;
  cpu->Inst[0x04]=_P4; cpu->Inst[0x05]=_P5; cpu->Inst[0x06]=_P6; cpu->Inst[0x07]=_P7;
  cpu->Inst[0x08]=_P8; cpu->Inst[0x09]=_P9; cpu->Inst[0x0a]=_PA; cpu->Inst[0x0b]=_PB;
  cpu->Inst[0x0c]=_PC; cpu->Inst[0x0d]=_PD; cpu->Inst[0x0e]=_PE; cpu->Inst[0x0f]=_PF;

  cpu->Inst[0x10]=_P10; cpu->Inst[0x11]=_P11; cpu->Inst[0x12]=_P12; cpu->Inst[0x13]=_P13;
  cpu->Inst[0x14]=_P14; cpu->Inst[0x15]=_P15; cpu->Inst[0x16]=_P16; cpu->Inst[0x17]=_P17;
  cpu->Inst[0x18]=_P18; cpu->Inst[0x19]=_P19; cpu->Inst[0x1a]=_P1A; cpu->Inst[0x1b]=_P1B;
  cpu->Inst[0x1c]=_P1C; cpu->Inst[0x1d]=_P1D; cpu->Inst[0x1e]=_P1E; cpu->Inst[0x1f]=_P1F;

  cpu->Inst[0x20]=_P20; cpu->Inst[0x21]=_P21; cpu->Inst[0x22]=_P22; cpu->Inst[0x23]=_P23;
  cpu->Inst[0x24]=_P24; cpu->Inst[0x25]=_P25; cpu->Inst[0x26]=_P26; cpu->Inst[0x27]=_P27;
  cpu->Inst[0x28]=_P28; cpu->Inst[0x29]=_P29; cpu->Inst[0x2a]=_P2A; cpu->Inst[0x2b]=_P2B;
  cpu->Inst[0x2c]=_P2C; cpu->Inst[0x2d]=_P2D; cpu->Inst[0x2e]=_P2E; cpu->Inst[0x2f]=_P2F;

  cpu->Inst[0x30]=_P30; cpu->Inst[0x31]=_P31; cpu->Inst[0x32]=_P32; cpu->Inst[0x33]=_P33;
  cpu->Inst[0x34]=_P34; cpu->Inst[0x35]=_P35; cpu->Inst[0x36]=_P36; cpu->Inst[0x37]=_P37;
  cpu->Inst[0x38]=_P38; cpu->Inst[0x39]=_P39; cpu->Inst[0x3a]=_P3A; cpu->Inst[0x3b]=_P3B;
  cpu->Inst[0x3c]=_P3C; cpu->Inst[0x3d]=_P3D; cpu->Inst[0x3e]=_P3E; cpu->Inst[0x3f]=_P3F;

  cpu->Inst[0x40]=_P40; cpu->Inst[0x41]=_P41; cpu->Inst[0x42]=_P42; cpu->Inst[0x43]=_P43;
  cpu->Inst[0x44]=_P44; cpu->Inst[0x45]=_P45; cpu->Inst[0x46]=_P46; cpu->Inst[0x47]=_P47;
  cpu->Inst[0x48]=_P48; cpu->Inst[0x49]=_P49; cpu->Inst[0x4a]=_P4A; cpu->Inst[0x4b]=_P4B;
  cpu->Inst[0x4c]=_P4C; cpu->Inst[0x4d]=_P4D; cpu->Inst[0x4e]=_P4E; cpu->Inst[0x4f]=_P4F;

  cpu->Inst[0x50]=_P50; cpu->Inst[0x51]=_P51; cpu->Inst[0x52]=_P52; cpu->Inst[0x53]=_P53;
  cpu->Inst[0x54]=_P54; cpu->Inst[0x55]=_P55; cpu->Inst[0x56]=_P56; cpu->Inst[0x57]=_P57;
  cpu->Inst[0x58]=_P58; cpu->Inst[0x59]=_P59; cpu->Inst[0x5a]=_P5A; cpu->Inst[0x5b]=_P5B;
  cpu->Inst[0x5c]=_P5C; cpu->Inst[0x5d]=_P5D; cpu->Inst[0x5e]=_P5E; cpu->Inst[0x5f]=_P5F;

  cpu->Inst[0x60]=_P60; cpu->Inst[0x61]=_P61; cpu->Inst[0x62]=_P62; cpu->Inst[0x63]=_P63;
  cpu->Inst[0x64]=_P64; cpu->Inst[0x65]=_P65; cpu->Inst[0x66]=_P66; cpu->Inst[0x67]=_P67;
  cpu->Inst[0x68]=_P68; cpu->Inst[0x69]=_P69; cpu->Inst[0x6a]=_P6A; cpu->Inst[0x6b]=_P6B;
  cpu->Inst[0x6c]=_P6C; cpu->Inst[0x6d]=_P6D; cpu->Inst[0x6e]=_P6E; cpu->Inst[0x6f]=_P6F;

  cpu->Inst[0x70]=_P70; cpu->Inst[0x71]=_P71; cpu->Inst[0x72]=_P72; cpu->Inst[0x73]=_P73;
  cpu->Inst[0x74]=_P74; cpu->Inst[0x75]=_P75; cpu->Inst[0x76]=_P76; cpu->Inst[0x77]=_P77;
  cpu->Inst[0x78]=_P78; cpu->Inst[0x79]=_P79; cpu->Inst[0x7a]=_P7A; cpu->Inst[0x7b]=_P7B;
  cpu->Inst[0x7c]=_P7C; cpu->Inst[0x7d]=_P7D; cpu->Inst[0x7e]=_P7E; cpu->Inst[0x7f]=_P7F;

  cpu->Inst[0x80]=_P80; cpu->Inst[0x81]=_P81; cpu->Inst[0x82]=_P82; cpu->Inst[0x83]=_P83;
  cpu->Inst[0x84]=_P84; cpu->Inst[0x85]=_P85; cpu->Inst[0x86]=_P86; cpu->Inst[0x87]=_P87;
  cpu->Inst[0x88]=_P88; cpu->Inst[0x89]=_P89; cpu->Inst[0x8a]=_P8A; cpu->Inst[0x8b]=_P8B;
  cpu->Inst[0x8c]=_P8C; cpu->Inst[0x8d]=_P8D; cpu->Inst[0x8e]=_P8E; cpu->Inst[0x8f]=_P8F;

  cpu->Inst[0x90]=_P90; cpu->Inst[0x91]=_P91; cpu->Inst[0x92]=_P92; cpu->Inst[0x93]=_P93;
  cpu->Inst[0x94]=_P94; cpu->Inst[0x95]=_P95; cpu->Inst[0x96]=_P96; cpu->Inst[0x97]=_P97;
  cpu->Inst[0x98]=_P98; cpu->Inst[0x99]=_P99; cpu->Inst[0x9a]=_P9A; cpu->Inst[0x9b]=_P9B;
  cpu->Inst[0x9c]=_P9C; cpu->Inst[0x9d]=_P9D; cpu->Inst[0x9e]=_P9E; cpu->Inst[0x9f]=_P9F;

  cpu->Inst[0xa0]=_PA0; cpu->Inst[0xa1]=_PA1; cpu->Inst[0xa2]=_PA2; cpu->Inst[0xa3]=_PA3;
  cpu->Inst[0xa4]=_PA4; cpu->Inst[0xa5]=_PA5; cpu->Inst[0xa6]=_PA6; cpu->Inst[0xa7]=_PA7;
  cpu->Inst[0xa8]=_PA8; cpu->Inst[0xa9]=_PA9; cpu->Inst[0xaa]=_PAA; cpu->Inst[0xab]=_PAB;
  cpu->Inst[0xac]=_PAC; cpu->Inst[0xad]=_PAD; cpu->Inst[0xae]=_PAE; cpu->Inst[0xaf]=_PAF;

  cpu->Inst[0xb0]=_PB0; cpu->Inst[0xb1]=_PB1; cpu->Inst[0xb2]=_PB2; cpu->Inst[0xb3]=_PB3;
  cpu->Inst[0xb4]=_PB4; cpu->Inst[0xb5]=_PB5; cpu->Inst[0xb6]=_PB6; cpu->Inst[0xb7]=_PB7;
  cpu->Inst[0xb8]=_PB8; cpu->Inst[0xb9]=_PB9; cpu->Inst[0xba]=_PBA; cpu->Inst[0xbb]=_PBB;
  cpu->Inst[0xbc]=_PBC; cpu->Inst[0xbd]=_PBD; cpu->Inst[0xbe]=_PBE; cpu->Inst[0xbf]=_PBF;

  cpu->Inst[0xc0]=_PC0; cpu->Inst[0xc1]=_PC1; cpu->Inst[0xc2]=_PC2; cpu->Inst[0xc3]=_PC3;
  cpu->Inst[0xc4]=_PC4; cpu->Inst[0xc5]=_PC5; cpu->Inst[0xc6]=_PC6; cpu->Inst[0xc7]=_PC7;
  cpu->Inst[0xc8]=_PC8; cpu->Inst[0xc9]=_PC9; cpu->Inst[0xca]=_PCA; cpu->Inst[0xcb]=_PCB;
  cpu->Inst[0xcc]=_PCC; cpu->Inst[0xcd]=_PCD; cpu->Inst[0xce]=_PCE; cpu->Inst[0xcf]=_PCF;

  cpu->Inst[0xd0]=_PD0; cpu->Inst[0xd1]=_PD1; cpu->Inst[0xd2]=_PD2; cpu->Inst[0xd3]=_PD3;
  cpu->Inst[0xd4]=_PD4; cpu->Inst[0xd5]=_PD5; cpu->Inst[0xd6]=_PD6; cpu->Inst[0xd7]=_PD7;
  cpu->Inst[0xd8]=_PD8; cpu->Inst[0xd9]=_PD9; cpu->Inst[0xda]=_PDA; cpu->Inst[0xdb]=_PDB;
  cpu->Inst[0xdc]=_PDC; cpu->Inst[0xdd]=_PDD; cpu->Inst[0xde]=_PDE; cpu->Inst[0xdf]=_PDF;

  cpu->Inst[0xe0]=_PE0; cpu->Inst[0xe1]=_PE1; cpu->Inst[0xe2]=_PE2; cpu->Inst[0xe3]=_PE3;
  cpu->Inst[0xe4]=_PE4; cpu->Inst[0xe5]=_PE5; cpu->Inst[0xe6]=_PE6; cpu->Inst[0xe7]=_PE7;
  cpu->Inst[0xe8]=_PE8; cpu->Inst[0xe9]=_PE9; cpu->Inst[0xea]=_PEA; cpu->Inst[0xeb]=_PEB;
  cpu->Inst[0xec]=_PEC; cpu->Inst[0xed]=_PED; cpu->Inst[0xee]=_PEE; cpu->Inst[0xef]=_PEF;

  cpu->Inst[0xf0]=_PF0; cpu->Inst[0xf1]=_PF1; cpu->Inst[0xf2]=_PF2; cpu->Inst[0xf3]=_PF3;
  cpu->Inst[0xf4]=_PF4; cpu->Inst[0xf5]=_PF5; cpu->Inst[0xf6]=_PF6; cpu->Inst[0xf7]=_PF7;
  cpu->Inst[0xf8]=_PF8; cpu->Inst[0xf9]=_PF9; cpu->Inst[0xfa]=_PFA; cpu->Inst[0xfb]=_PFB;
  cpu->Inst[0xfc]=_PFC; cpu->Inst[0xfd]=_PFD; cpu->Inst[0xfe]=_PFE; cpu->Inst[0xff]=_PFF;
  }

void cpu_irq(CPU *cpu,byte b) {
  word a;
  word i;
  if ((cpu->psu & FLAG_II) != 0) return;
  cpu->psu |= FLAG_II;
  if ((b & 0x40) == 0) {
    a = (b & 0x3f) & 0x1fff;
    }
  else {
    a = 8191 - ((b & 0x3f) ^ 0x3f);
    }
  if (b & 0x80) {
    i = a;
    a = readMem(i) << 8;
    i = (i+1) & 0x1fff;
    a |= readMem(i);
    a &= 0x1fff;
    cpu->ts += 6;
    }
  cpu->psu = (cpu->psu & 0xf8) | (((cpu->psu & 0x7) + 1) & 0x7);
  cpu->stack[cpu->psu & 0x7] = cpu->pc;
  cpu->page = 0;
  cpu->pc = a;
  cpu->ts += 9;
  }

void cpu_reset(CPU* cpu) {
  cpu->pc = 0;
  cpu->page = 0;
  cpu->rs = 0;
  cpu->psl = 0;
  }

void cpu_cycle(CPU* cpu) {
  cpu->ts = 0;
  cpu->op = _fetch(cpu);
  cpu->Inst[cpu->op](cpu);
  }

