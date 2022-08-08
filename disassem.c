#include "header.h"

word Disassem(word addr, char* dest) {
  char tmp[80];
  char tmp2[16];
  sprintf(dest,"%04x: %02x ", addr, ram[addr]);
//         sprintf(tmp,"%02x           NEG      $%02x",ram[addr+1],ram[addr+1]);
//         addr += 2;
  sprintf(tmp," ");
  switch (ram[addr]) {
    case 0x00:
         break;
    case 0x01:
         sprintf(tmp,"             LODZ     R1");
         addr += 1;
         break;
    case 0x02:
         sprintf(tmp,"             LODZ     R2");
         addr += 1;
         break;
    case 0x03:
         sprintf(tmp,"             LODZ     R3");
         addr += 1;
         break;
    case 0x04:
         sprintf(tmp,"%02x           LODI,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x05:
         sprintf(tmp,"%02x           LODI,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x06:
         sprintf(tmp,"%02x           LODI,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x07:
         sprintf(tmp,"%02x           LODI,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x08:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x09:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x0a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x0b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           LODR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           LODR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x0c:
         sprintf(tmp,"%02x %02x        LODA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x0d:
         sprintf(tmp,"%02x %02x        LODA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x0e:
         sprintf(tmp,"%02x %02x        LODA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x0f:
         sprintf(tmp,"%02x %02x        LODA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0x12:
         sprintf(tmp,"             SPSU");
         addr += 1;
         break;
    case 0x13:
         sprintf(tmp,"             SPSL");
         addr += 1;
         break;
    case 0x14:
         sprintf(tmp,"             RETC,0");
         addr += 1;
         break;
    case 0x15:
         sprintf(tmp,"             RETC,1");
         addr += 1;
         break;
    case 0x16:
         sprintf(tmp,"             RETC,2");
         addr += 1;
         break;
    case 0x17:
         sprintf(tmp,"             RETC,3");
         addr += 1;
         break;
    case 0x18:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,0   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,0   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,0   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,0   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x19:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,1   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,1   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,1   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,1   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x1a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,2   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,2   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,2   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,2   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x1b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,3   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,3   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCTR,3   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCTR,3   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x1c:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BCTA,0   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BCTA,0   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x1d:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BCTA,1   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BCTA,1   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x1e:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BCTA,2   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BCTA,2   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x1f:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BCTA,3   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BCTA,3   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;

    case 0x20:
         sprintf(tmp,"             EORZ     R1");
         addr += 1;
         break;
    case 0x21:
         sprintf(tmp,"             EORZ     R1");
         addr += 1;
         break;
    case 0x22:
         sprintf(tmp,"             EORZ     R2");
         addr += 1;
         break;
    case 0x23:
         sprintf(tmp,"             EORZ     R3");
         addr += 1;
         break;
    case 0x24:
         sprintf(tmp,"%02x           EORI,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x25:
         sprintf(tmp,"%02x           EORI,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x26:
         sprintf(tmp,"%02x           EORI,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x27:
         sprintf(tmp,"%02x           EORI,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x28:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x29:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x2a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x2b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           EORR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           EORR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x2c:
         sprintf(tmp,"%02x %02x        EORA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x2d:
         sprintf(tmp,"%02x %02x        EORA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x2e:
         sprintf(tmp,"%02x %02x        EORA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x2f:
         sprintf(tmp,"%02x %02x        EORA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0x30:
         sprintf(tmp,"             REDC,R0");
         addr += 1;
         break;
    case 0x31:
         sprintf(tmp,"             REDC,R1");
         addr += 1;
         break;
    case 0x32:
         sprintf(tmp,"             REDC,R2");
         addr += 1;
         break;
    case 0x33:
         sprintf(tmp,"             REDC,R3");
         addr += 1;
         break;
    case 0x34:
         sprintf(tmp,"             RETE,0");
         addr += 1;
         break;
    case 0x35:
         sprintf(tmp,"             RETE,1");
         addr += 1;
         break;
    case 0x36:
         sprintf(tmp,"             RETE,2");
         addr += 1;
         break;
    case 0x37:
         sprintf(tmp,"             RETE,3");
         addr += 1;
         break;
    case 0x38:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,0   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,0   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,0   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,0   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x39:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,1   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,1   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,1   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,1   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x3a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,2   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,2   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,2   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,2   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x3b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,3   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,3   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSTR,3   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSTR,3   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x3c:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSTA,0   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSTA,0   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x3d:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSTA,1   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSTA,1   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x3e:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSTA,2   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSTA,2   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x3f:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSTA,3   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSTA,3   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;

    case 0x40:
         sprintf(tmp,"             HALT");
         addr += 1;
         break;
    case 0x41:
         sprintf(tmp,"             ANDZ     R1");
         addr += 1;
         break;
    case 0x42:
         sprintf(tmp,"             ANDZ     R2");
         addr += 1;
         break;
    case 0x43:
         sprintf(tmp,"             ANDZ     R3");
         addr += 1;
         break;
    case 0x44:
         sprintf(tmp,"%02x           ANDI,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x45:
         sprintf(tmp,"%02x           ANDI,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x46:
         sprintf(tmp,"%02x           ANDI,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x47:
         sprintf(tmp,"%02x           ANDI,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x48:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x49:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x4a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x4b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ANDR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ANDR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x4c:
         sprintf(tmp,"%02x %02x        ANDA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x4d:
         sprintf(tmp,"%02x %02x        ANDA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x4e:
         sprintf(tmp,"%02x %02x        ANDA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x4f:
         sprintf(tmp,"%02x %02x        ANDA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0x50:
         sprintf(tmp,"             RRR,R0");
         addr += 1;
         break;
    case 0x51:
         sprintf(tmp,"             RRR,R1");
         addr += 1;
         break;
    case 0x52:
         sprintf(tmp,"             RRR,R2");
         addr += 1;
         break;
    case 0x53:
         sprintf(tmp,"             RRR,R3");
         addr += 1;
         break;
    case 0x54:
         sprintf(tmp,"%02x           REDE,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x55:
         sprintf(tmp,"%02x           REDE,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x56:
         sprintf(tmp,"%02x           REDE,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x57:
         sprintf(tmp,"%02x           REDE,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x58:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x59:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x5a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x5b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BRNR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BRNR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x5c:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BRNA,R0  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BRNA,R0  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x5d:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BRNA,R1  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BRNA,R1  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x5e:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BRNA,R2  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BRNA,R2  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x5f:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BRNA,R3  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BRNA,R3  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;

    case 0x60:
         sprintf(tmp,"             IORZ     R0");
         addr += 1;
         break;
    case 0x61:
         sprintf(tmp,"             IORZ     R1");
         addr += 1;
         break;
    case 0x62:
         sprintf(tmp,"             IORZ     R2");
         addr += 1;
         break;
    case 0x63:
         sprintf(tmp,"             IORZ     R3");
         addr += 1;
         break;
    case 0x64:
         sprintf(tmp,"%02x           IORI,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x65:
         sprintf(tmp,"%02x           IORI,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x66:
         sprintf(tmp,"%02x           IORI,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x67:
         sprintf(tmp,"%02x           IORI,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x68:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x69:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x6a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x6b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           IORR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           IORR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x6c:
         sprintf(tmp,"%02x %02x        IORA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x6d:
         sprintf(tmp,"%02x %02x        IORA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x6e:
         sprintf(tmp,"%02x %02x        IORA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x6f:
         sprintf(tmp,"%02x %02x        IORA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0x70:
         sprintf(tmp,"             REDD,R0");
         addr += 1;
         break;
    case 0x71:
         sprintf(tmp,"             REDD,R1");
         addr += 1;
         break;
    case 0x72:
         sprintf(tmp,"             REDD,R2");
         addr += 1;
         break;
    case 0x73:
         sprintf(tmp,"             REDD,R3");
         addr += 1;
         break;
    case 0x74:
         sprintf(tmp,"%02x           CPSU     $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x75:
         sprintf(tmp,"%02x           CPSL     $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x76:
         sprintf(tmp,"%02x           PPSU     $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x77:
         sprintf(tmp,"%02x           PPSL     $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x78:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x79:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x7a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x7b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSNR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSNR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x7c:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSNA,R0  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSNA,R0  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x7d:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSNA,R1  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSNA,R1  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x7e:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSNA,R2  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSNA,R2  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x7f:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSNA,R3  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSNA,R3  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;

    case 0x80:
         sprintf(tmp,"             ADDZ     R0");
         addr += 1;
         break;
    case 0x81:
         sprintf(tmp,"             ADDZ     R1");
         addr += 1;
         break;
    case 0x82:
         sprintf(tmp,"             ADDZ     R2");
         addr += 1;
         break;
    case 0x83:
         sprintf(tmp,"             ADDZ     R3");
         addr += 1;
         break;
    case 0x84:
         sprintf(tmp,"%02x           ADDI,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x85:
         sprintf(tmp,"%02x           ADDI,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x86:
         sprintf(tmp,"%02x           ADDI,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x87:
         sprintf(tmp,"%02x           ADDI,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0x88:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x89:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x8a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x8b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ADDR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ADDR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x8c:
         sprintf(tmp,"%02x %02x        ADDA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x8d:
         sprintf(tmp,"%02x %02x        ADDA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x8e:
         sprintf(tmp,"%02x %02x        ADDA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0x8f:
         sprintf(tmp,"%02x %02x        ADDA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0x92:
         sprintf(tmp,"             LPSU");
         addr += 1;
         break;
    case 0x93:
         sprintf(tmp,"             LPSL");
         addr += 1;
         break;
    case 0x94:
         sprintf(tmp,"             DAR,R0");
         addr += 1;
         break;
    case 0x95:
         sprintf(tmp,"             DAR,R1");
         addr += 1;
         break;
    case 0x96:
         sprintf(tmp,"             DAR,R2");
         addr += 1;
         break;
    case 0x97:
         sprintf(tmp,"             DAR,R3");
         addr += 1;
         break;
    case 0x98:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCFR,0   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCFR,0   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCFR,0   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCFR,0   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x99:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCFR,1   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCFR,1   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCFR,1   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCFR,1   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x9a:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCFR,2   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCFR,2   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BCFR,2   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BCFR,2   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x9b:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ZBRR     [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ZBRR     [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ZBRR     -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ZBRR     +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0x9c:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BCFA,0   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BCFA,0   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x9d:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BCFA,1   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BCFA,1   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x9e:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BCFA,2   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BCFA,2   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0x9f:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BXA      [$%02x%02x],X",ram[addr+1],ram[addr+2],
                  ram[addr+1] & 0x7f, ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BXA       %02x%02x,X",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;

    case 0xa0:
         sprintf(tmp,"             SUBZ     R0");
         addr += 1;
         break;
    case 0xa1:
         sprintf(tmp,"             SUBZ     R1");
         addr += 1;
         break;
    case 0xa2:
         sprintf(tmp,"             SUBZ     R2");
         addr += 1;
         break;
    case 0xa3:
         sprintf(tmp,"             SUBZ     R3");
         addr += 1;
         break;
    case 0xa4:
         sprintf(tmp,"%02x           SUBI,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xa5:
         sprintf(tmp,"%02x           SUBI,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xa6:
         sprintf(tmp,"%02x           SUBI,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xa7:
         sprintf(tmp,"%02x           SUBI,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xa8:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xa9:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xaa:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xab:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           SUBR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           SUBR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xac:
         sprintf(tmp,"%02x %02x        SUBA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xad:
         sprintf(tmp,"%02x %02x        SUBA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xae:
         sprintf(tmp,"%02x %02x        SUBA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xaf:
         sprintf(tmp,"%02x %02x        SUBA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0xb0:
         sprintf(tmp,"             WRTC,R0");
         addr += 1;
         break;
    case 0xb1:
         sprintf(tmp,"             WRTC,R1");
         addr += 1;
         break;
    case 0xb2:
         sprintf(tmp,"             WRTC,R2");
         addr += 1;
         break;
    case 0xb3:
         sprintf(tmp,"             WRTC,R3");
         addr += 1;
         break;
    case 0xb4:
         sprintf(tmp,"%02x           TPSU     $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xb5:
         sprintf(tmp,"%02x           TPSL     $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xb8:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSFR,0   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSFR,0   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSFR,0   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSFR,0   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xb9:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSFR,1   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSFR,1   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSFR,1   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSFR,1   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xba:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSFR,2   [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSFR,2   [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BSFR,2   -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BSFR,2   +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xbb:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ZBSR     [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ZBSR     [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           ZBSR     -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           ZBSR     +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xbc:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSFA,0   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSFA,0   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xbd:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSFA,1   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSFA,1   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xbe:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSFA,2   [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSFA,2   $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xbf:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BSXA     [$%02x%02x],X",ram[addr+1],ram[addr+2],
                  ram[addr+1] & 0x7f, ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BSXA      %02x%02x,X",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;

    case 0xc0:
         sprintf(tmp,"             NOP");
         addr += 1;
         break;
    case 0xc1:
         sprintf(tmp,"             STRZ     R1");
         addr += 1;
         break;
    case 0xc2:
         sprintf(tmp,"             STRZ     R2");
         addr += 1;
         break;
    case 0xc3:
         sprintf(tmp,"             STRZ     R3");
         addr += 1;
         break;
    case 0xc8:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xc9:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xca:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xcb:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           STRR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           STRR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xcc:
         sprintf(tmp,"%02x %02x        STRA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xcd:
         sprintf(tmp,"%02x %02x        STRA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xce:
         sprintf(tmp,"%02x %02x        STRA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xcf:
         sprintf(tmp,"%02x %02x        STRA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0xd0:
         sprintf(tmp,"             RRL,R0");
         addr += 1;
         break;
    case 0xd1:
         sprintf(tmp,"             RRL,R1");
         addr += 1;
         break;
    case 0xd2:
         sprintf(tmp,"             RRL,R2");
         addr += 1;
         break;
    case 0xd3:
         sprintf(tmp,"             RRL,R3");
         addr += 1;
         break;
    case 0xd4:
         sprintf(tmp,"%02x           WRTE,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xd5:
         sprintf(tmp,"%02x           WRTE,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xd6:
         sprintf(tmp,"%02x           WRTE,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xd7:
         sprintf(tmp,"%02x           WRTE,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xd8:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xd9:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xda:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xdb:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BIRR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BIRR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xdc:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BIRA,R0  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BIRA,R0  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xdd:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BIRA,R1  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BIRA,R1  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xde:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BIRA,R2  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BIRA,R2  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xdf:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BIRA,R2  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BIRA,R2  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;

    case 0xe0:
         sprintf(tmp,"             COMZ     R0");
         addr += 1;
         break;
    case 0xe1:
         sprintf(tmp,"             COMZ     R1");
         addr += 1;
         break;
    case 0xe2:
         sprintf(tmp,"             COMZ     R2");
         addr += 1;
         break;
    case 0xe3:
         sprintf(tmp,"             COMZ     R3");
         addr += 1;
         break;
    case 0xe4:
         sprintf(tmp,"%02x           COMI,R0  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xe5:
         sprintf(tmp,"%02x           COMI,R1  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xe6:
         sprintf(tmp,"%02x           COMI,R2  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xe7:
         sprintf(tmp,"%02x           COMI,R3  $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xe8:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xe9:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xea:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xeb:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           COMR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           COMR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xec:
         sprintf(tmp,"%02x %02x        COMA,R0  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xed:
         sprintf(tmp,"%02x %02x        COMA,R1  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xee:
         sprintf(tmp,"%02x %02x        COMA,R2  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;
    case 0xef:
         sprintf(tmp,"%02x %02x        COMA,R3  ", ram[addr+1], ram[addr+2]);
         if (ram[addr+1] & 0x80) strcat(tmp,"[");
         sprintf(tmp2,"$%02x%02x", ram[addr+1] & 0x1f, ram[addr+2]);
         strcat(tmp, tmp2);
         if (ram[addr+1] & 0x80) strcat(tmp,"]");
         if ((ram[addr+1] & 0x60) != 0) {
           strcat(tmp,",");
           if ((ram[addr+1] & 0x60) == 0x40) strcat(tmp,"-");
           if ((ram[addr+1] & 0x60) == 0x20) strcat(tmp,"+");
           sprintf(tmp2,"R%d",(ram[addr] & 0x03));
           strcat(tmp,tmp2);
           }
         addr += 3;
         break;

    case 0xf0:
         sprintf(tmp,"             WRTD,R0");
         addr += 1;
         break;
    case 0xf1:
         sprintf(tmp,"             WRTD,R1");
         addr += 1;
         break;
    case 0xf2:
         sprintf(tmp,"             WRTD,R2");
         addr += 1;
         break;
    case 0xf3:
         sprintf(tmp,"             WRTD,R3");
         addr += 1;
         break;
    case 0xf4:
         sprintf(tmp,"%02x           TMI,R0   $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xf5:
         sprintf(tmp,"%02x           TMI,R1   $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xf6:
         sprintf(tmp,"%02x           TMI,R2   $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xf7:
         sprintf(tmp,"%02x           TMI,R3   $%02x",ram[addr+1],ram[addr+1]);
         addr += 2;
         break;
    case 0xf8:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R0  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R0  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R0  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R0  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xf9:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R1  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R1  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R1  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R1  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xfa:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R2  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R2  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R2  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R2  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xfb:
         if (ram[addr+1] & 0x80) {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R3  [-$%02x]",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R3  [+$%02x]",ram[addr+1],ram[addr+1] & 0x3f);
           }
         else {
           if (ram[addr+1] & 0x40)
             sprintf(tmp,"%02x           BDRR,R3  -$%02x",ram[addr+1],((ram[addr+1] & 0x3f) ^ 0x3f)+1);
           else
             sprintf(tmp,"%02x           BDRR,R3  +$%02x",ram[addr+1],ram[addr+1] & 0x3f);
           }
         addr += 2;
         break;
    case 0xfc:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BDRA,R0  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BDRA,R0  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xfd:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BDRA,R1  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BDRA,R1  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xfe:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BDRA,R2  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BDRA,R2  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    case 0xff:
         if (ram[addr+1] & 0x80) {
           sprintf(tmp,"%02x %02x        BDRA,R3  [$%02x%02x]",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         else {
           sprintf(tmp,"%02x %02x        BDRA,R3  $%02x%02x",ram[addr+1],ram[addr+2],
                  ram[addr+1], ram[addr+2]);
           }
         addr += 3;
         break;
    default: 
         sprintf(tmp,"             DB       $%02x",ram[addr]); addr++; break;
    }
  strcat(dest, tmp);
  return addr;
  }

