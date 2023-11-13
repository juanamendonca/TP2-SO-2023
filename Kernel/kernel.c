#include "idtLoader.h"
#include "keyboard_driver.h"
#include "lib.h"
#include "memoryManager.h"
#include "moduleLoader.h"
#include "naiveConsole.h"
#include "pipe.h"
#include "scheduler.h"
#include "semaphores2.h"
#include "sound.h"
#include "time.h"
#include "video.h"
#include <stdbool.h>
#include <stdint.h>

#define HEAP_SIZE 0x100000

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
uint64_t *registers;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static void *const memoryManagerModuleAddress = (void *)0x0000000000050000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8       // Size of the stack (32KiB)
                  - sizeof(uint64_t) // Starts at the top of the stack
  );
}

void *initializeKernelBinary() {
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};
  loadModules(&endOfKernelBinary, moduleAddresses);
  clearBSS(&bss, &endOfKernel - &bss);
  return getStackBase();
}

int main() {
  createMemoryManager();
  initalizeScheduler();
  start_semaphores();
  initPipes();

  char *args[] = {"shell"};
  int ufd[2] = {0, 0};
  initalizeProcess(sampleCodeModuleAddress, 1, args, 1, ufd);
  load_idt(); // sets the IDT before the terminal launches

  return 0;
}
