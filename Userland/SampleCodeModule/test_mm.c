#include "syscall.h"
#include "test_util.h"
#include <stdlib.h>
#include <string.h>
#include "user_syscalls.h"

#define MAX_BLOCKS 1280

typedef struct MM_rq {
    void *address;
    uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]) {
    sys_write("Beginning test_mm...\n", 0xFFFFFF);
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    uint64_t max_memory;

    if (argc != 1) {
        sys_write("argc != 1\n", 0xFFFFFF);
        return -1;
    }

    if ((max_memory = satoi(argv[0])) <= 0) {
        sys_write("max memory error\n", 0xFFFFFF);
        return -1;
    }


    while (1) {
        rq = 0;
        total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory) {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = sys_alloc(mm_rqs[rq].size);

            if (mm_rqs[rq].address) {
                total += mm_rqs[rq].size;
                rq++;
            }
        }

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                memset(mm_rqs[i].address, i, mm_rqs[i].size);

        // Check
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
                    sys_write("test_mm ERROR\n",0xFF0000);
                    return -1;
                }

        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                sys_free(mm_rqs[i].address);

    }
    sys_write("Test_mm done!\n", 0xFFFFFF);
    return 0;

}