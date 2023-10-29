#include <stdint.h>
void _0_empty(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _1_write(uint64_t c, uint64_t color, uint64_t r3, uint64_t r4, uint64_t r5);
void _2_read(uint64_t buffer, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _3_getHours(uint64_t hours, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _4_getMinutes(uint64_t min, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _5_getSeconds(uint64_t seconds, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _6_newLine(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _7_write_dec(uint64_t c, uint64_t color, uint64_t r3, uint64_t r4, uint64_t r5);
void _8_beep(uint64_t frequency, uint64_t time, uint64_t r3, uint64_t r4, uint64_t r5);
void _9_get_ticks(uint64_t  delta, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _10_put_pixel(uint64_t color, uint64_t x, uint64_t y, uint64_t r4, uint64_t r5);
void _11_get_screen_width(uint64_t width, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _12_clean_buffer(uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);
void _13_save_registers(uint64_t registers, uint64_t isSaved, uint64_t r3, uint64_t r4, uint64_t r5);

int64_t sysDispatcher(uint64_t syscallNumber, uint64_t r1, uint64_t r2, uint64_t r3, uint64_t r4, uint64_t r5);


