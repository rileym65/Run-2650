#define MAIN
#include "header.h"
#include "cpu.h"

void clearMem() {
  int i;
  for (i=0; i<32768; i++) ram[i] = 0;
  }

int main(int argc, char** argv) {
  int i;
  printf("Run/2650 v1.0.0\n");
  printf("by Michael H. Riley\n");

  clearMem();
  cpu_reset(&cpu);
  runDebugger = 0xff;
  for (i=1; i<argc; i++) {
    if (strcmp(argv[i], "-d") == 0) runDebugger = 0xff;
    else if (strcmp(argv[i], "-n") == 0) runDebugger = 0x00;
    else if (loadFile(argv[i])) {
      printf("Error: Could not load %s\n",argv[i]);
      return 1;
      }
    }
  cpu_prepare(&cpu);
  if (runDebugger) Debugger();
  else {
    while (cpu.halt == 0) {
      cpu_cycle(&cpu);
      }
    }
  return 0;
  }

