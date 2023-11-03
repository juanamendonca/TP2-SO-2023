#include <stdint.h>
#include <stdbool.h>

void sys_clear_screen();

void sys_write(char *c, int color);

void sys_read(char *buff);

void sys_get_hours(int *hours);

void sys_get_minutes(int *min);

void sys_get_seconds(int *sec);

void sys_write_dec(int c, int color);

void sys_new_line();

void sys_beep(int freq, int time);

void sys_sleep(int ms);

void sys_put_pixel(uint32_t color, uint32_t x, uint32_t y);

void sys_get_screen_width(int *width);

void sys_get_clean_buffer();

void sys_registers(uint64_t *registers, bool *isSaved);

void sys_alloc(uint64_t **memory, int size);
