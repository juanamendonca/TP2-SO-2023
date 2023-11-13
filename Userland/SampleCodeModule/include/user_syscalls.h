#include <stdbool.h>
#include <stdint.h>

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

void *sys_alloc(int size);

int sys_create_process(void (*process)(unsigned int argc, char **argv),
                       int argc, char **argv, int foreground, int *fd);

int sys_kill_process(int pid);

int sys_get_pid();

void sys_get_info_processes(char *buffer);

int sys_nice(int pid, int priority);

int sys_block_process(int pid);

int sys_unblock_process(int pid);

void sys_giveup_cpu();

int sys_semPost(int id);

int sys_semWait(int id);

int sys_semClose(char *name);

int sys_semOpen(char *id, int initialValue);

void sys_waitpid(int pid);

void sys_free(void *memory);

void sys_printBitmap();

void sys_sem();
