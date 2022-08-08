#include "header.h"

word address;
char lines[16][80];
word breakpoints[1000];
int  numBreakpoints;
int  tracing;
byte traps[256];
unsigned int clocks;
char conditions[100][256];
int  numConditions;
char useConditions;


void Cls() {
  printf("\e[H\e[2J");
  }

void GotoXY(int x, int y) {
  printf("\e[%d;%dH",y,x);
  }

int IsHex(char* line) {
  if (*line >= '0' && *line <= '9') return -1;
  if (*line >= 'a' && *line <= 'f') return -1;
  if (*line >= 'A' && *line <= 'F') return -1;
  return 0;
  }

word GetHex(char* line) {
  word ret;
  ret = 0;
  while (IsHex(line)) {
    ret <<= 4;
    if (*line >= '0' && *line <= '9') ret |= (*line - '0');
    if (*line >= 'a' && *line <= 'f') ret |= (*line - 87);
    if (*line >= 'A' && *line <= 'F') ret |= (*line - 55);
    line++;
    }
  return ret;
  }

void DrawScreen() {
  Cls();
  printf("+R0+       +------------------------------------------------------------------+\n");
  printf("|--|       |                                                                  |\n");
  printf("+--+       |                                                                  |\n");
  printf("+R1+ +R4+  |                                                                  |\n");
  printf("|--| |--|  |                                                                  |\n");
  printf("+--+ +--+  |                                                                  |\n");
  printf("+R2+ +R5+  |                                                                  |\n");
  printf("|--| |--|  |                                                                  |\n");
  printf("+--+ +--+  |                                                                  |\n");
  printf("+R3+ +R6+  |                                                                  |\n");
  printf("|--| |--|  |                                                                  |\n");
  printf("+--+ +--+  |                                                                  |\n");
  printf("+--PSW--+  |                                                                  |\n");
  printf("+ -- -- |  |                                                                  |\n");
  printf("+-------+  |                                                                  |\n");
  printf("+-PC-+     |                                                                  |\n");
  printf("|----|     |                                                                  |\n");
  printf("+----+     +------------------------------------------------------------------+\n");
  printf("           -- --     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --          -\n"); 
  printf("\n");
  printf("  -->\n");
  printf("    >");
  }

void UpdateScreen() {
  int i;
  GotoXY(2,2); printf("%02x",cpu.r[0]);
  GotoXY(2,5); printf("%02x",cpu.r[1]);
  GotoXY(7,5); printf("%02x",cpu.r[4]);
  GotoXY(2,8); printf("%02x",cpu.r[2]);
  GotoXY(7,8); printf("%02x",cpu.r[5]);
  GotoXY(2,11); printf("%02x",cpu.r[3]);
  GotoXY(7,11); printf("%02x",cpu.r[6]);
  GotoXY(3,14); printf("%02x",cpu.psu);
  GotoXY(6,14); printf("%02x",cpu.psl);
  GotoXY(2,17); printf("%04x",cpu.pc);
  GotoXY(12, 19); printf("%02x",cpu.port);
  GotoXY(15, 19); printf("%02x",cpu.control);
  GotoXY(22, 19);
  for (i=0; i<16; i++) printf("%02x ",cpu.ports[i]);
  GotoXY(79, 19); printf("%d",cpu.sense);
  }

void Output(char* msg) {
  int i;
  for (i=0; i<15; i++) {
    strcpy(lines[i], lines[i+1]);
    GotoXY(13,i+2); printf("%s", lines[i]);
    }
  strcpy(lines[15], msg);
  while (strlen(lines[15]) < 66) strcat(lines[15]," ");
  lines[15][65] = 0;
  GotoXY(13,17); printf("%s",lines[15]); fflush(stdout);
  }

void debugger_qm(char* line) {
  char l[70];
  char t[6];
  int  stack;
  int  x,y;
  stack = 0;
  if (*line == 'c' || *line == 'C') {
    sprintf(l,"Clocks: %u",clocks);
    Output(l);
    line++;
    if (*line == '-') clocks = 0;
    return;
    }
  if (IsHex(line)) address = GetHex(line);
  else if (*line == 'p' || *line == 'P') address = cpu.pc;
  for (y=0; y<16; y++) {
    if (stack) sprintf(l,"%04x:",0x100 + (address & 0xff));
      else sprintf(l,"%04x:",address);
    for (x=0; x<16; x++) {
      if (stack) sprintf(t," %02x",ram[0x100 + ((address++) & 0xff)]);
        else sprintf(t," %02x",ram[address++]);
      strcat(l, t);
      }
    Output(l);
    }
  }

void debugger_dl(char* line) {
  char l[80];
  int  y;
  if (IsHex(line)) address = GetHex(line);
  else if (*line == 'p' || *line == 'P') address = cpu.pc;
  for (y=0; y<16; y++) {
    address = Disassem(address, l);
    Output(l);
    }
  }

void debugger_ex(char*line) {
  word v;
  word addr;
  if (*line == 'P' || *line == 'p') addr = cpu.pc;
  else if (IsHex(line)) {
    addr = GetHex(line);
    while (IsHex(line)) line++;
    while (*line == ' ') line++;
    }
  else return;
  while (IsHex(line)) {
    v = GetHex(line);
    ram[addr++] = v;
    while (IsHex(line)) line++;
    while (*line == ' ') line++;
    }
  }

void debugger_a(char*line) {
  }

void debugger_b(char* line) {
  int  i,j;
  word addr;
  char l[70];
  char t[6];
  if (*line == '?') {
    Output("Breakpoints:");
    i = 0;
    strcpy(l,"  ");
    while (i < numBreakpoints) {
      sprintf(t," %04x", breakpoints[i++]);
      strcat(l, t);
      if (strlen(l) > 62) {
        Output(l);
        strcpy(l,"  ");
        }
      }
    if (strlen(l) > 2) Output(l);
    return;
    }
  if (*line == '+') {
    line++;
    addr = GetHex(line);
    for (i=0; i<numBreakpoints; i++)
      if (breakpoints[i] == addr) return;
    breakpoints[numBreakpoints++] = addr;
    return;
    }
  if (*line == '-') {
    line++;
    addr = GetHex(line);
    for (i=0; i<numBreakpoints; i++)
      if (breakpoints[i] == addr) {
        for (j=i; j<numBreakpoints-1; j++)
          breakpoints[j] = breakpoints[j+1];
        numBreakpoints--;
        return;
        }
    return;
    }
  if (*line == 'c' || *line == 'C') {
    numBreakpoints = 0;
    return;
    }
  }

void debugger_c(char*line) {
  int i;
  int j;
  char outp[300];
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.control = GetHex(line);
      UpdateScreen();
      }
    return;
    }
  if (*line == '?') {
    Output("Conditional breakpoints:");
    for (i=0; i<numConditions; i++) {
      sprintf(outp,"%d:%s",i+1,conditions[i]);
      Output(outp);
      }
    return;
    }
  if (*line == '+' && *(line+1) == '+') {
    useConditions = 'Y';
    Output("Conditions enabled");
    return;
    }
  if (*line == '-' && *(line+1) == '-') {
    useConditions = 'N';
    Output("Conditions disabled");
    return;
    }
  if (*line == '+') {
    line++;
    strcpy(conditions[numConditions++], line);
    return;
    }
  if (*line == '-') {
    line++;
    j = atoi(line) - 1;
    if (j >= 0 && j < numConditions) {
      for (i=j; i<numConditions-1; i++)
        strcpy(conditions[i], conditions[i+1]);
      numConditions--;
      }
    }
  if (*line == 'c' || *line == 'C') {
    numConditions = 0;
    Output("Conditions cleared");
    return;
    }
  }


void debugger_d(char* line) {
  }

void debugger_e(char* line) {
  }

void debugger_f(char* line) {
  }

void debugger_i(char*line) {
  byte b;
  if ( (*line >= '0' && *line <= '9') ||
       (*line >= 'a' && *line <= 'f') ||
       (*line >= 'A' && *line <= 'F')) {
    b = GetHex(line);
    cpu_irq(&cpu,b);
    UpdateScreen();
    return;
    }
  cpu_irq(&cpu, 0);
  UpdateScreen();
  }

void debugger_p(char*line) {
  byte p;
  if ( ((*line >= '0' && *line <= '9') ||
        (*line >= 'a' && *line <= 'f') ||
        (*line >= 'A' && *line <= 'F')) &&
       ((*(line+1) >= '0' && *(line+1) <= '9') ||
        (*(line+1) >= 'a' && *(line+1) <= 'f') ||
        (*(line+1) >= 'A' && *(line+1) <= 'F')) &&
       *(line+2) == '=') {
    if (*line >= '0' && *line <= '9') p = (*line - '0') << 8;
    if (*line >= 'a' && *line <= 'f') p = (*line - 87) << 8;
    if (*line >= 'A' && *line <= 'F') p = (*line - 55) << 8;
    if (*(line+1) >= '0' && *(line+1) <= '9') p |= (*(line+1) - '0');
    if (*(line+1) >= 'a' && *(line+1) <= 'f') p |= (*(line+1) - 87);
    if (*(line+1) >= 'A' && *(line+1) <= 'F') p |= (*(line+1) - 55);
    line += 3;
    if (IsHex(line)) {
      cpu.ports[p] = GetHex(line);
      UpdateScreen();
      }
    return;
    }
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.port = GetHex(line);
      UpdateScreen();
      }
    return;
    }
  if (*line == 'c' || *line == 'C') {
    line++;
    if (*line == '=') {
      line++;
      if (IsHex(line)) {
        cpu.pc = GetHex(line);
        UpdateScreen();
        }
      }
    return;
    }
  }

void debugger_r(char*line) {
  byte b;
  byte i;
  if (*line >= '0' && *line <= '6') i = *line - '0';
  else {
    cpu_reset(&cpu);
    UpdateScreen();
    return;
    }
  line++;
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      b = GetHex(line);
      cpu.r[i] = b & 0xff;
      UpdateScreen();
      }
    }
  }

void debugger_s(char*line) {
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.sense = GetHex(line) & 1;
      UpdateScreen();
      }
    return;
    }
  }

void debugger_u(char*line) {
  }

void debugger_t(char* line) {
  int  i;
  word addr;
  char l[70];
  char t[6];
  if (*line == 'r' || *line == 'R') {
    line++;
    if (*line == '+') tracing = 0xff;
    if (*line == '-') tracing = 0x00;
    return;
    }
  if (*line == '?') {
    Output("Traps:");
    i = 0;
    strcpy(l,"  ");
    for (i=0; i<256; i++) {
      if (traps[i] != 0) {
        sprintf(t," %02x", i);
        strcat(l, t);
        if (strlen(l) > 62) {
          Output(l);
          strcpy(l,"  ");
          }
        }
      }
    if (strlen(l) > 2) Output(l);
    return;
    }
  if (*line == '+') {
    line++;
    addr = GetHex(line);
    traps[addr & 0xff] = 0xff;
    return;
    }
  if (*line == '-') {
    line++;
    addr = GetHex(line);
    traps[addr & 0xff] = 0x00;
    return;
    }
  if (*line == 'c' || *line == 'C') {
    for (i=0; i<256; i++) traps[i] = 0x00;
    return;
    }
  }

void debugger_w(char* line) {
  }

void debugger_x(char*line) {
  }

void debugger_y(char*line) {
  }

void debugger_run(char* line) {
  int i;
  int run;
  char dis[1024];
  run = -1;
  cpu.halt = 0;
  if (IsHex(line)) cpu.pc = GetHex(line);
  while (run) {
    if (tracing) {
      Disassem(cpu.pc, dis);
      Output(dis);
      }
    cpu_cycle(&cpu);
    clocks += cpu.ts;
    if (cpu.halt) run = 0;
    for (i=0; i<numBreakpoints; i++)
      if (cpu.pc == breakpoints[i]) {
        run = 0;
        UpdateScreen();
        }
    if (useConditions == 'Y') {
      for (i=0; i<numConditions; i++)
        if (evaluate(conditions[i]) != 0) {
          run = 0;
          UpdateScreen();
          }
      }
    if (traps[ram[cpu.pc]] != 0) {
      run = 0;
      UpdateScreen();
      }
    }
  UpdateScreen();
  }

void Debugger() {
  int i;
  int flag;
  char line[1024];
  char dis[1024];
  for (i=0; i<15; i++)
    strcpy(lines[i],"                                                                  ");
  for (i=0; i<256; i++) traps[i] = 0;
  address = 0;
  numBreakpoints = 0;
  numConditions = 0;
  useConditions = 'Y';
  tracing = 0;
  clocks = 0;
  DrawScreen();
  UpdateScreen();
  flag = -1;
  while (flag) {
    GotoXY(6, 21); printf("                                                               ");
    GotoXY(6, 22); printf("                                                               ");
    Disassem(cpu.pc+cpu.page, line);
    GotoXY(6, 21); printf("%s",line);
    GotoXY(1, 22); printf("%04x>",cpu.pc+cpu.page);
    fgets(line, 1023, stdin);
    while (strlen(line) > 0 && line[strlen(line)-1] <= ' ')
      line[strlen(line)-1] = 0;
    if (strcmp(line,"/") == 0) flag = 0;
    if (line[0] == '?') debugger_qm(line+1);
    if (line[0] == '$') debugger_dl(line+1);
    if (line[0] == '!') debugger_ex(line+1);
    if (line[0] == 'a' || line[0] == 'A') debugger_a(line+1);
    if (line[0] == 'b' || line[0] == 'B') debugger_b(line+1);
    if (line[0] == 'c' || line[0] == 'C') debugger_c(line+1);
    if (line[0] == 'd' || line[0] == 'D') debugger_d(line+1);
    if (line[0] == 'e' || line[0] == 'E') debugger_e(line+1);
    if (line[0] == 'f' || line[0] == 'F') debugger_f(line+1);
    if (line[0] == 'i' || line[0] == 'I') debugger_i(line+1);
    if (line[0] == 'p' || line[0] == 'P') debugger_p(line+1);
    if (line[0] == 'r' || line[0] == 'R') debugger_r(line+1);
    if (line[0] == 's' || line[0] == 'S') debugger_s(line+1);
    if (line[0] == 't' || line[0] == 'T') debugger_t(line+1);
    if (line[0] == 'u' || line[0] == 'U') debugger_u(line+1);
    if (line[0] == 'w' || line[0] == 'W') debugger_w(line+1);
    if (line[0] == 'x' || line[0] == 'X') debugger_x(line+1);
    if (line[0] == 'y' || line[0] == 'Y') debugger_y(line+1);
    if (line[0] == '@') debugger_run(line+1);
    if (strlen(line) == 0) {
      if (tracing) {
        Disassem(cpu.pc+cpu.page, dis);
        Output(dis);
        }
      cpu_cycle(&cpu);
      clocks += cpu.ts;
      UpdateScreen();
      }
    }
  }



