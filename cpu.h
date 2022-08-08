#ifndef _CPU_H
#define _CPU_H

#ifndef _BYTE
typedef unsigned char byte;
typedef unsigned short word;
#define _BYTE
#endif

#define FLAG_CC1    0x80
#define FLAG_CC0    0x40
#define FLAG_IDC    0x20
#define FLAG_RS     0x10
#define FLAG_WC     0x08
#define FLAG_OVF    0x04
#define FLAG_COM    0x02
#define FLAG_C      0x01

#define FLAG_S      0x80
#define FLAG_F      0x40
#define FLAG_II     0x20

typedef struct _CPU {
  word stack[8];
  byte r[7];
  byte psu;
  byte psl;
  byte rs;
  byte halt;
  byte ts;
  byte op;
  byte sense;
  word pc;
  word page;
  byte port;
  byte control;
  byte ports[256];
  void (*Inst[256])(struct _CPU *cpu);
  } CPU;

#endif

