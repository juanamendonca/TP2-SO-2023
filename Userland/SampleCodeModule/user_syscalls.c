#include <stdint.h>
#include <stdbool.h>
#include <user_syscalls.h>
#include <user_lib.h>

void sys_clear_screen()
{
    sys_int_80((uint64_t)0, 0, 0, 0, 0, 0);
}

void sys_write(char *c, int color)
{
    sys_int_80((uint64_t)1, (uint64_t)c, color, 0, 0, 0);
}

void sys_read(char *buff)
{
    sys_int_80((uint64_t)2, (uint64_t)buff, 0, 0, 0, 0);
}

void sys_get_hours(int *hours)
{
    sys_int_80((uint64_t)3, (uint64_t)hours, 0, 0, 0, 0);
}

void sys_get_minutes(int *min)
{
    sys_int_80((uint64_t)4, (uint64_t)min, 0, 0, 0, 0);
}

void sys_get_seconds(int *sec)
{
    sys_int_80((uint64_t)5, (uint64_t)sec, 0, 0, 0, 0);
}

void sys_new_line()
{
    sys_int_80((uint64_t)6, 0, 0, 0, 0, 0);
}

void sys_write_dec(int c, int color)
{
    sys_int_80((uint64_t)7, (uint64_t)c, color, 0, 0, 0);
}

void sys_beep(int freq, int time)
{
    sys_int_80((uint64_t)8, (uint64_t)freq, (uint64_t)time, 0, 0, 0);
}

void sys_sleep(int ms)
{
    sys_int_80((uint64_t)9, (uint64_t)ms, 0, 0, 0, 0);
}

void sys_put_pixel(uint32_t color, uint32_t x, uint32_t y)
{
    sys_int_80((uint64_t)10, (uint64_t)color, (uint64_t)x, (uint64_t)y, 0, 0);
}

void sys_get_screen_width(int *width)
{
    sys_int_80((uint64_t)11, (uint64_t)width, 0, 0, 0, 0);
}

void sys_get_clean_buffer()
{
    sys_int_80((uint64_t)12, 0, 0, 0, 0, 0);
}

void sys_registers(uint64_t *registers, bool *isSaved)
{
    sys_int_80((uint64_t)13, (uint64_t)registers, (uint64_t)isSaved, 0, 0, 0);
}

void sys_alloc(uint64_t **memory, int size)
{
    sys_int_80((uint64_t)14, (uint64_t)memory, (uint64_t)size, 0, 0, 0);
}
