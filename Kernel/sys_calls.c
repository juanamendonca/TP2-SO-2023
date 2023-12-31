#include "sys_calls.h"
#include "keyboard_driver.h"
#include "lib.h"
#include "memoryManager.h"
#include "pipe.h"
#include "scheduler.h"
#include "semaphores2.h"
#include "sound.h"
#include "time.h"
#include "video.h"
#include <stdbool.h>
#include <stdint.h>

extern const uint64_t regs[18];

extern int getHours();
extern int getMinutes();
extern int getSeconds();

typedef int64_t (*syscall)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

void _0_empty(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5) {
  clearScreen();
}

void _1_write(uint64_t c, uint64_t color, uint64_t r3, uint64_t r4,
              uint64_t r5) {
  putArrayNext((char *)c, color);
}

void _2_read(uint64_t buffer, uint64_t r2, uint64_t r3, uint64_t r4,
             uint64_t r5) {
  ((char *)buffer)[0] = nextElement();
}

void _3_getHours(uint64_t hours, uint64_t r2, uint64_t r3, uint64_t r4,
                 uint64_t r5) {
  *((int *)hours) = getHours();
}

void _4_getMinutes(uint64_t min, uint64_t r2, uint64_t r3, uint64_t r4,
                   uint64_t r5) {
  *((int *)min) = getMinutes();
}

void _5_getSeconds(uint64_t seconds, uint64_t r2, uint64_t r3, uint64_t r4,
                   uint64_t r5) {
  *((int *)seconds) = getSeconds();
}

void _6_newLine(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                uint64_t r5) {
  putArrayNext("\n", WHITE);
}

void _7_write_dec(uint64_t c, uint64_t color, uint64_t r3, uint64_t r4,
                  uint64_t r5) {
  putDecNext((int)c, color);
}

void _8_beep(uint64_t frequency, uint64_t time, uint64_t r3, uint64_t r4,
             uint64_t r5) {
  beep((int)frequency, (int)time);
}

void _9_get_ticks(uint64_t delta, uint64_t r2, uint64_t r3, uint64_t r4,
                  uint64_t r5) {
  sleepms((int)delta);
}

void _10_put_pixel(uint64_t color, uint64_t x, uint64_t y, uint64_t r4,
                   uint64_t r5) {
  putPixel((uint32_t)color, (uint32_t)x, (uint32_t)y);
}

void _11_get_screen_width(uint64_t width, uint64_t r2, uint64_t r3, uint64_t r4,
                          uint64_t r5) {
  *((int *)width) = getWidth();
}

void _12_clean_buffer(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                      uint64_t r5) {
  cleanBuffer();
}

void _13_save_registers(uint64_t registers, uint64_t isSaved, uint64_t r3,
                        uint64_t r4, uint64_t r5) {
  if (!isRegsSaved()) {
    (*(bool *)isSaved) = false;
    return;
  }
  (*(bool *)isSaved) = true;
  for (int i = 0; i < 18; i++) {
    ((uint64_t *)registers)[i] = regs[i];
  }
}

void *_14_alloc(uint64_t size, uint64_t r2, uint64_t r3, uint64_t r4,
                uint64_t r5) {
  return malloc(size);
}

int _15_create_process(uint64_t process, uint64_t argc, uint64_t argv,
                       uint64_t foreground, uint64_t fd) {
  return initalizeProcess((void (*)(int argc, char **argv))process, (int)argc,
                          (char **)argv, (int)foreground, (int *)fd);
}

int _16_kill_process(uint64_t pid, uint64_t r2, uint64_t r3, uint64_t r4,
                     uint64_t r5) {
  return killProcess((int)pid);
}

int _17_get_pid(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                uint64_t r5) {
  return getPid();
}

void _18_get_info_processes(uint64_t buffer, uint64_t r2, uint64_t r3,
                            uint64_t r4, uint64_t r5) {
  processesInfo((char *)buffer);
}

int _19_nice(uint64_t pid, uint64_t priority, uint64_t r3, uint64_t r4,
             uint64_t r5) {
  return nice((int)pid, (int)priority);
}

int _20_block_process(uint64_t pid, uint64_t r2, uint64_t r3, uint64_t r4,
                      uint64_t r5) {
  return block((int)pid);
}

int _21_unblock_process(uint64_t pid, uint64_t r2, uint64_t r3, uint64_t r4,
                        uint64_t r5) {
  return unblock((int)pid);
}

void _22_giveup_cpu(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                    uint64_t r5) {
  giveUpCPU();
}

int _23_sem_wait(uint64_t semIndex, uint64_t r2, uint64_t r3, uint64_t r4,
                 uint64_t r5) {
  return sem_wait((int)semIndex);
}

int _24_sem_post(uint64_t semIndex, uint64_t r2, uint64_t r3, uint64_t r4,
                 uint64_t r5) {
  return sem_post((int)semIndex);
}

int _25_sem_open(uint64_t name, uint64_t initValue, uint64_t r3, uint64_t r4,
                 uint64_t r5) {
  return sem_open((char *)name, (int)initValue);
}

int _26_sem_close(uint64_t id, uint64_t r2, uint64_t r3, uint64_t r4,
                  uint64_t r5) {
  return sem_close((int)id);
}

void _27_waitpid(uint64_t pid, uint64_t r2, uint64_t r3, uint64_t r4,
                 uint64_t r5) {
  waitpid((int)pid);
}

void _29_free(uint64_t memory, uint64_t r2, uint64_t r3, uint64_t r4,
              uint64_t r5) {
  void *ptr = (void *)(uintptr_t)memory;
  free(ptr);
}

void _28_sem_(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5) {
  sem();
}

void _30_printBitmap(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                     uint64_t r5) {
  printBitmap();
}

int _31_pipe_open(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                  uint64_t r5) {
  return pipeOpen((char *)r1);
}

int _32_pipe_close(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                   uint64_t r5) {
  return pipeClose((uint64_t)r1);
}

char _33_pipe_read(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                   uint64_t r5) {
  return readPipe((uint64_t)r1);
}

int _34_pipe_write(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4,
                   uint64_t r5) {
  return writePipe((uint64_t)r1, (char *)r2);
}

void _35_put_char(uint64_t c, uint64_t color, uint64_t r3, uint64_t r4,
                  uint64_t r5) {
  putChar((char)c, (int)color);
}

void _36_pipe(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5) {
  pipe();
}

static syscall syscalls[] = {(syscall)_0_empty,
                             (syscall)_1_write,
                             (syscall)_2_read,
                             (syscall)_3_getHours,
                             (syscall)_4_getMinutes,
                             (syscall)_5_getSeconds,
                             (syscall)_6_newLine,
                             (syscall)_7_write_dec,
                             (syscall)_8_beep,
                             (syscall)_9_get_ticks,
                             (syscall)_10_put_pixel,
                             (syscall)_11_get_screen_width,
                             (syscall)_12_clean_buffer,
                             (syscall)_13_save_registers,
                             (syscall)_14_alloc,
                             (syscall)_15_create_process,
                             (syscall)_16_kill_process,
                             (syscall)_17_get_pid,
                             (syscall)_18_get_info_processes,
                             (syscall)_19_nice,
                             (syscall)_20_block_process,
                             (syscall)_21_unblock_process,
                             (syscall)_22_giveup_cpu,
                             (syscall)_23_sem_wait,
                             (syscall)_24_sem_post,
                             (syscall)_25_sem_open,
                             (syscall)_26_sem_close,
                             (syscall)_27_waitpid,
                             (syscall)_28_sem_,
                             (syscall)_29_free,
                             (syscall)_30_printBitmap,
                             (syscall)_31_pipe_open,
                             (syscall)_32_pipe_close,
                             (syscall)_33_pipe_read,
                             (syscall)_34_pipe_write,
                             (syscall)_35_put_char,
                             (syscall)_36_pipe};

int64_t sysDispatcher(uint64_t syscallNumber, uint64_t r1, uint64_t r2,
                      uint64_t r3, uint64_t r4, uint64_t r5) {
  return syscalls[syscallNumber](r1, r2, r3, r4, r5);
}