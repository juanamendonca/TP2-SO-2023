#include <stdint.h>

uint64_t sys_int_80(uint64_t syscall, uint64_t r1, uint64_t r2, uint64_t r3,
                    uint64_t r4, uint64_t r5);
